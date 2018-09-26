#include "CLoader.h"
#include "CObjectPool.h"
#include "CPyramid.h"
#include "CPyramidNode.h"
#include "CTaskSet.h"
#include "CSource.h"
#include "CSourceList.h"
#include "..\Common\CConfig.h"
#include "..\Common\CHelper.h"
#include "..\Common\CImg.h"
#include "..\Common\CMemory.h"
#include "..\FileSystem\CDataFileBuilder.h"
#include "..\Projection\CProjectionUtm.h"
#include "..\Projection\CProjectionNASA.h"
#include "..\Projection\CProjectionBing.h"
#include "..\..\CxImage\ximage.h"
#include <Windows.h>

///////////////////////// tileimage pool /////////////////////////

void* FuncNewTileImage(CLoader *pLoader)
{
    CImg *pImg = new CImg(pLoader->projTile->tileWidth, pLoader->projTile->tileHeight);
    pImg->CreateAlpha();
    pLoader->stat.tileInMemoryCnt++;
    return pImg;
}
void FuncDeleteTileImage(CLoader *pLoader, void *pObject)
{
    delete (CImg*)pObject;
    pLoader->stat.tileInMemoryCnt--;
}
void FuncResetTileImage(void *pObject)
{
    CImg *pImg = (CImg*)pObject;
    pImg->Clear();
    pImg->ClearAlpha();
}

///////////////////////// memfile pool /////////////////////////

void* FuncNewMemFile(CLoader *pLoader) { CxMemFile *pMemFile = new CxMemFile(); pMemFile->Open(); return pMemFile;}
void FuncDeleteMemFile(CLoader *pLoader, void *pObject) {  delete (CxMemFile*)pObject; }
void FuncResetMemFile(void *pObject) { ((CxMemFile*)pObject)->Seek(0, SEEK_SET);}

///////////////////////// pyramidnode pool /////////////////////////

void* FuncNewPyramidNode(CLoader *pLoader) { return new CPyramidNode(pLoader);}
void FuncDeletePyramidNode(CLoader *pLoader, void *pObject) {  delete (CPyramidNode*)pObject; }
void FuncResetPyramidNode(void *pObject) { }

///////////////////////// doublearray pool /////////////////////////

void* FuncNewDoubleArray(CLoader *pLoader) { return new SDoubleXY[pLoader->projTile->tileWidth * pLoader->projTile->tileHeight];}
void FuncDeleteDoubleArray(CLoader *pLoader, void *pObject) {  delete (SDoubleXY*)pObject; }
void FuncResetDoubleArray(void *pObject) { }

int CLoader::RunTask(void *param)
{
    CTaskSet *pTaskSet = (CTaskSet*)param;
    try
    {
        pTaskSet->pLoader->stat.runningThreadCnt++;
        pTaskSet->Run();
    }
    catch (char * msg) { Log("Thread Exception: %s", msg); }
    catch (...) { Log("Thread Exception: unknown exception"); }
    pTaskSet->pLoader->stat.runningThreadCnt--;
    return 0;
}



CLoader::CLoader(string outputDir, const SSourceSetMetaInfo sourceSetMetaInfo)
    : CSemaphore(1, 1)
{
    // init tiff lib
    CHelper::InitTiffLib();

    // init projection
    if (strcmp(CConfig::DEST_PROJECTION, "UTM") == 0 && strcmp(CConfig::SOURCE_PROJECTION, "UTM") != 0)
        LogThrow("CLoader: when dest projection is UTM, source projection must also be UTM");
    projSource = CreateProjection(CConfig::SOURCE_PROJECTION);
    projTile = CreateProjection(CConfig::DEST_PROJECTION);
    
    // init members
    this->sourceSetMetaInfo = sourceSetMetaInfo;
    this->pDataFileBuilder = new CDataFileBuilder(outputDir);
	
    double tileSize = (double)projTile->tileWidth * projTile->tileHeight;
    pPoolTileImage = new CObjectPool(this, tileSize * (BYTE_PER_PIXEL+1), 10000, FuncNewTileImage, FuncDeleteTileImage, FuncResetTileImage);
    pPoolMemFile = new CObjectPool(this, 0, MEMFILE_POOL_CAP, FuncNewMemFile, FuncDeleteMemFile, FuncResetMemFile);    // does not count memfile memory usage
    pPoolPyramidNode = new CObjectPool(this, sizeof(CPyramidNode), 200000, FuncNewPyramidNode, FuncDeletePyramidNode, FuncResetPyramidNode);
    pPoolDoubleArray = new CObjectPool(this, tileSize * 8, 4000, FuncNewDoubleArray, FuncDeleteDoubleArray, FuncResetDoubleArray);

	pPyramid = new CPyramid(this);
    pSourceList = new CSourceList();
	pTaskSetWrite = new CTaskSet(this);
	pTaskSetCalc = new CTaskSet(this);
	pTaskSetLoad = new CTaskSet(this);
    pTaskSetPreload = new CTaskSet(this);
	memset(&stat, 0, sizeof(stat));
	
	pMemSource = new CMemory(CConfig::MEMORY_LIMIT_SOURCE);
	pMemTile = new CMemory(CConfig::MEMORY_LIMIT_TILE);
}

CLoader::~CLoader()
{
    delete pTaskSetWrite;
	delete pTaskSetCalc;
    delete pTaskSetLoad;
    delete pTaskSetPreload;
    delete pSourceList;
    delete pPyramid;
    delete pPoolDoubleArray;
    delete pPoolPyramidNode;
    delete pPoolMemFile;
    delete pPoolTileImage;
	delete pMemSource;
	delete pMemTile;
	delete pDataFileBuilder;
    delete projSource;
    delete projTile;
}

void CLoader::Abort()
{
    pTaskSetWrite->Close();
    pTaskSetCalc->Close();
    pTaskSetLoad->Close();
    pTaskSetPreload->Close();
}

void CLoader::AddSource(SSourceMetaInfo sourceMetaInfo, string dataPath)
{
	CSource* pSource = new CSource(this, sourceMetaInfo, dataPath);
    pSource->CreatePyramidNodes();
    pSourceList->AddSource(pSource);
}

void CLoader::Load()
{
    Log("--------- Load Start ---------");

    // adjust memory
    pSourceList->AdjustMemoryLimit(pMemSource, pMemTile);

    // add starting tasks
    pPyramid->CheckDependencyCount(pSourceList->GetNodeLinkCnt());
    ;
    stat.tileMemoryEstimate = (int)(pSourceList->CreatePreLoadTasks(pPyramid->nodes) * 1.0 * projTile->tileWidth * projTile->tileHeight * 4 / 1024 / 1024);
    stat.tileMemoryLimit = (int)(pMemTile->limitBytes / 1024 / 1024);

    // prepare threads
    int totalThreadsCnt = CConfig::PRELOAD_THREAD_CNT + CConfig::LOAD_THREAD_CNT + CConfig::CALC_THREAD_CNT + CConfig::WRITE_THREAD_CNT;
    HANDLE *handles = new HANDLE[totalThreadsCnt];
    int i = 0, j = 0;
    for (j=0; j<CConfig::PRELOAD_THREAD_CNT; j++, i++)
	{
        handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunTask, pTaskSetPreload, 0, NULL);
	}
    for (j=0; j<CConfig::LOAD_THREAD_CNT; j++, i++)
	{
        handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunTask, pTaskSetLoad, 0, NULL);
	}
	for (j=0; j<CConfig::CALC_THREAD_CNT; j++, i++)
	{
        handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunTask, pTaskSetCalc, 0, NULL);
	}
	for (j=0; j<CConfig::WRITE_THREAD_CNT; j++, i++)
	{
        handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunTask, pTaskSetWrite, 0, NULL);
	}

    // wait for threads to exit
    for (i=0; i<totalThreadsCnt; i++)
        WaitForSingleObject(handles[i], INFINITE);
    delete[] handles;
    Log("--------- Load End ---------");
}

SLoaderStat CLoader::GetStat()
{
    stat.unfinishedPreloadTaskCnt = pTaskSetPreload->Length();
    stat.unfinishedLoadTaskCnt = pTaskSetLoad->Length();
    stat.unfinishedCalcTaskCnt = pTaskSetCalc->Length();
    stat.unfinishedWriteTaskCnt = pTaskSetWrite->Length();
    stat.finishedPreloadTaskCnt = pTaskSetPreload->totalFinishedTaskCnt;
    stat.finishedLoadTaskCnt = pTaskSetLoad->totalFinishedTaskCnt;
    stat.finishedCalcTaskCnt = pTaskSetCalc->totalFinishedTaskCnt;
    stat.finishedWriteTaskCnt = pTaskSetWrite->totalFinishedTaskCnt;
    stat.threadCntWaitingForSourceMemory = pMemSource->GetThreadCntBlocking();
    stat.threadCntWaitingForTileMemory = pMemTile->GetThreadCntBlocking();
    stat.memoryUsedBySource = (int)(pMemSource->GetUsedBytes() / 1024 / 1024);
    stat.memoryUsedByTile = (int)(pMemTile->GetUsedBytes() / 1024 / 1024);
    stat.memoryUsedByTileMax = max(stat.memoryUsedByTileMax, stat.memoryUsedByTile);
    return stat;
}

void CLoader::OutputTile(STileOutput tileOutput)
{
	pDataFileBuilder->WriteTile(&tileOutput);
    pPoolMemFile->ReturnObject(tileOutput.pMemFile);
	IncreaseTileOutputCnt();
}

void CLoader::IncreaseTileOutputCnt()
{
	Lock();
	stat.tileOutputedCnt++;
	if (stat.tileOutputedCnt == stat.tileTotalCnt)
		Abort();
	Unlock();
}

CProjection* CLoader::CreateProjection(string projectionName)
{
    if (projectionName == "UTM")
        return new CProjectionUtm();
    else if (projectionName == "NASA")
        return new CProjectionNASA();
    else if (projectionName == "BING")
        return new CProjectionBing();
    else
    {
        LogThrow("CLoader::CreateProjection: projection name unknow: %s", projectionName.c_str());
        return NULL; // remove compiler warning
    }
}

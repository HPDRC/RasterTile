#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

class CLoader : public CSemaphore
{
private:
    static int RunTask(void *param);

private:
    CProjection* CreateProjection(string projectionName);

public:
	// a loader has one pyramid
	CPyramid *pPyramid;

    // from which projection, to which projection
    CProjection *projSource, *projTile;

    // this list manage all sources
    CSourceList *pSourceList;

    // a loader has one preload taskset
	CTaskSet *pTaskSetPreload, *pTaskSetLoad, *pTaskSetCalc, *pTaskSetWrite;

    CObjectPool *pPoolTileImage, *pPoolMemFile, *pPoolPyramidNode, *pPoolDoubleArray;
    
	// status of this loader
	SLoaderStat stat;

    // one loader loads one sourceset, and must know its info
    SSourceSetMetaInfo sourceSetMetaInfo;

    // one loader has one builder
    CDataFileBuilder *pDataFileBuilder;

	// these two controls use of memory by source and tile
	CMemory *pMemSource, *pMemTile;

/////////////////////////////// functions for external use //////////////////////////////////
public:
    CLoader(string outputDir, const SSourceSetMetaInfo sourceSetMetaInfo);
    ~CLoader();

    void AddSource(SSourceMetaInfo sourceMetaInfo, string dataPath);

    // start load process
    void Load();

    // abort the load process
    void Abort();

    SLoaderStat GetStat();

////////////////////////////// functions for internal(in the same library) use /////////////
public:
	void OutputTile(STileOutput tileOutput);

	void IncreaseTileOutputCnt();
        
//////////////////////////////////// configurable //////////////////////////////////////////

	// how many ms will the thread sleep when there is not enough memory
	static const int sleep_nomemory = 500;
};

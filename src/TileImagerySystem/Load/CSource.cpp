#include "CSource.h"
#include "CPyramid.h"
#include "CPyramidNode.h"
#include "CLoader.h"
#include "CTaskSet.h"
#include "..\Common\CDoqq.h"
#include "..\Common\CConfig.h"
#include "..\Common\CMemory.h"
#include "..\Common\CFile.h"
#include "..\Common\CImg.h"
#include "..\Projection\CProjection.h"
#include "..\..\CxImage\ximage.h"

#define COVER_SOURCE_MARGIN (100)

CSource::CSource(CLoader *pLoader, SSourceMetaInfo sourceMetaInfo, string filePath)
    : CSemaphore(1, 1)
{
	this->pLoader = pLoader;
    this->sourceMetaInfo = sourceMetaInfo;
	this->filePath = filePath;
	this->ungeneratedTileCnt = 0;
    this->pImg = NULL;
    this->loadLevel = 0;
	this->fileBuffer = NULL;
	this->fileLength = (int)CFile::GetSize(filePath);

    CalculateTileBoundary();
}

CSource::~CSource()
{
	delete pImg;
    pLoader->pMemSource->Release((double)sourceMetaInfo.width * sourceMetaInfo.height * (BYTE_PER_PIXEL + (CConfig::TRIM_ENABLED ? 1 : 0)));
}

void CSource::Preload()
{
    while (pLoader->pTaskSetWrite->Length() > MEMFILE_POOL_CAP * 0.8)
    {
        Sleep(3000);
    }

	// allocate memory
	pLoader->pMemSource->Allocate((double)sourceMetaInfo.width * sourceMetaInfo.height * (BYTE_PER_PIXEL + (CConfig::TRIM_ENABLED ? 1 : 0)) + fileLength);

    // create file
    CFile* file = new CFile(filePath, false, 2);

    // get file length
    fileBuffer = new U8[fileLength];

    // read into memory
    file->SeekStrict(0, SEEK_SET);
    file->ReadStrict(fileBuffer, fileLength);

    // close file
    delete file;
}

void CSource::Load()
{
	switch (sourceMetaInfo.fileType)
	{
	case doqq:
		pImg = CDoqq::LoadDoqq(sourceMetaInfo, fileBuffer, fileLength, filePath.c_str(), false);
		break;
	case doqqc:
		pImg = CDoqq::LoadDoqq(sourceMetaInfo, fileBuffer, fileLength, filePath.c_str(), true);
		break;
	case tif:
		pImg = new CImg(fileBuffer, fileLength, CXIMAGE_FORMAT_TIF);
		break;
	case jpg:
		pImg = new CImg(fileBuffer, fileLength, CXIMAGE_FORMAT_JPG);
		break;
	case png:
		pImg = new CImg(fileBuffer, fileLength, CXIMAGE_FORMAT_PNG);
		break;
	default:
		LogThrow("CSource: unknow file type");
	}

    if (CConfig::TRIM_ENABLED)
    {
        vector<SMask> localMasks;
        GenerateLocalMasks(&localMasks);
        pImg->Trim(&localMasks);
    }
}

void CSource::GenerateLocalMasks(vector<SMask> *pMasks)
{
    SRectF sourceLocalBoundary;
    sourceLocalBoundary.top = sourceLocalBoundary.left = 0;
    sourceLocalBoundary.right = this->pImg->width;
    sourceLocalBoundary.bottom = this->pImg->height;
    
    SRectF sourceLatLonRect, maskLocalRect;
    SLatLon latlon;
    SSourcePixelXY sourcePixel;
    sourcePixel.sourceMetaInfo = this->sourceMetaInfo;

    for (int i=0; i<CConfig::masks.size(); i++)
    {
        // convert mask rectangle to radian latlon first
        SMask mask = CConfig::masks[i];
        SRectF maskLatlonRect = CConfig::masks[i].rect;
        maskLatlonRect.top = maskLatlonRect.top * M_PI / 180;
        maskLatlonRect.bottom = maskLatlonRect.bottom * M_PI / 180;
        maskLatlonRect.left = maskLatlonRect.left * M_PI / 180;
        maskLatlonRect.right = maskLatlonRect.right * M_PI / 180;

        // converting a latlon rectangle to a local projection rectangle will suffer from distortion
        // to reduce distortion, obtain a preciser rectange box by:
        //      A. convert the source rectangle to a latlon rectangle which guanrantees to cover the source
        //      B. clip mask to the latlon rectangle 
        //      C. convert this new mask to maximum local projection rectangle
        // for REMOVE (type==3) mask, A and B will be skipped

        if (mask.type != 3)
        {
            ////////////////// step A ////////////////////
            sourcePixel.offsetw = -COVER_SOURCE_MARGIN, sourcePixel.offseth = -COVER_SOURCE_MARGIN;
            pLoader->projSource->SourceToLatLon(sourcePixel, latlon);
            sourceLatLonRect.left = sourceLatLonRect.right = latlon.lon;
            sourceLatLonRect.top = sourceLatLonRect.bottom = latlon.lat;

            sourcePixel.offsetw = this->pImg->width + COVER_SOURCE_MARGIN, sourcePixel.offseth = -COVER_SOURCE_MARGIN;
            pLoader->projSource->SourceToLatLon(sourcePixel, latlon);
            sourceLatLonRect.EnlargeToInclude(latlon.lon, latlon.lat);

            sourcePixel.offsetw = this->pImg->width + COVER_SOURCE_MARGIN, sourcePixel.offseth = this->pImg->height + COVER_SOURCE_MARGIN;
            pLoader->projSource->SourceToLatLon(sourcePixel, latlon);
            sourceLatLonRect.EnlargeToInclude(latlon.lon, latlon.lat);

            sourcePixel.offsetw = -COVER_SOURCE_MARGIN, sourcePixel.offseth = this->pImg->height + COVER_SOURCE_MARGIN;
            pLoader->projSource->SourceToLatLon(sourcePixel, latlon);
            sourceLatLonRect.EnlargeToInclude(latlon.lon, latlon.lat);

            ////////////////// step B ////////////////////
            maskLatlonRect.Clip(sourceLatLonRect);
        }

        ////////////////// step C ////////////////////
        latlon.lon = maskLatlonRect.left;
        latlon.lat = maskLatlonRect.top;
        pLoader->projSource->LatLonToSource(latlon, sourcePixel);
        maskLocalRect.top = sourcePixel.offseth;
        maskLocalRect.left = sourcePixel.offsetw;

        latlon.lon = maskLatlonRect.right;
        latlon.lat = maskLatlonRect.bottom;
        pLoader->projSource->LatLonToSource(latlon, sourcePixel);
        maskLocalRect.bottom = sourcePixel.offseth;
        maskLocalRect.right = sourcePixel.offsetw;

        latlon.lon = maskLatlonRect.right;
        latlon.lat = maskLatlonRect.top;
        pLoader->projSource->LatLonToSource(latlon, sourcePixel);
        maskLocalRect.EnlargeToInclude(sourcePixel.offsetw, sourcePixel.offseth);

        latlon.lon = maskLatlonRect.left;
        latlon.lat = maskLatlonRect.bottom;
        pLoader->projSource->LatLonToSource(latlon, sourcePixel);
        maskLocalRect.EnlargeToInclude(sourcePixel.offsetw, sourcePixel.offseth);

        // skip if it doesn't overlaps with this source
        if (!maskLocalRect.IntersectWith(sourceLocalBoundary))
            continue;
        
        // add mask to list
        mask.rect = maskLocalRect;
        pMasks->push_back(mask);
    }
}

void CSource::Release()
{
    delete fileBuffer, fileBuffer = NULL;
	pLoader->pMemSource->Release(fileLength);
}

void CSource::CalculateTileBoundary()
{
    SSourcePixelXY sourcexy[4];
	STileXY corners[4];
    
    // prepare 4 corners
    for (int i=0; i<4; i++)
        sourcexy[i].sourceMetaInfo = sourceMetaInfo;
    sourcexy[0].offsetw = 0;
    sourcexy[0].offseth = 0;
	sourcexy[1].offsetw = sourceMetaInfo.width - 1;
    sourcexy[1].offseth = 0;
    sourcexy[2].offsetw = 0;
    sourcexy[2].offseth = sourceMetaInfo.height - 1;
    sourcexy[3].offsetw = sourceMetaInfo.width - 1;
    sourcexy[3].offseth = sourceMetaInfo.height - 1;

	// get the center point of source
	SSourcePixelXY centerOfSource;
	centerOfSource.sourceMetaInfo = sourceMetaInfo;
	centerOfSource.offsetw = sourceMetaInfo.width / 2;
	centerOfSource.offseth = sourceMetaInfo.height / 2;
	// calc the center point should load to which level
    loadLevel = pLoader->projTile->SourceToProperLevel(*pLoader->projSource, centerOfSource);

	// get tilexy of the 4 corners
	for (int i=0; i<4; i++)
    {
        STilePixelXY tpxy;
        pLoader->projTile->SourceToTile(*pLoader->projSource, sourcexy[i], tpxy, loadLevel);
        corners[i] = tpxy.tilexy;
    }
    
    // confirm the range of nodes which can be generated from this source by using the 4 corners
	tilexmin = corners[0].x, tilexmax = corners[0].x;
	tileymin = corners[0].y, tileymax = corners[0].y;
	for (int i=0; i<4; i++)
	{
		tilexmin = min(tilexmin, (int)corners[i].x), tilexmax = max(tilexmax, (int)corners[i].x);
		tileymin = min(tileymin, (int)corners[i].y), tileymax = max(tileymax, (int)corners[i].y);
	}
}

bool CSource::ContainsPoint(int x, int y)
{
	if (x < 0 || x >= sourceMetaInfo.width || y < 0 || y >= sourceMetaInfo.height)
        return false;
    if (pImg->HasAlpha() && pImg->GetAlpha(x, y)==0)
        return false;
    else
        return true;
}

bool CSource::ContainsTile(STileXY tilexy)
{
    if (tilexy.level != loadLevel)
        return false;
    if ((int)tilexy.x < tilexmin || (int)tilexy.x > tilexmax || (int)tilexy.y < tileymin || (int)tilexy.y > tileymax)
        return false;
    return true;
}

void CSource::CreatePyramidNodes()
{
    // create the nodes, and link the nodes with this source
	STileXY currentTileXY;
	currentTileXY.level = loadLevel;
	for (int x=tilexmin; x<=tilexmax; x++)
	{
		for (int y=tileymin; y<=tileymax; y++)
		{
            // prepare tilexy
			currentTileXY.x = x, currentTileXY.y = y;

            // create node
            pLoader->pPyramid->AddNode(currentTileXY);

            // add this node to source's nodes list
            pyramidNodesList.push_back(currentTileXY);
            ungeneratedTileCnt++;
		}
	}

    pLoader->stat.tileTotalCnt = (int)pLoader->pPyramid->nodes.size();
}

int CSource::DecreaseUngeneratedTileCnt()
{
	int result = 0;
	Lock();
	ungeneratedTileCnt--;
	result = ungeneratedTileCnt;
    Assert(ungeneratedTileCnt>=0, "CSource: ungeneratedTileCnt < 0");
	Unlock();
	return result;
}

void CSource::CreateGenerateRelativeTilesTask()
{
    for (vector<STileXY>::iterator it=pyramidNodesList.begin(); it!=pyramidNodesList.end(); it++)
    {
        CPyramidNode *pNode = pLoader->pPyramid->GetNode(*it);
        if (pNode == NULL)
            LogThrow("CreateGenerateRelativeTilesTask: node no longer exists");
        pNode->CreateGenerateImgFromSourceTask(this);
    }
}

void CSource::CreatePreloadTask()
{
    pLoader->pTaskSetPreload->PushBack(STask(CSource::TaskFunc_Preload, this, NULL));
}

void CSource::TaskFunc_Preload(CSource *pSource, CPyramidNode* pNode)
{
    CLoader *pLoader = pSource->pLoader;
    
    // preload file and add load task
    pSource->Preload();
    pLoader->pTaskSetLoad->PushBack(STask(CSource::TaskFunc_Load, pSource, NULL));
}

void CSource::TaskFunc_Load(CSource *pSource, CPyramidNode* pNode)
{
    // do the load and release file buffer
    pSource->Load();
    pSource->Release();

    // change stat
    pSource->pLoader->stat.sourceInMemoryCnt++;

    // when source is loaded, can start to generate tiles
	pSource->CreateGenerateRelativeTilesTask();
}
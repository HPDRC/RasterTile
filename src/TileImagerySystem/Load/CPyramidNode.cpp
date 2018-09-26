#include "CTaskSet.h"
#include "CLoader.h"
#include "CLoader.h"
#include "CSource.h"
#include "CSourceList.h"
#include "CObjectPool.h"
#include "CPyramid.h"
#include "CPyramidNode.h"
#include "..\Common\CImg.h"
#include "..\Common\CMemory.h"
#include "..\Projection\CProjection.h"
#include "..\..\CxImage\ximage.h"

const U8 CPyramidNode::ALL_FILLED = 0x0F;
const U8 CPyramidNode::COMPLETED = 0xFE;
const int CPyramidNode::FILLED_COUNT[15] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3};

CPyramidNode::CPyramidNode(CLoader *pLoader)
    : CSemaphore(1, 1)
{
    this->pLoader = pLoader;
    this->pImg = NULL;
}

void CPyramidNode::Reset(const STileXY &tilexy, int dependencyCount)
{
    this->tilexy = tilexy;
    this->dependencyCount = dependencyCount;
    this->hasPixelFromSource = false;
    if (this->pImg != NULL)
        LogThrow("CPyramidNode.Reset: pImg is not null!!!");
}

CPyramidNode* CPyramidNode::GetParent()
{
    if (tilexy.level == TOP_TILE_LEVEL)
        return NULL;
    return pLoader->pPyramid->GetNode(tilexy.Father());
}

void CPyramidNode::AllocateMemoryForImg(bool ignoreMemoryLimit)
{
    Lock();
    if (pImg == NULL)
        pImg = (CImg*)pLoader->pPoolTileImage->FetchObject(ignoreMemoryLimit);
    Unlock();
}

void CPyramidNode::DecreaseDependency()
{
    Lock();
    dependencyCount--;
    Assert(dependencyCount>=0, "CPyramidNode.DecreaseDependency: dependencyCount < 0");
    if (dependencyCount != 0)
    {
        Unlock();
        return;
    }

	/*------- when a node is complete ------*/
	// first, convert alpha layer
	ConvertAlphaLayer();

	// check if this node is all black
	bool isEmpty = true;
	if (pImg != NULL)
		isEmpty = pImg->IsEmpty();

	// if has image
	if (!isEmpty)
	{
		// compress image
        bool isFull = pImg->IsFull();
        bool isSuccess = false;
        CxMemFile *pMemFile = (CxMemFile*)pLoader->pPoolMemFile->FetchObject(true);
        if (isFull)
        {
            pImg->pCxImage->SetJpegQuality(JPEG_QUALITY);
            isSuccess = pImg->pCxImage->Encode(pMemFile, CXIMAGE_FORMAT_JPG);
        }
        else
            isSuccess = pImg->pCxImage->Encode(pMemFile, CXIMAGE_FORMAT_PNG);

        if (!isSuccess)
            LogThrow("CPyramidNode: Compress image failed");
        int compressedImageLength = pMemFile->Tell();

        // put into write task list
		STileOutput tileOutput;
		tileOutput.metaInfo.date = pLoader->sourceSetMetaInfo.date;
		tileOutput.metaInfo.originId = pLoader->sourceSetMetaInfo.originId;
		tileOutput.metaInfo.zone = pLoader->sourceSetMetaInfo.outputZone;
		tileOutput.metaInfo.tilexy = tilexy;
		tileOutput.metaInfo.imgLength = compressedImageLength;
        tileOutput.metaInfo.hasAlpha = isFull ? 0 : 1;
        tileOutput.pMemFile = pMemFile;
		pLoader->pTaskSetWrite->PushBack(STask(tileOutput));

		// trigger the father node to generate img
		CPyramidNode *pDad = GetParent();
		if (pDad != NULL)
			pDad->GenerateImgFromSon(this);
	}
	else
	{
		// decrease father node's link count
		CPyramidNode *pDad = GetParent();
		if (pDad != NULL)
			pDad->DecreaseDependency();
		pLoader->IncreaseTileOutputCnt();
	}

	// delete self
    pLoader->pPoolTileImage->ReturnObject(pImg);
    pImg = NULL;
    Unlock();
    pLoader->pPyramid->DeleteNode(tilexy);
}

void CPyramidNode::ConvertAlphaLayer()
{
    if (pImg == NULL)
        return;

    const int x[16] = {0, 0, 1, -1, -1, -1, 1, 1, 0, 0, 2, -2, -2, -2, 2, 2};   // 4 direct neighbors, 8 direct neighbors, 8 secondary neighbors
    const int y[16] = {1, -1, 0, 0, -1, 1, -1, 1, 2, -2, 0, 0, -2, 2, -2, 2};

    // for nodes in lowest level, unfinished pixels are completed by copying from a neighbor pixel
    // for nodes in higher levels, unfinished pixels are completed by calculation
    for (int h=0; h<pImg->height; h++)
    {
        for (int w=0; w<pImg->width; w++)
        {
            U8 *alpha = pImg->GetAlphaPointer(w, h);
            if (*alpha == 0)
                continue;
            if (*alpha == ALL_FILLED)
            {
                *alpha = ALPHA_OPAQUE;
                continue;
            }
            U8 *pixel = pImg->GetPixelPointer(w, h);
            if (!hasPixelFromSource)
            {
                pixel[0] = (U8)(((int)pixel[0]) * 4 / FILLED_COUNT[*alpha]);
                pixel[1] = (U8)(((int)pixel[1]) * 4 / FILLED_COUNT[*alpha]);
                pixel[2] = (U8)(((int)pixel[2]) * 4 / FILLED_COUNT[*alpha]);
                *alpha = ALPHA_OPAQUE;
            }
            else
            {
                bool hasNeighbor = false;
                for (int i=0; i<16; i++)
                {
                    // check if this neighbor is available
                    int w1 = w + x[i], h1 = h + y[i];
                    if (w1<0 || w1>=pImg->width || h1<0 || h1>=pImg->height)
                        continue;

                    // check if this neighbor is complete
                    U8 nAlpha = pImg->GetAlpha(w1, h1);
                    if (nAlpha==ALL_FILLED || nAlpha==ALPHA_OPAQUE)
                    {
                        hasNeighbor = true;
                        memcpy(pixel, pImg->GetPixelPointer(w1, h1), BYTE_PER_PIXEL);
                        break;
                    }
                }
                if (!hasNeighbor)
                {
                    pixel[0] = (U8)(((int)pixel[0]) * 4 / FILLED_COUNT[*alpha]);
                    pixel[1] = (U8)(((int)pixel[1]) * 4 / FILLED_COUNT[*alpha]);
                    pixel[2] = (U8)(((int)pixel[2]) * 4 / FILLED_COUNT[*alpha]);
                }
                *alpha = COMPLETED;
            }
        }
    }

    // change COMPLETED to OPAQUE
    if (hasPixelFromSource)
    {
        for (int h=0; h<pImg->height; h++)
        {
            for (int w=0; w<pImg->width; w++)
            {
                U8 *alpha = pImg->GetAlphaPointer(w, h);
                if (*alpha == COMPLETED)
                    *alpha = ALPHA_OPAQUE;
            }
        }
    }
}

void CPyramidNode::CreateGenerateImgFromSourceTask(CSource* pSource)
{
    pLoader->pTaskSetCalc->PushBack(STask(TaskFunc_GenerateImgFromSource, pSource, this));
}

void CPyramidNode::GenerateImgFromSource(CSource* pSource)
{
    // change status
    hasPixelFromSource = true;

    // see if source covers this area
    if (pSource->ContainsTile(tilexy))
    {
        AllocateMemoryForImg(false);

        Lock();

        // prepare variables
        CImg *pImgTile = pImg, *pImgSource = pSource->pImg;
        int tileWidth = pImgTile->width, tileHeight = pImgTile->height;
        int x[4], y[4], portion[4], portionx1, portionx2, portiony1, portiony2;
        const int baseNum = 1024, baseShift = 20;
        U8 *alphaPixel, *tilePixel;
        int color_r, color_g, color_b;
        int rOffset = pImgSource->R_Offset;
        int gOffset = pImgSource->G_Offset;
        int bOffset = pImgSource->B_Offset;

        // calculate projections
        SDoubleXY *spxy = (SDoubleXY*)pLoader->pPoolDoubleArray->FetchObject(true);
        pLoader->projTile->TileToSourceBulk(*pLoader->projSource, tilexy, pSource->sourceMetaInfo, spxy);

        // start to generate img
        for (int h=0; h<tileHeight; h++)
        {
            int offset = h * tileWidth;
            for (int w=0; w<tileWidth; w++)
            {
                // if this point is already filled, skip
                alphaPixel = pImg->GetAlphaPointer(w,h);
                if (*alphaPixel == ALL_FILLED)
                    continue;

                // the following codes use bilinear interpolation to calc the pixel value of the tile.
                // every pixel is calced by the four surrounding corners.
            
                // calc 4 corners
                x[0] = x[2] = (int)floor(spxy[offset + w].x), x[1] = x[3] = x[0] + 1;
                y[0] = y[1] = (int)floor(spxy[offset + w].y), y[2] = y[3] = y[0] + 1;

                // calc portions
                portionx2 = (int)((spxy[offset + w].x - x[0]) * baseNum);
                portionx1 = baseNum - portionx2;
                portiony2 = (int)((spxy[offset + w].y - y[0]) * baseNum);
                portiony1 = baseNum - portiony2;
                portion[0] = portionx1 * portiony1;
                portion[1] = portionx2 * portiony1;
                portion[2] = portionx1 * portiony2;
                portion[3] = portionx2 * portiony2;

                // calc other stuff
                tilePixel = pImgTile->GetPixelPointer(w, h);

                // check 4 corners
                color_r = color_g = color_b = 0;
                for (int i=0; i<4; i++)
                {
                    // if this corner is already calculated, skip
                    if ((((*alphaPixel)>>i)&1) == 1)
                        continue;

                    // if source doesn't have this corner point, skip
                    if (!pSource->ContainsPoint(x[i], y[i]))
                        continue;
                
                    // calc the pixel
                    U8 *sourcePixel = pSource->pImg->GetPixelPointer(x[i], y[i]);
                    color_r += portion[i] * sourcePixel[rOffset];
                    color_g += portion[i] * sourcePixel[gOffset];
                    color_b += portion[i] * sourcePixel[bOffset];

                    // mark this corner as filled
                    *alphaPixel |= 1 << i;
                }

                // The purpose of first getting sum, and then divide is to keep maximum precision during the calculation.
                // tile image uses CxImage lib which uses BGR byte order
                tilePixel[2] += (color_r >> baseShift) & BYTE_MAX;
                tilePixel[1] += (color_g >> baseShift) & BYTE_MAX;
                tilePixel[0] += (color_b >> baseShift) & BYTE_MAX;
            }
        }

        pLoader->pPoolDoubleArray->ReturnObject(spxy);
        Unlock();
    }

    // after img is generated, try to delete the source, and decrease linkCnt
	if (pSource->DecreaseUngeneratedTileCnt() == 0)
	{
		pLoader->Lock();
		pLoader->stat.sourceInMemoryCnt--;
		pLoader->pSourceList->DeleteSource(pSource);
		pLoader->Unlock();
	}

    DecreaseDependency();
}

void CPyramidNode::GenerateImgFromSon(CPyramidNode *pSon)
{
    // if no son, report error
	if (pSon == NULL)
        LogThrow("CPyramidNode.GenerateImgFromSon: pSon == NULL");

    // generate image
    AllocateMemoryForImg(true);
    int sonIndex = pSon->tilexy.IndexInSons();
    GenerateImgFromSon(pSon, 
        (sonIndex&1)==0?0:pLoader->projTile->tileWidth/2, 
        (sonIndex&2)==0?0:pLoader->projTile->tileHeight/2); // and generate
    
    // decrease link count
    DecreaseDependency();
}

void CPyramidNode::GenerateImgFromSon(CPyramidNode *pSon, int offsetw, int offseth)
{
    CImg *imgSon = pSon->pImg;
    CImg *imgDad = pImg;
    const int x[4] = {0, 1, 0, 1};
    const int y[4] = {0, 0, 1, 1};
    U8 *sonPixel, *dadPixel, *pAlpha;
    
	// in the son's image, 4 pixels in a square is a group
	// for each such group in son
    int tileWidth = pLoader->projTile->tileWidth;
    int tileHeight = pLoader->projTile->tileHeight;
	for (int h=0; h<tileHeight; h+=2)
	{
        int hDad = offseth+(h>>1);
		for (int w=0; w<tileWidth; w+=2)
		{
            int wDad = offsetw+(w>>1);
            dadPixel = imgDad->GetPixelPointer(wDad, hDad);

            pAlpha = imgDad->GetAlphaPointer(wDad, hDad);
            if (*pAlpha == ALL_FILLED)
                continue;

            // for 4 corners, get average of usable son pixel value
            int p1 = 0, p2 = 0, p3 = 0;
            for (int i=0; i<4; i++)
            {
                // if this corner is already calculated, skip
                if ((((*pAlpha)>>i)&1) == 1)
                    continue;

                // if son node's pixel is not available, skip
                if (imgSon->GetAlpha(w+x[i], h+y[i]) == 0)
                    continue;

                // calc the pixel
                sonPixel = imgSon->GetPixelPointer(w+x[i], h+y[i]);
                p1 += sonPixel[0], p2 += sonPixel[1], p3 += sonPixel[2];

                // mark this corner as filled
                *pAlpha |= 1 << i;
            }
            
            dadPixel[0] += (p1+1) >> 2;  // +1 so that .75 is rounded up to 1
            dadPixel[1] += (p2+1) >> 2;
            dadPixel[2] += (p3+1) >> 2;
		}
    }
}

void CPyramidNode::TaskFunc_GenerateImgFromSource(CSource *pSource, CPyramidNode *pNode)
{
    pNode->GenerateImgFromSource(pSource);
}

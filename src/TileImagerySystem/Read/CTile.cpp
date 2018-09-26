#include "CTile.h"
#include "CDBCache.h"
#include "..\FileSystem\CTableQualities.h"
#include "..\FileSystem\CSqlConnection.h"
#include "..\Common\CImg.h"
#include "..\Common\CFile.h"
#include "..\..\CxImage\ximage.h"

CTile::CTile(CDBCache *pCache)
{
    this->pCache = pCache;
    pImageBuffer = NULL, isImageBufferLoaded = false;
    pImageObj = NULL, isImageObjectLoaded = false;
    weight = 0, isWeightLoaded = false;
    gridCompleteness = 0, isGridCompletenessLoaded = false;
    isBufferOutdated = false;
}

CTile::~CTile()
{
    delete[] pImageBuffer;
    delete pImageObj;
}

U8* CTile::GetImageBuffer()
{
    if (isBufferOutdated)
    {
        SaveImageObjectToBuffer();
    }
    else if (!isImageBufferLoaded)
    {
        isImageBufferLoaded = true;

        // get path of the image
        char *path = pCache->GetPath(pathId);
        if (path == NULL)
        {
            Log("CTile: path id %d not found", pathId);
            return NULL;
        }

        // read image buffer from file
        pImageBuffer = new U8[imgLength];
        try
        {
            CFile imageFile(path, false, 2);
            imageFile.SeekStrict(imgOffset, SEEK_SET);
            imageFile.ReadStrict(pImageBuffer, imgLength);
        }
        catch (...)
        {
            delete[] pImageBuffer, pImageBuffer = NULL;
        }
    }
    return pImageBuffer;
}

CImg* CTile::GetImageObject()
{
    if (!isImageObjectLoaded)
    {
        isImageObjectLoaded = true;

        U8* buff = GetImageBuffer();
        if (buff != NULL)
        {
            if (IsBufferPng(buff, imgLength))
                pImageObj = new CImg(buff, imgLength, CXIMAGE_FORMAT_PNG);
            else
                pImageObj = new CImg(buff, imgLength, CXIMAGE_FORMAT_JPG);
            if (pImageObj == NULL || pImageObj->width<=0 || pImageObj->height<=0)   // check if size is valid
                Log("CTile.GetImgObject: wrong size of image"), delete pImageObj, pImageObj = NULL;
        }
    }
    return pImageObj;
}

void CTile::SaveImageObjectToBuffer(int quality)
{
    CImg *pImg = GetImageObject();
    if (pImg != NULL)
    {
        delete[] pImageBuffer, pImageBuffer = NULL, imgLength = 0;
        pImageBuffer = pImg->EncodeAsJpeg(quality, &imgLength);
    }
    isBufferOutdated = false;
}

void CTile::ConvertImageBufferToJpg()
{
    U8 *buffer = GetImageBuffer();
    if (buffer == NULL) return;
    if (IsBufferPng(buffer, imgLength))
        SaveImageObjectToBuffer();
}


int CTile::GetWeight(CSqlConnection& conn)
{
    if (!isWeightLoaded)
    {
        isWeightLoaded = true;
        conn.ResetStmt();
        weight = pCache->GetWeight(originId, tilexy.level);
    }
    return weight;
}

int CTile::CompareQualityTo(CTile *pTile, CSqlConnection& conn)
{
    int thisWeight = GetWeight(conn);
    int thatWeight = pTile->GetWeight(conn);
    if (thisWeight > thatWeight)
        return 1;
    else if (thisWeight < thatWeight)
        return -1;
    else
    {
        if (date > pTile->date)
            return 1;
        else if (date < pTile->date)
            return -1;
        else
            return 0;
    }
}

bool CTile::IsGridComplete(CSqlConnection& conn, int gridx, int gridy)
{
    if (!isGridCompletenessLoaded)  // if there's no record in database, then generate this info and save to database
    {
        gridCompleteness = 0;
        CImg *pImg = GetImageObject();
        if (pImg != NULL)           // if image not found, don't save quality into db, because the files maybe just temporarily inaccessible
        {
            gridCompleteness = pImg->GetGridCompleteness();
            conn.ResetStmt();
            CTableQualities::SetGridCompleteness(conn, id, gridCompleteness);
        }
        isGridCompletenessLoaded = true;
    }
    int offset = gridy * TILE_GRID_X_CNT + gridx;
    return ((gridCompleteness >> offset) & 1) == 1 ? true : false;
}

bool CTile::PatchFromGrid(CTile* pTile, int gridx, int gridy, bool keepNoneBlackPixels)
{
    CImg *imgSrc = pTile->GetImageObject();
    if (imgSrc == NULL) return false;
    CImg *imgDst = this->GetImageObject();
    if (imgDst == NULL) return false;

    // calc some params
    int gridWidth = imgSrc->width / TILE_GRID_X_CNT;
    int gridHeight = imgSrc->height / TILE_GRID_Y_CNT;
    int gridw = gridx * gridWidth;
    int gridh = gridy * gridHeight;
    
    // begin to patch
    isBufferOutdated = true;
    if (!keepNoneBlackPixels)
    {
        // copy pixel
        int rowWidth = gridWidth * BYTE_PER_PIXEL;
        for (int h=gridh; h<gridh+gridHeight; h++)
            memcpy(imgDst->GetPixelPointer(gridw,h), imgSrc->GetPixelPointer(gridw, h), rowWidth);

        // set alpha
        imgDst->SetAlpha(ALPHA_OPAQUE, gridw, gridh, gridWidth, gridHeight);
        return true;
    }
    else
    {
        bool isGridComplete = true;
        if (!imgDst->HasAlpha())
        {
            imgDst->CreateAlpha();
            imgDst->ClearAlpha(ALPHA_OPAQUE);
        }
        for (int h=gridh; h<gridh+gridHeight; h++)
        {
            int wOffset = h * imgDst->width;
            for (int w=gridw; w<gridw+gridWidth; w++)
            {
                if (imgDst->GetAlpha(w, h) == ALPHA_OPAQUE)
                    continue;
                if (imgSrc->HasAlpha() && imgSrc->GetAlpha(w,h) != ALPHA_OPAQUE)
                    isGridComplete = false;
                else
                {
                    *imgDst->GetAlphaPointer(w, h) = ALPHA_OPAQUE;
                    memcpy(imgDst->GetPixelPointer(w, h), imgSrc->GetPixelPointer(w,h), BYTE_PER_PIXEL);
                }
            }
        }
        return isGridComplete;
    }
}

bool CTile::IsBufferPng(U8 *buffer, int bufferLength)
{
    return bufferLength > 4 && buffer[0]==0x89 && buffer[1]==0x50 && buffer[2]==0x4E && buffer[3]==0x47 ? true : false;
}
#pragma once
#include "includes.h"

class CTile
{
private:
    // image buffer of this tile, only loaded when first used
    U8* pImageBuffer;
    bool isImageBufferLoaded;

    // image object of this tile, only loaded when first used
    CImg* pImageObj;
    bool isImageObjectLoaded;
    
    // weight of this tile, only loaded when first used
    int weight;
    bool isWeightLoaded;

    // if image is modified
    bool isBufferOutdated;

public:
    // record each grid is complete or not, only loaded when first used
    int gridCompleteness;
    bool isGridCompletenessLoaded;

public:
    S64 id;
    int pathId;
    int imgOffset;
    int imgLength;
    int date;
    int originId;
    STileXY tilexy;
    CDBCache *pCache;

public:
    CTile(CDBCache *pCache);
    ~CTile();

    // get image buffer. return NULL if not found.
    U8* GetImageBuffer();

    // get img object. return NULL if not found
    CImg* GetImageObject(); 

    // after modifying image object, you may want to recompress the object back to buffer
    void SaveImageObjectToBuffer(int quality = JPEG_QUALITY);

    // when imageBuffer is a png compressed binary string, convert it to jpg
    void ConvertImageBufferToJpg();

    // get weight of this tile from db
    int GetWeight(CSqlConnection& conn);

    // compare quality to another tile, currently, the algorightm is: first compare weight, then compare date.
    int CompareQualityTo(CTile *pTile, CSqlConnection& conn);

    // check if a grid is complete
    bool IsGridComplete(CSqlConnection& conn, int gridx, int gridy);

    // patch from another tile's grid, return if grid is complete after patch
    bool PatchFromGrid(CTile* pTile, int gridx, int gridy, bool keepNoneBlackPixels);

    // judge if a buffer contains png image
    bool IsBufferPng(U8 *buffer, int bufferLength);
};
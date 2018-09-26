#include "CReader.h"
#include "CTile.h"
#include "CDBCache.h"
#include "..\Common\CImg.h"
#include "..\FileSystem\CTableTiles.h"
#include "..\FileSystem\CSqlConnection.h"

const int CReader::maxMultiTileArea = 20000 * 20000; // img cannot exceed 2GB (CxImage library limit), which means no more than 600M pixels

CReader::CReader(int tileWidth, int tileHeight, char *dbConnectionString, char *originName, int zone, int date, int jpegQuality, bool enableLog)
    : CLoggable(enableLog)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    this->dbConnectionString = dbConnectionString;
    this->originName = originName;
    this->zone = zone;
    this->date = date;
    this->jpegQuality = jpegQuality;

    this->resultArray = NULL;
    this->resultArrayLength = 0;
    this->pConn = new CSqlConnection();

    pConn->Open(dbConnectionString);
    pCache = CDBCache::GetDBCache(dbConnectionString, pConn);
}

CReader::~CReader()
{
    delete resultArray, resultArray = NULL;
    resultArrayLength = 0;
    for (int i=0; i<tileList.size(); i++)
        delete tileList[i];
    delete pConn;
}

CImg* CReader::GetMosaic(int x1, int y1, int x2, int y2, U8 level)
{
    // ensure x1 is left, x2 is right
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    
    WriteLog("########## CReader: GetMosaic at (x1,y1,x2,y2,level,zone): %d,%d,%d,%d,%d,%d ##########", x1, y1, x2, y2, (int)level, zone);

    // exit if too many pixels
    double area = (x2-x1+1.0)*(y2-y1+1.0);
    if (area > maxMultiTileArea)
    {
        Log("CReader.GetMultiTile: target tile's area %.0lf > %d", area, maxMultiTileArea);
        return NULL;
    }
    
    // find xn yn
    int xn1 = x1 / tileWidth;
    int yn1 = y1 / tileHeight;
    int xn2 = x2 / tileWidth;
    int yn2 = y2 / tileHeight;

    WriteLog("Number of tiles: %d", (xn2-xn1+1)*(yn2-yn1+1));

    // create final tile
    CImg *finalImg = new CImg(x2-x1+1, y2-y1+1);
    finalImg->Clear();   // initialize image as black

    // paste all tiles onto this tile
    STileXY tilexy;
    tilexy.level = level;
    for (int x=xn1; x<=xn2; x++)
    {
        for (int y=yn1; y<=yn2; y++)
        {
            tilexy.x = x, tilexy.y = y;
            CTile *tile = GetTile(tilexy);
            if (tile == NULL) continue;
            CImg *pImgSrc = tile->GetImageObject();
            if (pImgSrc == NULL) continue;
            pImgSrc->PasteTo(finalImg, x*tileWidth - x1, y*tileHeight - y1);
        }
    }

    return finalImg;
}

bool CReader::GetTileExist(STileXY& tilexy)
{
    try
    {
        // clear previous result
        for (int i=0; i<tileList.size(); i++)
            delete tileList[i];
        tileList.clear();

        // fetch all tiles' meta info
        WriteLog("########## IsSingleTileExist at (x,y,level,zone): %d,%d,%d,%d ##########", tilexy.x, tilexy.y, (int)tilexy.level, zone);
        CTableTiles::GetTiles(*pConn, tilexy, zone, pCache->GetOriginId(originName), date, &tileList, pCache);
        WriteLog("found %d tiles", tileList.size());
        return (tileList.size() == 0) ? false : true;
    }
    catch (char * msg)
    {
        Log("CReader.IsSingleTileExist: exception when requesting %d,%d,%d. Detail: %s", tilexy.x, tilexy.y, (int)tilexy.level, msg);
    }
    catch (...)
    {
        Log("CReader.IsSingleTileExist: unkown exception when requesting %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    }
    return false;
}

CTile* CReader::GetTile(STileXY& tilexy)
{
    try
    {
        // clear previous result
        for (int i=0; i<tileList.size(); i++)
            delete tileList[i];
        tileList.clear();

        // fetch all tiles' meta info
        WriteLog("########## GetSingleTile at (x,y,level,zone): %d,%d,%d,%d ##########", tilexy.x, tilexy.y, (int)tilexy.level, zone);
        CTableTiles::GetTiles(*pConn, tilexy, zone, pCache->GetOriginId(originName), date, &tileList, pCache);
        WriteLog("found %d tiles", tileList.size());
        if (isLogEnabled)
        {
            for (int i=0; i<tileList.size(); i++)
            {
                char *path = pCache->GetPath(tileList[i]->pathId);
                WriteLog("Tile #%d: offset=%d, length=%d, %spath=%s, quality loaded:%d", i, tileList[i]->imgOffset, tileList[i]->imgLength, tileList[i]->GetImageBuffer() == NULL ? "file not found, " : "", path == NULL ? "NULL" : path, tileList[i]->isGridCompletenessLoaded ? 1 : 0);
            }
        }

        // return NULL if no tile is found
        if (tileList.size() == 0) return NULL;

        // if there's only one tile, return this tile directly
        if (tileList.size() == 1) return tileList[0];
    
        // if there're multiple tiles, merge all tiles(which are incomplete), grid by grid, highest quality goes first
        // first, sort by quality
        SortTilesByQuality(tileList);

        if (isLogEnabled)
        {
            WriteLog("--- Sort tiles by quality from high to low ---");
            for (int i=0; i<tileList.size(); i++)
            {
                char *path = pCache->GetPath(tileList[i]->pathId);
                WriteLog("Tile #%d: offset=%d, length=%d, %spath=%s", i, tileList[i]->imgOffset, tileList[i]->imgLength, tileList[i]->GetImageBuffer() == NULL ? "file not found, " : "", path == NULL ? "NULL" : path);
            }
        }

        // and find the highest quality tile which is available
        CTile* finalTile = NULL;
        int tileStartIndex = 0;
        for (; tileStartIndex<tileList.size(); tileStartIndex++)
        {
            finalTile = tileList[tileStartIndex];
            if (finalTile->GetImageBuffer() != NULL)
                break;
        }
        
        WriteLog("--- Tile #%d is used as base tile ---", tileStartIndex);

        // return null if no tile is available
        if (tileStartIndex == tileList.size())
            return NULL;
        tileStartIndex++;

        // perform grid based patching strategy
        for (int i=0; i<TILE_GRID_X_CNT; i++)
        {
            for (int j=0; j<TILE_GRID_Y_CNT; j++)
            {
                // if final tile's this grid is complete, skip
                if (finalTile->IsGridComplete(*pConn, i, j))
                    continue;

                WriteLog("Grid(%d,%d) is patched from all tiles", i, j);
                for (int k=tileStartIndex; k<tileList.size(); k++)
                    if (finalTile->PatchFromGrid(tileList[k], i, j, true))
                        break;
            }
        }

        return finalTile;
    }
    catch (char * msg)
    {
        Log("CReader.GetSingleTile: exception when requesting %d,%d,%d. Detail: %s", tilexy.x, tilexy.y, (int)tilexy.level, msg);
    }
    catch (...)
    {
        Log("CReader.GetSingleTile: unkown exception when requesting %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    }
    return NULL;
}

void CReader::SortTilesByQuality(vector<CTile*> &tileList)
{
    int length = (int)tileList.size();
    CTile* tmp = NULL;
    for (int i=0; i<length; i++)
        for (int j=i+1; j<length; j++)
            if (tileList[j]->CompareQualityTo(tileList[i], *pConn) > 0)
                tmp = tileList[j], tileList[j] = tileList[i], tileList[i] = tmp;
}

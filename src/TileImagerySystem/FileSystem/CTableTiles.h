#pragma once
#include "includes.h"

class CTableTiles
{
public:
    // prepare to add tile, only need to be called once if no other statment is executed
    static void AddTilePrepare(CSqlConnection& conn);

    // add tile
    static void AddTile(CSqlConnection& conn, int pathId, STileMetaInfo *pTileMetaInfo);

    // add multiple tiles at one time
    static void AddTiles(CSqlConnection& conn, int pathId, STileMetaInfo tileMetaInfos[], int start, int end);

    // get tiles at specified xn yn, if want to find all origins, pass originId = 0
    static void GetTiles(CSqlConnection& conn, STileXY& tilexy, int zone, int originId, int date, vector<CTile*>* pTileList, CDBCache *pCache);
    
    // get different origins at specified tile position
    static void GetOrigins(CSqlConnection& conn, STileXY& tilexy, int zone, vector<SOriginAndDate>* pOriginList);

    // get list of origins and dates for a range of tiles
    static void GetOrigins(CSqlConnection& conn, U32 x1, U32 y1, U32 x2, U32 y2, U8 level, int zone, vector<SOriginAndDate> &originList);
};

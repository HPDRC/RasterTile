#pragma once
#include "includes.h"
#include "..\Common\CLoggable.h"

class CReader : public CLoggable
{
protected:
    CSqlConnection *pConn;

    vector<CTile*> tileList;

    void SortTilesByQuality(vector<CTile*> &tileList);

    CDBCache *pCache;

protected:
    static const int maxMultiTileArea;

    int tileWidth, tileHeight, date, jpegQuality;

    int zone;

    char *dbConnectionString, *originName;

    // returned CImg object must be deleted
    CImg* GetMosaic(int x1, int y1, int x2, int y2, U8 level);

public:
    // store result of last read
    int resultArrayLength;
    U8 *resultArray;

public:
    CReader(int tileWidth, int tileHeight, char *dbConnectionString, char *originName, int zone, int date, int jpegQuality, bool enableLog);
    ~CReader();

    // get a tile
    CTile* GetTile(STileXY& tilexy);

    // check a tile's existence
    bool GetTileExist(STileXY& tilexy);
};

/*
How to generate a best available tile:
    1. first sort tiles by their <tileQuality>
    2. use the best available tile as base tile (final tile)
    3. for each small grid, patch other grids into the base tile
        1. if base tile's current grid is complete, skip
        2. use the same grid from worse quality tiles until the grid is complete.

What is a grid:
    For example, divide a tile by 16*16, each small part is a grid

How to evaluate <tileQuality>:
    Principles:
        1. first compare weight, higher weight tile always has higher quality
        2. second compare date, newer tile always has higher quality

*/

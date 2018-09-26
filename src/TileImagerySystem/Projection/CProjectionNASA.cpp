#include "CProjectionNASA.h"

const int CProjectionNASA::TILE_WIDTH = 512;
const int CProjectionNASA::TILE_HEIGHT = 512;
const int CProjectionNASA::TOTAL_LEVEL_CNT = 23;

const S64 CProjectionNASA::mapWidthArray[TOTAL_LEVEL_CNT] = 
{
    1<<9,   // level 0
    1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15, 1<<16, 1<<17, 1<<18, 1<<19, // level 1 to 10
    1<<20, 1<<21, 1<<22, 1<<23, 1<<24, 1<<25, 1<<26, 1<<27, 1<<28, 1<<29, //level 11 to 20
    1<<30, (1i64)<<31 //level 21 to 22
};

CProjectionNASA::CProjectionNASA()
    : CProjection(TILE_WIDTH, TILE_HEIGHT, TOTAL_LEVEL_CNT)
{
}

void CProjectionNASA::SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const
{
    SDoubleXY xy = source.GetXY();
    latlon.lon = xy.x * M_PI / 180;
    latlon.lat = xy.y * M_PI / 180;
}

void CProjectionNASA::LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const
{
    SDoubleXY xy;
    xy.x = latlon.lon * 180 / M_PI;
    xy.y = latlon.lat * 180 / M_PI;
    source.SetXY(xy);
}
	
void CProjectionNASA::TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const
{
    SIntXY xy = tile.GetXY(tileWidth, tileHeight);

    // get x, y on (1, 0.5) sized map
    double x = (double)xy.x / mapWidthArray[tile.tilexy.level];
    double y = (double)xy.y / mapWidthArray[tile.tilexy.level];

	// calc latlon
    latlon.lon = (x - 0.5) * 2 * M_PI;
    latlon.lat = (0.25 - y) * 2 * M_PI;
}

void CProjectionNASA::LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const
{
    // transform lat lon to offset in (1, 0.5) map
    double x = latlon.lon / M_PI / 2 + 0.5;
    double y = 0.25 - latlon.lat / M_PI / 2;

    // transfrom from 1, 0.5 map to current level map
    U32 pixelx = (U32)(x * mapWidthArray[level]);
    U32 pixely = (U32)(y * mapWidthArray[level]);

    tile.SetXY(pixelx, pixely, level, tileWidth, tileHeight);
}

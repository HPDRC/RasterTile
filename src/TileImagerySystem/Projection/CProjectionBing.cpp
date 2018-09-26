#include "CProjectionBing.h"

#define MAP_SIZE_IN_METER (20037508.34*2)

const int CProjectionBing::TILE_WIDTH = 256;
const int CProjectionBing::TILE_HEIGHT = 256;
const int CProjectionBing::TOTAL_LEVEL_CNT = 24;

const S64 CProjectionBing::mapSizeArray[TOTAL_LEVEL_CNT] = 
{
    1<<8,   // level 0
    1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15, 1<<16, 1<<17, 1<<18, // level 1 to 10
    1<<19, 1<<20, 1<<21, 1<<22, 1<<23, 1<<24, 1<<25, 1<<26, 1<<27, 1<<28, //level 11 to 20
    1<<29, 1<<30, (1i64)<<31 //level 21 to 23
};

const double CProjectionBing::mapLatlonResolutionArray[TOTAL_LEVEL_CNT] = 
{
    2*M_PI / (1<<8),   // level 0
    2*M_PI / (1<<9), 2*M_PI / (1<<10), 2*M_PI / (1<<11), 2*M_PI / (1<<12), 2*M_PI / (1<<13), 2*M_PI / (1<<14), 2*M_PI / (1<<15), 2*M_PI / (1<<16), 2*M_PI / (1<<17), 2*M_PI / (1<<18), // level 1 to 10
    2*M_PI / (1<<19), 2*M_PI / (1<<20), 2*M_PI / (1<<21), 2*M_PI / (1<<22), 2*M_PI / (1<<23), 2*M_PI / (1<<24), 2*M_PI / (1<<25), 2*M_PI / (1<<26), 2*M_PI / (1<<27), 2*M_PI / (1<<28), //level 11 to 20
    2*M_PI / (1<<29), 2*M_PI / (1<<30), 2*M_PI / ((1i64)<<31) //level 21 to 23
};
const double CProjectionBing::mapMeterResolutionArray[TOTAL_LEVEL_CNT] = 
{
    MAP_SIZE_IN_METER / (1<<8),   // level 0
    MAP_SIZE_IN_METER / (1<<9), MAP_SIZE_IN_METER / (1<<10), MAP_SIZE_IN_METER / (1<<11), MAP_SIZE_IN_METER / (1<<12), MAP_SIZE_IN_METER / (1<<13), MAP_SIZE_IN_METER / (1<<14), MAP_SIZE_IN_METER / (1<<15), MAP_SIZE_IN_METER / (1<<16), MAP_SIZE_IN_METER / (1<<17), MAP_SIZE_IN_METER / (1<<18), // level 1 to 10
    MAP_SIZE_IN_METER / (1<<19), MAP_SIZE_IN_METER / (1<<20), MAP_SIZE_IN_METER / (1<<21), MAP_SIZE_IN_METER / (1<<22), MAP_SIZE_IN_METER / (1<<23), MAP_SIZE_IN_METER / (1<<24), MAP_SIZE_IN_METER / (1<<25), MAP_SIZE_IN_METER / (1<<26), MAP_SIZE_IN_METER / (1<<27), MAP_SIZE_IN_METER / (1<<28), //level 11 to 20
    MAP_SIZE_IN_METER / (1<<29), MAP_SIZE_IN_METER / (1<<30), MAP_SIZE_IN_METER / ((1i64)<<31) //level 21 to 23
};


CProjectionBing::CProjectionBing()
    : CProjection(TILE_WIDTH, TILE_HEIGHT, TOTAL_LEVEL_CNT)
{
}

void CProjectionBing::SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const
{
    LogThrow("CProjectionBing::SourceToLatLon: Not implemented");
}

void CProjectionBing::LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const
{
    LogThrow("CProjectionBing::LatLonToSource: Not implemented");
}
	
void CProjectionBing::TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const
{
    // get pixel x, y on current level
    SIntXY xy = tile.GetXY(tileWidth, tileHeight);

    // get x, y on (1, 1) sized map
    double x = (double)xy.x / mapSizeArray[tile.tilexy.level] - 0.5;
    double y = 0.5 - (double)xy.y / mapSizeArray[tile.tilexy.level];

	// calc latlon
    latlon.lat = M_PI/2 - atan(exp(-y*2*M_PI))*2;
    latlon.lon = x * 2 * M_PI;
}

void CProjectionBing::LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const
{
    double x = latlon.lon / M_PI / 2 + 0.5;
    double sinLat = sin(latlon.lat);
	double y = 0;
    if (sinLat == 1)
		y = 0;
	else if (sinLat == -1)
		y = 1;
	else
		y = 0.5 - log((1 + sinLat) / (1 - sinLat)) / (4 * M_PI);

	// clip y to (0,1)
	if (y < 0) y = 0;
	if (y >= 1) y = 0.9999999999999;

    // transform from 1, 1 map to current level map
    int pixelx = (int)(x * mapSizeArray[level] + 0.5);
    int pixely = (int)(y * mapSizeArray[level] + 0.5);

    // calc offsets
    tile.SetXY(pixelx, pixely, level, tileWidth, tileHeight);
}

void CProjectionBing::MeterToTile(const SDoubleXY &xy, STilePixelXY &tile, U8 level)
{
    double x = xy.x / MAP_SIZE_IN_METER + 0.5;
    double y = 0.5 - xy.y / MAP_SIZE_IN_METER;

    // transfrom from 1, 1 map to current level map
    int pixelx = (int)(x * mapSizeArray[level] + 0.5);
    int pixely = (int)(y * mapSizeArray[level] + 0.5);

    // calc offsets
    tile.SetXY(pixelx, pixely, level, tileWidth, tileHeight);
}


int CProjectionBing::LatlonResolutionToLevel(double radianLonPerPixel)
{
    int level;
    radianLonPerPixel *= 1.05;
    for (level=0; level<TOTAL_LEVEL_CNT; level++)
        if (radianLonPerPixel >= mapLatlonResolutionArray[level])
            break;
    return level;
}

int CProjectionBing::MeterResolutionToLevel(double xMeterPerPixel)
{
    int level;
    xMeterPerPixel *= 1.05;
    for (level=0; level<TOTAL_LEVEL_CNT; level++)
        if (xMeterPerPixel >= mapMeterResolutionArray[level])
            break;
    return level;
}

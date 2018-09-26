#include "CProjectionUtm.h"

const int CProjectionUtm::TILE_WIDTH = 400;
const int CProjectionUtm::TILE_HEIGHT = 400;
const int CProjectionUtm::TOTAL_LEVEL_CNT = 31;
const int CProjectionUtm::ZONE_CNT = 60;
const int CProjectionUtm::MAX_NORTHING = 13107200;

const double CProjectionUtm::mapResArray[TOTAL_LEVEL_CNT] = 
{
    1<<20, // level 0
    1<<19, 1<<18, 1<<17, 1<<16, 1<<15, 1<<14, 1<<13, 1<<12, 1<<11, 1<<10,   // level 1 to 10
    1<<9, 1<<8, 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1<<0,             // level 11 to 20
    1.0/(1<<1), 1.0/(1<<2), 1.0/(1<<3), 1.0/(1<<4), 1.0/(1<<5), 1.0/(1<<6), 1.0/(1<<7), 1.0/(1<<8), 1.0/(1<<9), 1.0/(1<<10), // level 21 to 30
};

CProjectionUtm::CProjectionUtm()
    : CProjection(TILE_WIDTH, TILE_HEIGHT, TOTAL_LEVEL_CNT)
{
    // init pjLatLong
    pjLatlon = pj_init_plus("+proj=latlong +datum=NAD83");

    // init pjUtmArray
    char initString[128];
    pjUtmArray = new projPJ[ZONE_CNT + 1 + ZONE_CNT];
    for (int zone=-ZONE_CNT; zone<=ZONE_CNT; zone++)
    {
        if (zone == 0)
		{
			pjUtmArray[zone+ZONE_CNT] = NULL;
            continue;
		}
        else if (zone < 0)
            sprintf(initString, "+proj=utm +zone=%d +south +datum=NAD83", -zone);
        else
            sprintf(initString, "+proj=utm +zone=%d +datum=NAD83", zone);
        pjUtmArray[zone+ZONE_CNT] = pj_init_plus(initString);
    }
}

CProjectionUtm::~CProjectionUtm()
{
    for (int i=0; i<ZONE_CNT + 1 + ZONE_CNT; i++)
    {
        pj_free(pjUtmArray[i]);
    }
    delete[] pjUtmArray;
    pj_free(pjLatlon);
}


void CProjectionUtm::SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const
{
    SDoubleXY xy = source.GetXY();
    latlon.lon = xy.x;
    latlon.lat = xy.y;

    // ensure zone != 0
    if (source.sourceMetaInfo.zone == 0)
        LogThrow("CProjectionUtm.XYToLatLon: error: zone==0");

	// transform utm to lat lon
    if (pj_transform(pjUtmArray[source.sourceMetaInfo.zone+ZONE_CNT], pjLatlon, 1, 0, &latlon.lon, &latlon.lat, NULL) != 0)
        LogThrow("CProjectionUtm.XYToLatLon: transform error: %lf, %lf, %d", latlon.lat, latlon.lon, (int)source.sourceMetaInfo.zone);
}

void CProjectionUtm::LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const
{
    SDoubleXY xy;
    xy.x = latlon.lon, xy.y = latlon.lat;

    // transform
    int result = pj_transform(pjLatlon, pjUtmArray[source.sourceMetaInfo.zone+ZONE_CNT], 1, 0, &xy.x, &xy.y, NULL);
    if (result != 0)
			LogThrow("SourceUTM: SSourceXY.SetLatLon: transform error: %s!", pj_strerrno(result));

    source.SetXY(xy);
}
	
void CProjectionUtm::TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const
{
    LogThrow("CProjectionUtm::TileToLatLon: this function should never be called");
}

void CProjectionUtm::LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const
{
    LogThrow("CProjectionUtm::LatLonToTile: this function should never be called");
}

void CProjectionUtm::SourceToTile(const CProjection &sourceProjection, const SSourcePixelXY &source, STilePixelXY &tile, U8 level) const
{
    // calculate new offsets
	SDoubleXY utmxy = source.GetXY();
    double pixelx = utmxy.x / mapResArray[level];
    double pixely = (MAX_NORTHING - utmxy.y) / mapResArray[level];
    tile.SetXY((int)pixelx, (int)pixely, level, tileWidth, tileHeight);
}

void CProjectionUtm::TileToSource(const CProjection &sourceProjection, const STilePixelXY &tile, SSourcePixelXY &source) const
{
    SIntXY pixelxy = tile.GetXY(tileWidth, tileHeight);
    SDoubleXY sourcexy;
    sourcexy.x = pixelxy.x * mapResArray[tile.tilexy.level];
    sourcexy.y = MAX_NORTHING - pixelxy.y * mapResArray[tile.tilexy.level];
    source.SetXY(sourcexy);
}

void CProjectionUtm::TileToSourceBulk(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, SDoubleXY xy[]) const
{
    TileToSourceBulk_NotOptimized(sourceProjection, tilexy, sourceMetaInfo, xy);
}

int CProjectionUtm::SourceToProperLevel(const CProjection &sourceProjection, const SSourcePixelXY &source) const
{
    int level;
    for (level=0; level<totalLevelCnt-1; level++)
        if (source.sourceMetaInfo.xScale >= mapResArray[level])
            break;
    return level;
}

int CProjectionUtm::ResolutionToLevel(double meterPerPixel)
{
    int level;
    for (level=0; level<TOTAL_LEVEL_CNT; level++)
        if (meterPerPixel >= mapResArray[level])
            break;
    return level;
}

STilePixelXY CProjectionUtm::UtmXyToTilePixel(double utmx, double utmy, U8 level)
{
    STilePixelXY tpxy;
    double pixelx = utmx / mapResArray[level];
    double pixely = (MAX_NORTHING - utmy) / mapResArray[level];
    tpxy.SetXY((int)pixelx, (int)pixely, level, tileWidth, tileHeight);
    return tpxy;
}

#pragma once
#include "includes.h"
#include "..\..\Proj4\proj_api.h"

// base class of all projections
class CProjection
{
private:
    // check if bulk projection transformation can be optimized: for four corners in the tile, a[1] = (a[0]+a[2])/2 (optimizeLength == 2)
    bool TileToSourceBulk_IsOptimizable(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, int optimizeLength) const;

protected:
    void TileToSourceBulk_NotOptimized(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, SDoubleXY xy[]) const;

public:
    int tileWidth, tileHeight, tilePixelCnt, totalLevelCnt;

public:
    CProjection(int tileWidth, int tileHeight, int totalLevelCnt);

	virtual void SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const = 0;

	virtual void LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const = 0;
	
    virtual void TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const = 0;

    virtual void LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const = 0;

    // convert a source point at a source projection to a tile point at this projection on specific level
    virtual void SourceToTile(const CProjection &sourceProjection, const SSourcePixelXY &source, STilePixelXY &tile, U8 level) const;

    // convert a tile point at this projection to a source point at a source projection
    virtual void TileToSource(const CProjection &sourceProjection, const STilePixelXY &tile, SSourcePixelXY &source) const;

    // bulk convert tile points to source points
    virtual void TileToSourceBulk(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, SDoubleXY xy[]) const;

    virtual int SourceToProperLevel(const CProjection &sourceProjection, const SSourcePixelXY &source) const;

    // convert radian format longitude to utm zone
    static int LongitudeToUtmZone(double longitude);

    // convert a radian format latlon rectangle to UTM, enlarge to smallest possible rectangle, and convert back to latlon
    static void LatLonRectangleExpandByUtm(int zone, const SRectF &rectOrg, SLatLon &tl, SLatLon &tr, SLatLon &bl, SLatLon &br);
};

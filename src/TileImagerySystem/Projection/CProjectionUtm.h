#pragma once
#include "CProjection.h"

// base class of all projections
class CProjectionUtm : public CProjection
{
public:
    static const int TILE_WIDTH;
    static const int TILE_HEIGHT;
    static const int TOTAL_LEVEL_CNT;
    static const int ZONE_CNT;
    static const int MAX_NORTHING;

    static const double mapResArray[];

private:
    projPJ *pjUtmArray, pjLatlon;
    
public:
    CProjectionUtm();
    virtual ~CProjectionUtm();

    virtual void SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const;

	virtual void LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const;

    virtual void TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const;

    virtual void LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const;

    virtual void SourceToTile(const CProjection &sourceProjection, const SSourcePixelXY &source, STilePixelXY &tile, U8 level) const;

    virtual void TileToSource(const CProjection &sourceProjection, const STilePixelXY &tile, SSourcePixelXY &source) const;

    virtual void TileToSourceBulk(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, SDoubleXY xy[]) const;

    virtual int SourceToProperLevel(const CProjection &sourceProjection, const SSourcePixelXY &source) const;

    //////////////// own functions //////////////////

    STilePixelXY UtmXyToTilePixel(double utmx, double utmy, U8 level);

    int ResolutionToLevel(double meterPerPixel);
};

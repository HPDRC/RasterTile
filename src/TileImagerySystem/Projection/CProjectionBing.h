#pragma once
#include "CProjection.h"

// base class of all projections
class CProjectionBing : public CProjection
{
public:
    static const int TILE_WIDTH;
    static const int TILE_HEIGHT;
    static const int TOTAL_LEVEL_CNT;
    static const S64 mapSizeArray[];
    static const double mapLatlonResolutionArray[];
    static const double mapMeterResolutionArray[];

public:
    CProjectionBing();

    virtual void SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const;

	virtual void LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const;

    virtual void TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const;

    virtual void LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const;

    void MeterToTile(const SDoubleXY &xy, STilePixelXY &tile, U8 level);

    int LatlonResolutionToLevel(double radianLonPerPixel);

    int MeterResolutionToLevel(double xMeterPerPixel);
};

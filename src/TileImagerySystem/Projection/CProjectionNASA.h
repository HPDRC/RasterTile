#pragma once
#include "CProjection.h"

// base class of all projections
class CProjectionNASA : public CProjection
{
public:
    static const int TILE_WIDTH;
    static const int TILE_HEIGHT;
    static const int TOTAL_LEVEL_CNT;

    static S64 const mapWidthArray[];

public:
    CProjectionNASA();

    virtual void SourceToLatLon(const SSourcePixelXY &source, SLatLon &latlon) const;

	virtual void LatLonToSource(const SLatLon &latlon, SSourcePixelXY &source) const;

    virtual void TileToLatLon(const STilePixelXY &tile, SLatLon &latlon) const;

    virtual void LatLonToTile(const SLatLon &latlon, STilePixelXY &tile, U8 level) const;
};

/*
	In NASA projection, SSourceMetaInfo structure has the following meaning:
	struct SSourceMetaInfo
	{
		x, y;	// image's upper left corner's coordinate in latlon (degree mode)
		xScale;	// resolution of image, unit is degree/pixel
		yScale;	// -xScale
	}
*/
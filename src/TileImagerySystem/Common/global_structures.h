#pragma once
#include "global_defines.h"

// a tile's coordinates
typedef struct STILEXY
{
    U32 x, y;
    U8 level;

    // define the operator <
    bool operator<(const STILEXY &tilexy) const
    {
        if (x < tilexy.x) return true;
        else if (x > tilexy.x) return false;
        if (y < tilexy.y) return true;
        else if (y > tilexy.y) return false;
        return level < tilexy.level;
    }

    bool operator==(const STILEXY &tilexy) const
    {
        return (x==tilexy.x && y==tilexy.y && level==tilexy.level) ? true : false;
    }

    // Figure out in its father's four sons, this coord is of which index
    inline int IndexInSons() const
    {
        return (y%2)*2 + (x%2);
    }

    // get this coord's father coord
    inline STILEXY Father() const
    {
        STileXY result;
        result.x = x >> 1;
        result.y = y >> 1;
        result.level = level - 1;
        return result;
    }
}STileXY;

// define a date
struct SDate
{
    U16 year;
    U8  month;
    U8  day;

    int ToInt()
    {
        return year * 10000 + month * 100 + day;
    }
};

// to be compatible with utm system, a tile's meta info has "zone" attribute
// for non-utm systems, "zone" will be ignored
struct STileMetaInfo
{
    U32 imgOffset; // offset of the img in the corresponding dat file
    U32 imgLength;	// length of the img in the corresponding dat file
    SDate date;     // date
    int originId;   // belongs to which origin
    int zone;        // zone
    STileXY tilexy; // coordinates
    U8 hasAlpha;    // if has alpha layer
};

struct STileOutput
{
	STileMetaInfo metaInfo;
	void *pMemFile;
};

struct SLoaderStat
{
    int unfinishedPreloadTaskCnt;
    int unfinishedLoadTaskCnt;
    int unfinishedCalcTaskCnt;
    int unfinishedWriteTaskCnt;
    int finishedPreloadTaskCnt;
    int finishedLoadTaskCnt;
    int finishedCalcTaskCnt;
    int finishedWriteTaskCnt;

    int sourceInMemoryCnt;
    int tileInMemoryCnt;
    int tileOutputedCnt;
    int tileTotalCnt;
    int runningThreadCnt;

    int memoryUsedBySource;
    int memoryUsedByTile;
    int memoryUsedByTileMax;
    
    int threadCntWaitingForSourceMemory;
    int threadCntWaitingForTileMemory;

    int tileMemoryEstimate;
    int tileMemoryLimit;
};

// define the meta info of a sourceset
struct SSourceSetMetaInfo
{
    SDate date;     // one sourceset should have the same date
    int originId;   // one sourceset should belong to one origin, and one origin has one id
    int outputZone; // decide generated tiles' zone value
};

enum ESourceFileType
{
	unknown = 0,
	doqq = 1,
	doqqc = 2,
	tif = 3,
	png = 4,
	jpg = 5,
};

// A universal description of a source image's location, resolution and dimension
// Its members(x, y, xScale and yScale) have different meaning under different projections
struct SSourceMetaInfo
{
	ESourceFileType fileType;			// type of this source file (tif, doqq, png, jpg, etc)
    double x;				// x coordinate of of this image
	double y;				// y coordinate of of this image
    double xScale;			// x resolution
    double yScale;			// y resolution
    int width;				// width of the image (how many pixels)
    int height;				// height of the image (how many pixels)
    int zone;				// Under UTM, its value range is [-60,0) U (0,60]. Useless in other projections
};

struct SLatLon
{
    double lat, lon;

    // convert from degree to radian
    void ConvertDtoR()
    {
        lat = lat * M_PI / 180;
        lon = lon * M_PI / 180;
    }
};

struct SDoubleXY
{
    double x, y;
};

struct SIntXY
{
    int x, y;
    bool operator<(const SIntXY &xy) const
    {
        if (x < xy.x) return true;
        else if (x > xy.x) return false;
        if (y < xy.y) return true;
        else return false;
    }
};

struct SShortXY
{
    short x, y;
};

// a pixel's location in a source
struct SSourcePixelXY
{
    SSourceMetaInfo sourceMetaInfo;
    double offsetw, offseth;

    SDoubleXY GetXY() const
    {
        SDoubleXY xy;
        xy.x = sourceMetaInfo.x + offsetw * sourceMetaInfo.xScale;
        xy.y = sourceMetaInfo.y + offseth * sourceMetaInfo.yScale;
        return xy;
    }

    void SetXY(SDoubleXY xy)
    {
        offsetw = (xy.x - sourceMetaInfo.x) / sourceMetaInfo.xScale;
        offseth = (xy.y - sourceMetaInfo.y) / sourceMetaInfo.yScale;
    }
};

// a pixel's coordinates (in tile)
// first, which tile, then, which pixel
struct STilePixelXY
{
    STileXY tilexy;
    int offsetw, offseth;

    SIntXY GetXY(int tileWidth, int tileHeight) const
    {
        SIntXY xy;
        xy.x = tilexy.x * tileWidth + offsetw;
        xy.y = tilexy.y * tileHeight + offseth;
        return xy;
    }

    void SetXY(int x, int y, U8 level, int tileWidth, int tileHeight)
    {
	    tilexy.level = level;
        tilexy.x = x / tileWidth;
	    tilexy.y = y / tileHeight;
        offsetw = x % tileWidth;
        offseth = y % tileHeight;
    }
};

struct SRectF
{
    double top, right, bottom, left;

    // will make top, right, bottom, left has the correct order
    void CorrectBoundary()
    {
        double t;
        if (right < left)
            t = left, left = right, right = t;
        if (bottom < top)
            t = top, top = bottom, bottom = t;
    }

    bool IntersectWith(SRectF another)
    {
        return !(another.left > right || another.right < left || another.top > bottom || another.bottom < top);
    }

    bool FullyCovers(SRectF another)
    {
        return another.left > left && another.right < right && another.top > top && another.bottom < bottom;
    }

    bool ContainsPoint(int x, int y)
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    // enlarge this rectangle to include a certain point
    void EnlargeToInclude(double x, double y)
    {
        if (left > x) left = x;
        if (right < x) right = x;
        if (top > y) top = y;
        if (bottom < y) bottom = y;
    }

    // clip this rectangle to bound, will shrink to a point or line if they don't overlap
    void Clip(SRectF bound)
    {
        if (left < bound.left) left = min(bound.left, right);
        if (right > bound.right) right = max(bound.right, left);
        if (top < bound.top) top = min(bound.top, bottom);
        if (bottom > bound.bottom) bottom = max(bound.bottom, top);
    }
};

struct SMask
{
    // latlon rectangle of this mask
    SRectF rect;

    // 1:SKIP, 2:NEWRANGE, 3:REMOVE (for REMOVE, mask won't shrink according to source)
    int type;

    // new range for void pixels, only useful when maskType==2
    int voidRange[6];
};

struct SOriginAndDate
{
    int date;
    int originId;
    U8 level;
};
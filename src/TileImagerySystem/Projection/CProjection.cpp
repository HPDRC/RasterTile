#include "CProjection.h"
#define OPTIMIZABLE_THRESHOLD   (0.001)
#define OPTIMIZE_LENGTH1         (16)
#define OPTIMIZE_LENGTH2         (8)
#define OPTIMIZE_LENGTH3         (4)

CProjection::CProjection(int tileWidth, int tileHeight, int totalLevelCnt)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    this->tilePixelCnt = tileWidth * tileHeight;
    this->totalLevelCnt = totalLevelCnt;
}

void CProjection::SourceToTile(const CProjection &sourceProjection, const SSourcePixelXY &source, STilePixelXY &tile, U8 level) const
{
    // implement the basic method: sourcexy -> latlon -> tilexy
    SLatLon latlon;
    sourceProjection.SourceToLatLon(source, latlon);
    this->LatLonToTile(latlon, tile, level);
}

void CProjection::TileToSource(const CProjection &sourceProjection, const STilePixelXY &tile, SSourcePixelXY &source) const
{
    // implement the basic method: tilexy -> latlon -> sourcexy
    SLatLon latlon;
    this->TileToLatLon(tile, latlon);
    sourceProjection.LatLonToSource(latlon, source);
}

int CProjection::SourceToProperLevel(const CProjection &sourceProjection, const SSourcePixelXY &source) const
{
    // What's a proper level: on this level, tile pixel change by 1, source pixel change by 1 or less
    // But to reduce the levels generated, source pixel change <= 1.11 is OK
    // Note that both x direction and y direction should be checked, and the larger level should be used

    // get tile pixel
    int level = 0;
    SSourcePixelXY source2 = source, source3 = source, sourceOrg = source;
    STilePixelXY tile;
    for (; level < totalLevelCnt-1; level++)
    {
        // get tile xy
        this->SourceToTile(sourceProjection, source, tile, level);

        // get source pixels
        this->TileToSource(sourceProjection, tile, sourceOrg);
        tile.offsetw++;
        this->TileToSource(sourceProjection, tile, source2);
        tile.offsetw--, tile.offseth++;
        this->TileToSource(sourceProjection, tile, source3);

        // decide the change
        if (abs(source2.offsetw - sourceOrg.offsetw) <= 1.11 
            && abs(source2.offseth - sourceOrg.offseth) <= 1.11
            && abs(source3.offsetw - sourceOrg.offsetw) <= 1.11
            && abs(source3.offseth - sourceOrg.offseth) <= 1.11)
            break;
    }

    return level;
}

void CProjection::TileToSourceBulk_NotOptimized(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, SDoubleXY xy[]) const
{
    // prepare
    STilePixelXY tpxy;
    SSourcePixelXY spxy;
    tpxy.tilexy = tilexy;
    spxy.sourceMetaInfo = sourceMetaInfo;

    // bulk convert
    for (int h=0; h<tileHeight; h++)
    {
        int offset = h * tileWidth;
        for (int w=0; w<tileWidth; w++)
        {
            tpxy.offsetw = w, tpxy.offseth = h;
            TileToSource(sourceProjection, tpxy, spxy);
            xy[offset+w].x = spxy.offsetw, xy[offset+w].y = spxy.offseth;
        }
    }
}

void CProjection::TileToSourceBulk(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, SDoubleXY xy[]) const
{
    // prepare
    STilePixelXY tpxy;
    SSourcePixelXY spxy;
    tpxy.tilexy = tilexy;
    spxy.sourceMetaInfo = sourceMetaInfo;
    int w, h;
    int optimizeLength = 0;

    if (TileToSourceBulk_IsOptimizable(sourceProjection, tilexy, sourceMetaInfo, OPTIMIZE_LENGTH1))
        optimizeLength = OPTIMIZE_LENGTH1;
    else if (TileToSourceBulk_IsOptimizable(sourceProjection, tilexy, sourceMetaInfo, OPTIMIZE_LENGTH2))
        optimizeLength = OPTIMIZE_LENGTH2;
    else if (TileToSourceBulk_IsOptimizable(sourceProjection, tilexy, sourceMetaInfo, OPTIMIZE_LENGTH3))
        optimizeLength = OPTIMIZE_LENGTH3;
    else
        optimizeLength = 0;

    if (optimizeLength != 0)
    {
        for (h=0; h<tileHeight; h++)
        {
            int offset = h * tileWidth;
            for (w=0; w<tileWidth; w+=optimizeLength)
            {
                tpxy.offsetw = w, tpxy.offseth = h;
                TileToSource(sourceProjection, tpxy, spxy);
                xy[offset+w].x = spxy.offsetw, xy[offset+w].y = spxy.offseth;
                if (w!=0)
                {
                    SDoubleXY start = xy[offset + w - optimizeLength];
                    SDoubleXY end = xy[offset + w];
                    double incrementx = (end.x - start.x) / optimizeLength;
                    double incrementy = (end.y - start.y) / optimizeLength;
                    for (int i=w-optimizeLength+1; i<w; i++)
                    {
                        start.x += incrementx;
                        start.y += incrementy;
                        xy[offset+i].x = start.x;
                        xy[offset+i].y = start.y;
                    }
                }
            }
            for (w=w-optimizeLength+1; w<tileWidth; w++)
            {
                tpxy.offsetw = w, tpxy.offseth = h;
                TileToSource(sourceProjection, tpxy, spxy);
                xy[offset+w].x = spxy.offsetw, xy[offset+w].y = spxy.offseth;
            }
        }
    }
    else
        TileToSourceBulk_NotOptimized(sourceProjection, tilexy, sourceMetaInfo, xy);
}

bool CProjection::TileToSourceBulk_IsOptimizable(const CProjection &sourceProjection, const STileXY &tilexy, const SSourceMetaInfo &sourceMetaInfo, int optimizeLength) const
{
    SDoubleXY start, end;
    STilePixelXY tpxy;
    SSourcePixelXY spxy;
    tpxy.tilexy = tilexy;
    spxy.sourceMetaInfo = sourceMetaInfo;

    // set four corners
    int x[4], y[4];
    x[0] = x[2] = 0;
    x[1] = x[3] = tileWidth - optimizeLength - 1;
    y[0] = y[1] = 0;
    y[2] = y[3] = tileHeight - 1;

    // begin to check if optimizable
    for (int i=0; i<4; i++)
    {
        // calc start point
        tpxy.offsetw = x[i], tpxy.offseth = y[i];
        TileToSource(sourceProjection, tpxy, spxy);
        start.x = spxy.offsetw, start.y = spxy.offseth;

        // calc end point
        tpxy.offsetw = x[i] + optimizeLength, tpxy.offseth = y[i];
        TileToSource(sourceProjection, tpxy, spxy);
        end.x = spxy.offsetw, end.y = spxy.offseth;

        // calc increment
        double incrementx = (end.x - start.x) / optimizeLength;
        double incrementy = (end.y - start.y) / optimizeLength;

        // validates points between them
        for (int j=1; j<optimizeLength; j++)
        {
            start.x += incrementx;
            start.y += incrementy;
            tpxy.offsetw = x[i] + j, tpxy.offseth = y[i];
            TileToSource(sourceProjection, tpxy, spxy);
            if (abs(spxy.offsetw - start.x) > OPTIMIZABLE_THRESHOLD || 
                abs(spxy.offseth - start.y) > OPTIMIZABLE_THRESHOLD)
                return false;
        }
    }

    return true;
}

int CProjection::LongitudeToUtmZone(double longitude)
{
    return ((int)(floor((longitude*180/M_PI + 180) / 6))) % 60 + 1;
}

void CProjection::LatLonRectangleExpandByUtm(int zone, const SRectF &rectOrg, SLatLon &tl, SLatLon &tr, SLatLon &bl, SLatLon &br)
{
    // get the proper utm zone
    double centerLat = (rectOrg.top + rectOrg.bottom) / 2;
    double centerLon = (rectOrg.left + rectOrg.right) / 2;

    // init proj4 objects
    projPJ pjLatlon = pj_init_plus("+proj=latlong +datum=NAD83");
    char initString[64];
    sprintf(initString, "+proj=utm +zone=%d %s +datum=NAD83", zone, centerLat>=0 ? "" : "+south");
    projPJ pjUtm = pj_init_plus(initString);

    // convert top left to utm
    SLatLon latlon;
    SRectF rectUtm;
    latlon.lat = rectOrg.top, latlon.lon = rectOrg.left;
    if (pj_transform(pjLatlon, pjUtm, 1, 0, &latlon.lon, &latlon.lat, NULL) != 0)
        goto LATLON_TO_UTM_ERROR;
    rectUtm.top = latlon.lat;
    rectUtm.left = latlon.lon;

    // convert bottom right to utm
    latlon.lat = rectOrg.bottom, latlon.lon = rectOrg.right;
    if (pj_transform(pjLatlon, pjUtm, 1, 0, &latlon.lon, &latlon.lat, NULL) != 0)
        goto LATLON_TO_UTM_ERROR;
    rectUtm.bottom = latlon.lat;
    rectUtm.right = latlon.lon;

    // convert rectangle to top < bottom
    rectUtm.CorrectBoundary();

    // enlarge to include top right
    latlon.lat = rectOrg.top, latlon.lon = rectOrg.right;
    if (pj_transform(pjLatlon, pjUtm, 1, 0, &latlon.lon, &latlon.lat, NULL) != 0)
        goto LATLON_TO_UTM_ERROR;
    rectUtm.EnlargeToInclude(latlon.lon, latlon.lat);

    // enlarge to include bottom left
    latlon.lat = rectOrg.bottom, latlon.lon = rectOrg.left;
    if (pj_transform(pjLatlon, pjUtm, 1, 0, &latlon.lon, &latlon.lat, NULL) != 0)
        goto LATLON_TO_UTM_ERROR;
    rectUtm.EnlargeToInclude(latlon.lon, latlon.lat);

    if (false)
LATLON_TO_UTM_ERROR:
    LogThrow("CProjection.LatLonRectangleExpandByUtm: transform latlon to utm error: %lf, %lf, %d", latlon.lat, latlon.lon, zone);

    // convert rectangle to top > bottom
    double t = rectUtm.top;
    rectUtm.top = rectUtm.bottom, rectUtm.bottom = t;

    // now convert utm back to latlon
    tl.lat = rectUtm.top, tl.lon = rectUtm.left;
    if (pj_transform(pjUtm, pjLatlon, 1, 0, &tl.lon, &tl.lat, NULL) != 0)
        goto UTM_TO_LATLON_ERROR;

    tr.lat = rectUtm.top, tr.lon = rectUtm.right;
    if (pj_transform(pjUtm, pjLatlon, 1, 0, &tr.lon, &tr.lat, NULL) != 0)
        goto UTM_TO_LATLON_ERROR;

    bl.lat = rectUtm.bottom, bl.lon = rectUtm.left;
    if (pj_transform(pjUtm, pjLatlon, 1, 0, &bl.lon, &bl.lat, NULL) != 0)
        goto UTM_TO_LATLON_ERROR;

    br.lat = rectUtm.bottom, br.lon = rectUtm.right;
    if (pj_transform(pjUtm, pjLatlon, 1, 0, &br.lon, &br.lat, NULL) != 0)
        goto UTM_TO_LATLON_ERROR;

    if (false)
UTM_TO_LATLON_ERROR:
    LogThrow("CProjection.LatLonRectangleExpandByUtm: transform utm to latlon error: %lf, %lf, %d", latlon.lat, latlon.lon, zone);

    pj_free(pjUtm), pj_free(pjLatlon);
}
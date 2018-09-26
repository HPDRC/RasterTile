#include "CBingReader.h"
#include "CDBCache.h"
#include "..\Common\CImg.h"
#include "..\Projection\CProjectionBing.h"
#include "..\FileSystem\CSqlConnection.h"
#include "..\FileSystem\CTableTiles.h"

const int CBingReader::SOURCELIST_MAX_AREA = 20*20;
CProjectionBing CBingReader::projBing;

CBingReader::CBingReader(char *dbConnectionString, char *originName, int zone, int date, int jpegQuality, bool enableLog)
    : CReader(CProjectionBing::TILE_WIDTH, CProjectionBing::TILE_HEIGHT, dbConnectionString, originName, zone, date, jpegQuality, enableLog)
{
}

void CBingReader::GetSourceListJson(int z, double top, double right, double bottom, double left, string &outputJson)
{
    STilePixelXY tpxy;
    SLatLon latlon;

    latlon.lat = top, latlon.lon = left;
    projBing.LatLonToTile(latlon, tpxy, z);
    STileXY tile1 = tpxy.tilexy;

    latlon.lat = bottom, latlon.lon = right;
    projBing.LatLonToTile(latlon, tpxy, z);
    STileXY tile2 = tpxy.tilexy;

    int x1 = min(tile1.x, tile2.x);
    int x2 = max(tile1.x, tile2.x);
    int y1 = min(tile1.y, tile2.y);
    int y2 = max(tile1.y, tile2.y);

    int area = (x2-x1+1) * (y2-y1+1);
    if (area > SOURCELIST_MAX_AREA)
    {
        outputJson = "{\"success\":false, \"error_message\":\"requested area is too large\", \"sources\":[]}";
        return;
    }

    // get origins and dates
    vector<SOriginAndDate> infos;
    CTableTiles::GetOrigins(*pConn, x1, y1, x2, y2, z, zone, infos);

    // organize origins
    set<int> originIds;
    for (int i=0; i<infos.size(); i++)
        originIds.insert(infos[i].originId);

    // generate json string
    try
    {
        outputJson = "{\"success\":true, \"error_message\":\"\", \"sources\":[";

        // for each source, generate a json object
        set<int> dates;
        char tmp[64];
        bool firstSource = true;
        for (set<int>::iterator it=originIds.begin(); it!=originIds.end(); it++)
        {
            // add source name
            int originId = *it;
            dates.clear();
            if (firstSource)
            {
                outputJson += "{\"name\":\"";
                firstSource = false;
            }
            else
                outputJson += ", {\"name\":\"";
            char *name = pCache->GetOriginName(originId);
            if (name == NULL)
                LogThrow("source name for id %d is NULL", originId);
            outputJson += name;

            // add source dates
            outputJson += "\", \"dates\":[";
            for (int j=0; j<infos.size(); j++)
            {
                if (infos[j].originId != originId) continue;
                if (infos[j].date == 0) continue;
                dates.insert(infos[j].date);
            }
            bool firstDate = true;
            for (set<int>::iterator itDate=dates.begin(); itDate!=dates.end(); itDate++)
            {
                if (firstDate)
                {
                    sprintf(tmp, "\"%d\"", *itDate);
                    firstDate = false;
                }
                else
                    sprintf(tmp, ", \"%d\"", *itDate);

                outputJson += tmp;
            }

            // close source object
            outputJson += "]}";
        }

        // close entire json object
        outputJson += "]}";
    }
    catch (char * msg)
    {
        outputJson = "{\"success\":false, \"error_message\":\"internal error: ";
        outputJson += msg;
        outputJson += "\", \"sources\":[]}";
    }
}

void CBingReader::GetMosaic(int z, int width, int height, double topLatRadian, double leftLonRadian)
{
    SIntXY xy;
    STilePixelXY tpxy;
    SLatLon latlon;

    // get request range
    latlon.lat = topLatRadian, latlon.lon = leftLonRadian;
    projBing.LatLonToTile(latlon, tpxy, z);
    xy = tpxy.GetXY(tileWidth, tileHeight);

    // get tile and resample to wanted width and height
    CImg *finalImg = CReader::GetMosaic(xy.x, xy.y, xy.x + width - 1, xy.y + height - 1, z);
    if (finalImg == NULL) return;

    // return final buffer
    resultArray = finalImg->EncodeAsJpeg(jpegQuality, &resultArrayLength);
    delete finalImg;
    return;
}

void CBingReader::GetMosaic(int width, int height, double top, double right, double bottom, double left)
{
    SIntXY xy1, xy2;
    STilePixelXY tpxy;
    SDoubleXY xy;
    double meterResolution = abs((right-left)/width);
    int z = projBing.MeterResolutionToLevel(meterResolution);

    WriteLog("########## CBingReader: GetMosaic at (top,right,bottom,left,z): %lf,%lf,%lf,%lf,%d ##########", top, right, bottom, left, z);
    WriteLog("Requested resolution = %lf, Used resolution = %lf", meterResolution, CProjectionBing::mapMeterResolutionArray[z]);

    // get request range
    xy.y = top, xy.x = left;
    projBing.MeterToTile(xy, tpxy, z);
    xy1 = tpxy.GetXY(tileWidth, tileHeight);

    xy.y = bottom, xy.x = right;
    projBing.MeterToTile(xy, tpxy, z);
    xy2 = tpxy.GetXY(tileWidth, tileHeight);

    // get tile and resample to wanted width and height
    CImg *finalImg = CReader::GetMosaic(xy1.x, xy1.y, xy2.x, xy2.y, z);
    if (finalImg == NULL) return;
    WriteLog("Resample final image from %d,%d to %d,%d", finalImg->width, finalImg->height, width, height);
    finalImg->Resample(width, height);

    // return final buffer
    resultArray = finalImg->EncodeAsJpeg(jpegQuality, &resultArrayLength);
    delete finalImg;
    return;
}

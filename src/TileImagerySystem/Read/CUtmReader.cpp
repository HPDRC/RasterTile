#include "CUtmReader.h"
#include "CDBCache.h"
#include "..\Common\CImg.h"
#include "..\FileSystem\CSqlConnection.h"
#include "..\FileSystem\CTableTiles.h"

CProjectionUtm* CUtmReader::projUtm = new CProjectionUtm();
const int CUtmReader::TIMESERIES_BOX_HALF_WIDTH = 500;
const int CUtmReader::TIMESERIES_BOX_HALF_HEIGHT = 250;
const int CUtmReader::LOWEST_POSSIBLE_LEVEL = 24;   // corresponds to resolution = 3 inch = 7.62cm
const int CUtmReader::HIGHEST_USEFUL_LEVEL = 10;    // corresponds to resolution = 76m


CUtmReader::CUtmReader(char *dbConnectionString, char *originName, int zone, int date, int jpegQuality, bool enableLog)
    : CReader(CProjectionUtm::TILE_WIDTH, CProjectionUtm::TILE_HEIGHT, dbConnectionString, originName, zone, date, jpegQuality, enableLog)
{
}

void CUtmReader::GetMosaic(double utmx1, double utmy1, double utmx2, double utmy2, double res)
{
    try
    {
        STilePixelXY tpxy1, tpxy2;
        SIntXY xy1, xy2;

        // first, convert res to level
        int level = projUtm->ResolutionToLevel(res);

        // then, get range
        tpxy1 = projUtm->UtmXyToTilePixel(utmx1, utmy1, level);
        xy1 = tpxy1.GetXY(tileWidth, tileHeight);
        tpxy2 = projUtm->UtmXyToTilePixel(utmx2, utmy2, level);
        xy2 = tpxy2.GetXY(tileWidth, tileHeight);

        // get tile and resample to wanted width and height
        CImg *finalImg = CReader::GetMosaic(xy1.x, xy1.y, xy2.x, xy2.y, level);
        if (finalImg == NULL) return;
        int width = abs((int)floor((utmx1 - utmx2) / res + 0.5));
        int height = abs((int)floor((utmy1 - utmy2) / res + 0.5));
        finalImg->Resample(width, height);

        // return final buffer
        resultArray = finalImg->EncodeAsJpeg(jpegQuality, &resultArrayLength);
        delete finalImg;
        return;
    }
    catch (char * msg)
    {
        Log("CUtmReader.GetMosaic: exception when requesting %d,%d,%d,%d,%d(x1,y1,x2,y2,zone)  Detail: %s ", (int)utmx1, (int)utmy1, (int)utmx2, (int)utmy2, zone, msg);
    }
    catch (...)
    {
        Log("CUtmReader.GetMosaic: unknow exception when requesting %d,%d,%d,%d,%d(x1,y1,x2,y2,zone)", (int)utmx1, (int)utmy1, (int)utmx2, (int)utmy2, zone);
    }
}

void CUtmReader::GetSourceListXml(double utmx, double utmy, double res, string &outputXml)
{
    vector<SOriginAndDate> infos;
    STilePixelXY tpxy;

    // if res == 0, get origins on all levels
    if (res == 0)
    {
        // first, get the lowest possible tile
        tpxy = projUtm->UtmXyToTilePixel(utmx, utmy, LOWEST_POSSIBLE_LEVEL);
        STileXY tilexy = tpxy.tilexy;

        // then, get all fathers of this tile
        for (int i=0; i<LOWEST_POSSIBLE_LEVEL - HIGHEST_USEFUL_LEVEL + 1; i++)
        {
            int preSize = (int)infos.size();
            CTableTiles::GetOrigins(*pConn, tilexy, zone, &infos);
            if (infos.size() > preSize)
            {
                SOriginAndDate bestOrigin;
                bestOrigin.date = 0, bestOrigin.level = tilexy.level, bestOrigin.originId = 0;
                infos.push_back(bestOrigin);
            }
            tilexy = tilexy.Father();
        }

        // when there is nothing at all, add best_available origin so the flash map can be initialized
        if (infos.size() == 0)
        {
            for (U8 level = HIGHEST_USEFUL_LEVEL; level < LOWEST_POSSIBLE_LEVEL; level++)
            {
                SOriginAndDate bestOrigin;
                bestOrigin.date = 0, bestOrigin.level = level, bestOrigin.originId = 0;
                infos.push_back(bestOrigin);
            }
        }
    }
    else
    {
        // get the leftTop, bottomRight of required box
        U8 level = projUtm->ResolutionToLevel(res);
        tpxy = projUtm->UtmXyToTilePixel(utmx - TIMESERIES_BOX_HALF_WIDTH, utmy + TIMESERIES_BOX_HALF_HEIGHT, level);
        STileXY tile1 = tpxy.tilexy;
        tpxy = projUtm->UtmXyToTilePixel(utmx + TIMESERIES_BOX_HALF_WIDTH, utmy - TIMESERIES_BOX_HALF_HEIGHT, level);
        STileXY tile2 = tpxy.tilexy;

        int x1 = min(tile1.x, tile2.x);
        int x2 = max(tile1.x, tile2.x);
        int y1 = min(tile1.y, tile2.y);
        int y2 = max(tile1.y, tile2.y);

        CTableTiles::GetOrigins(*pConn, x1, y1, x2, y2, level, zone, infos);
    }

    // get a list of all origins
    set<int> originIds;
    for (int i=0; i<infos.size(); i++)
        originIds.insert(infos[i].originId);


    // convert map to string
    outputXml = "<?xml version=\"1.0\"?><so>";
    char tmp[64];
    set<int> dates, levels;

    // for each origin
    for (set<int>::iterator it=originIds.begin(); it!=originIds.end(); it++)
    {
        // prepare header
        int originId = *it;
        dates.clear(), levels.clear();
        outputXml += "<source name=\"";
        char *name = pCache->GetOriginName(originId);
        if (name == NULL)
            LogThrow("CUtmReader.GetOriginList: name == NULL");
        outputXml += name;
        outputXml += "\">";

        // get list of levels and dates
        for (int j=0; j<infos.size(); j++)
        {
            if (infos[j].originId != originId) continue;
            levels.insert(infos[j].level);
            dates.insert(infos[j].date);
        }

        // write levels and dates
        for (set<int>::iterator itLevel=levels.begin(); itLevel!=levels.end(); itLevel++)
        {
            outputXml += "<res notch=\"true\">";
            sprintf(tmp, "%.6lf", CProjectionUtm::mapResArray[*itLevel]);
            outputXml += tmp;
            outputXml += "</res>";
        }
        for (set<int>::iterator itDate=dates.begin(); itDate!=dates.end(); itDate++)
        {
            if (*itDate == 0) continue;
            outputXml += "<source_image_date>";
            sprintf(tmp, "%d", *itDate);
            outputXml += tmp;
            outputXml += "</source_image_date>";
        }

        // add ending
        outputXml += "</source>";
    }
    outputXml += "</so>";
}

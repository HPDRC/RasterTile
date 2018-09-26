#pragma once
#include "includes.h"
#include "CReader.h"
#include "..\Projection\CProjectionUtm.h"

class CUtmReader : public CReader
{
private:
    static CProjectionUtm *projUtm;     // proj4 lib is not multi-thread safe! only use one static object for all readers!
    static const int TIMESERIES_BOX_HALF_WIDTH;
    static const int TIMESERIES_BOX_HALF_HEIGHT;
    static const int LOWEST_POSSIBLE_LEVEL;
    static const int HIGHEST_USEFUL_LEVEL;

public:
    CUtmReader(char *dbConnectionString, char *originName, int zone, int date, int jpegQuality, bool enableLog);

    void GetMosaic(double utmx1, double utmy1, double utmx2, double utmy2, double res);

    // res: pass 0 to search on all resolution
    void GetSourceListXml(double utmx, double utmy, double res, string &outputXml);
};
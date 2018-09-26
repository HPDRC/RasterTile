#pragma once
#include "includes.h"
#include "CReader.h"

class CBingReader : public CReader
{
private:
    static const int SOURCELIST_MAX_AREA;
    static CProjectionBing projBing;

public:
    CBingReader(char *dbConnectionString, char *originName, int zone, int date, int jpegQuality, bool enableLog);

    void GetMosaic(int z, int width, int height, double topLatRadian, double leftLonRadian);

    void GetMosaic(int width, int height, double top, double right, double bottom, double left);

    void GetSourceListJson(int z, double top, double right, double bottom, double left, string &outputJson);
};
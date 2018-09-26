#pragma once
#include "includes.h"
using namespace System;

namespace TileImagerySystem_CLR
{
    public ref class MReader
	{
    private:
        char *conn, *originName;
        int date, jpegQuality, zone;
        bool enableLog;

        void SetLog(std::string logStr);

    public:
        String^ logString;

    public:
        // originName MUST be all upper cases
        MReader(String^ dbConnectionString, String^ originName, bool enableLog, int zone, int date, int jpegQuality);
        ~MReader();

        /////////////////////////////////// General /////////////////////////////////////
        // get a tile, forceQuality: convert jpeg to specified quality instead of default quality
        array<Byte>^ GetTile(MSTileXY tilexy, bool forceQuality);
        
        // check if a tile exists
        bool GetTileExist(MSTileXY tilexy);

        /////////////////////////////////// UTM /////////////////////////////////////
        // get a mosaic image which is merged from multiple tiles
        array<Byte>^ GetUtmMosaic(double utmx1, double utmy1, double utmx2, double utmy2, double res);

        // get an json list of available sources and their dates and resolutions
        String^ GetUtmSourceListXml(double utmx, double utmy, double res);

        /////////////////////////////////// Bing /////////////////////////////////////
        // get an json list of available sources and their dates
        String^ GetBingSourceListJson(int z, double top, double right, double bottom, double left);

        array<Byte>^ GetBingMosaic(int z, int width, int height, double top, double left);

        array<Byte>^ GetBingMosaic(int width, int height, double top, double right, double bottom, double left);
	};
}
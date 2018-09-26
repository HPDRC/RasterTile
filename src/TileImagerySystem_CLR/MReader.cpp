#include "MReader.h"
#include "..\TileImagerySystem\Read\CTile.h"
#include "..\TileImagerySystem\Read\CUtmReader.h"
#include "..\TileImagerySystem\Read\CBingReader.h"
using namespace System::Runtime::InteropServices;

namespace TileImagerySystem_CLR
{
    MReader::MReader(String^ dbConnectionString, String^ originName, bool enableLog, int zone, int date, int jpegQuality)
    {
        this->conn = Helper::StringToCharArray(dbConnectionString);
        this->originName = Helper::StringToCharArray(originName);
        this->enableLog = enableLog;
        this->zone = zone;
        this->date = date;
        this->jpegQuality = jpegQuality;
        this->logString = "";
    }

    MReader::~MReader()
    {
        Helper::StringToCharArrayClean(conn);
        Helper::StringToCharArrayClean(originName);
    }

    bool MReader::GetTileExist(MSTileXY tilexy)
    {
        try
        {
            // prepare params
            STileXY tilexyUnmanaged;
            memcpy(&tilexyUnmanaged, &tilexy, sizeof(STileXY));
            
            // get tile
            CReader reader(0, 0, conn, originName, zone, date, jpegQuality, enableLog);
            bool result = reader.GetTileExist(tilexyUnmanaged);
            SetLog(reader.GetLog());
            return result;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    array<Byte>^ MReader::GetTile(MSTileXY tilexy, bool forceQuality)
    {
        try
        {
            // prepare params
            STileXY tilexyUnmanaged;
            memcpy(&tilexyUnmanaged, &tilexy, sizeof(STileXY));
            
            // get tile
            CReader reader(0, 0, conn, originName, zone, date, jpegQuality, enableLog);
            CTile* pTile = reader.GetTile(tilexyUnmanaged);
            SetLog(reader.GetLog());

            // if has no such tile, return empty array
            if (pTile==NULL)
                return gcnew array<Byte>(0);
            if (forceQuality && jpegQuality != JPEG_QUALITY)
                pTile->SaveImageObjectToBuffer(jpegQuality);
            if (pTile->GetImageBuffer() == NULL)
                return gcnew array<Byte>(0);
            pTile->ConvertImageBufferToJpg();

            // return result
            array<Byte>^ arr =  gcnew array<Byte>(pTile->imgLength);
            pin_ptr<unsigned char> pUnmanagedArr = &arr[0];
            memcpy(pUnmanagedArr, pTile->GetImageBuffer(), pTile->imgLength);
            return arr;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
    
    array<Byte>^ MReader::GetUtmMosaic(double utmx1, double utmy1, double utmx2, double utmy2, double res)
    {
        try
        {
            // get best tile
            CUtmReader reader(conn, originName, zone, date, jpegQuality, enableLog);
            reader.GetMosaic(utmx1, utmy1, utmx2, utmy2, res);
            SetLog(reader.GetLog());

            // if has no such tile, or this tile has no image, return empty array
            if (reader.resultArray == NULL)
                return gcnew array<Byte>(0);

            // else, return this tile's image
            array<Byte>^ result =  gcnew array<Byte>(reader.resultArrayLength);
            pin_ptr<unsigned char> pUnmanagedArr = &result[0];
            memcpy(pUnmanagedArr, reader.resultArray, reader.resultArrayLength);
            return result;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    String^ MReader::GetUtmSourceListXml(double utmx, double utmy, double res)
    {
        try
        {
            CUtmReader reader(conn, NULL, zone, 0, 0, enableLog);
            string xml;
            reader.GetSourceListXml(utmx, utmy, res, xml);
            SetLog(reader.GetLog());
            return gcnew String(xml.c_str());
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    String^ MReader::GetBingSourceListJson(int z, double top, double right, double bottom, double left)
    {
        try
        {
            CBingReader reader(conn, NULL, zone, 0, 0, enableLog);
            string json;
            reader.GetSourceListJson(z, top, right, bottom, left, json);
            SetLog(reader.GetLog());
            return gcnew String(json.c_str());
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    array<Byte>^ MReader::GetBingMosaic(int z, int width, int height, double top, double left)
    {
        try
        {
            // get best tile
            CBingReader reader(conn, originName, zone, date, jpegQuality, enableLog);
            reader.GetMosaic(z, width, height, top, left);
            SetLog(reader.GetLog());

            // if has no such tile, or this tile has no image, return empty array
            if (reader.resultArray == NULL)
                return gcnew array<Byte>(0);

            // else, return this tile's image
            array<Byte>^ result =  gcnew array<Byte>(reader.resultArrayLength);
            pin_ptr<unsigned char> pUnmanagedArr = &result[0];
            memcpy(pUnmanagedArr, reader.resultArray, reader.resultArrayLength);
            return result;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    array<Byte>^ MReader::GetBingMosaic(int width, int height, double top, double right, double bottom, double left)
    {
        try
        {
            // get best tile
            CBingReader reader(conn, originName, zone, date, jpegQuality, enableLog);
            reader.GetMosaic(width, height, top, right, bottom, left);
            SetLog(reader.GetLog());

            // if has no such tile, or this tile has no image, return empty array
            if (reader.resultArray == NULL)
                return gcnew array<Byte>(0);

            // else, return this tile's image
            array<Byte>^ result =  gcnew array<Byte>(reader.resultArrayLength);
            pin_ptr<unsigned char> pUnmanagedArr = &result[0];
            memcpy(pUnmanagedArr, reader.resultArray, reader.resultArrayLength);
            return result;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }


    void MReader::SetLog(string logStr)
    {
        if (!enableLog) return;
        logString = gcnew String(logStr.c_str());
        logString = "<pre>" + logString + "</pre>";
    }
}
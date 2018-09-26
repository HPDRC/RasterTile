#pragma once
#include "includes.h"
using namespace System;

namespace TileImagerySystem_CLR
{
    public value struct MSDate
    {
        UInt16 year;
        Byte  month;
        Byte  day;

    public:
        virtual String^ ToString() override
        {
            return year + "/" + month + "/" + day;
        }
    };

    public value struct MSSourceSetMetaInfo
    {
        MSDate date;
        int originId;
        int outputZone;
    };

	public enum class MESourceFileType
	{
		unknown = 0,
		doqq = 1,
		doqqc = 2,
		tif = 3,
		png = 4,
		jpg = 5,
	};

    public value struct MSSourceMetaInfo
    {
		MESourceFileType fileType;
        double x;	// the utm x coord of the upper left pixel of this image
        double y;	// the utm y coord of the upper left pixel of this image
        double xScale;			// x resolution (one pixel = ? meters)
        double yScale;			// y resolution
        int width;				// width of the image (how many pixels)
        int height;				// height of the image (how many pixels)
        int zone;				// should be from -60 to 60, exclude 0
    };

    public value struct MSTileXY
    {
        UInt32 x, y;
        Byte level;
    };

    public value struct MSTileMetaInfo
    {
        UInt32 imgOffset; // offset of the img in the corresponding dat file
        UInt32 imgLength;	// length of the img in the corresponding dat file
        MSDate date;    // date of this tile
        int originId;   // this tile belongs to which origin
        int zone;       // zone of this tile
        MSTileXY tilexy;
        Byte hasAlpha;

        public:
        virtual String^ ToString() override
        {
            return "Tilexy:" + tilexy.ToString() 
                + " Date:" + date.ToString() 
                + " OriginId:" + originId 
                + " zone:" + zone;
        }
    };

    public value struct MSLoaderStat
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

    public:
        virtual String^ ToString() override
        {
            return "Preload:" + unfinishedPreloadTaskCnt + "/" + finishedPreloadTaskCnt
                + " Load:" + unfinishedLoadTaskCnt + "/" + finishedLoadTaskCnt
                + " Calc:" + unfinishedCalcTaskCnt + "/" + finishedCalcTaskCnt
                + " Write:" + unfinishedWriteTaskCnt + "/" + finishedWriteTaskCnt
                + "\r\nSourceInMemory:" + sourceInMemoryCnt + " TileInMemory:" + tileInMemoryCnt + " MemoryUsage:" + memoryUsedBySource + "/" + memoryUsedByTile + "/" + memoryUsedByTileMax
                + "\r\nTileOutputed:" + tileOutputedCnt + "/" + (tileTotalCnt)
                + " RunningThread:" + runningThreadCnt
                + " WaitingForMemory:" + threadCntWaitingForSourceMemory + "/" + threadCntWaitingForTileMemory
                + "\r\nTileMemoryLimit:" + tileMemoryLimit + " TileMemoryEstimate:" + tileMemoryEstimate
                + (tileMemoryEstimate > tileMemoryLimit ? "\r\n!!! Warning !!!: Tile Memory Limit is exceeded. Consider increasing the limit." : "");
        }
    };
}
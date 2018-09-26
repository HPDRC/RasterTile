#pragma once
#include "includes.h"
using namespace System;

namespace TileImagerySystem_CLR
{
    public ref class MUtilities
	{
    public:
        static MSSourceMetaInfo ReadUtmMetaFromTiff(String^ filePath);

        static void ReadUtmMetaFromDoqq(String^ filePath, MSSourceMetaInfo %sourceMetaInfo, String^ %imageDate);

        static int TransformProjection(String^ srcProj, String^ dstProj, double %x, double %y);
        
        // MUST be called before use TileImagerySystem dll
        static void InitConfig(String^ configString);

        // MUST be called before load each loading list
        static void ResetConfig();
        
        // build index for files in a directory, return how many records are inserted
        static int BuildIndex(String^ dirPath, String^ connectionString);

        static void LatLonRectangleExpandByUtm(int zone, array<double>^ rectOrg, array<double>^ rectNew);
	};
}
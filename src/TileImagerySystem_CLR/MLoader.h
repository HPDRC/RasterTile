#pragma once
#include "includes.h"
#include "..\TileImagerySystem\Load\CLoader.h"
using namespace System;

namespace TileImagerySystem_CLR
{
    public ref class MLoader
	{
    private:
        CLoader *pLoader;

    public:
        MLoader(String^ outputDir, MSSourceSetMetaInfo sourceSetMetaInfo);
        ~MLoader();

        // sourceFileType: 0/tiff, 1/doqq, 2/doqqc
        void AddSource(MSSourceMetaInfo sourceMetaInfo, String^ dataPath);
        void Load();
        void Abort();
        void Close();
        MSLoaderStat GetStat();
	};
}

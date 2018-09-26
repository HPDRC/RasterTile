#pragma once
#include "includes.h"
#include "..\TileImagerySystem\FileSystem\CMetaFile.h"
using namespace System;

namespace TileImagerySystem_CLR
{
    public ref class MMetaFile
	{
    private:
        CMetaFile* pMetaFile;

    public:
        MMetaFile(String^ filePath);
        ~MMetaFile();
        
        MSTileMetaInfo Read(int recordIndex);
        int GetRecordCnt();
        void Close();
	};
}
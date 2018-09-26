#pragma once
#include "includes.h"
#include "..\TileImagerySystem\Common\CFile.h"
using namespace System;

namespace TileImagerySystem_CLR
{
    public ref class MDataFile
	{
    public:
        static array<Byte>^ Read(String^ filePath, int imgOffset, int imgLength);
	};
}
#pragma once
#include "includes.h"
#include <string>
using namespace std;

namespace TileImagerySystem_CLR
{
    class Helper
	{
    public:
        static void Throw(char * msg);

        static char* StringToCharArray(String^ str);

        static void StringToCharArrayClean(char* charArray);

        static void MarshalString(String ^input, string& output);
    };
}
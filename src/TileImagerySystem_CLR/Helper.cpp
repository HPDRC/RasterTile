#include "Helper.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace TileImagerySystem_CLR
{
    void Helper::Throw(char * msg)
    {
        throw gcnew Exception(Marshal::PtrToStringAnsi((IntPtr)msg));
    }

    char* Helper::StringToCharArray(String^ str)
    {
        return (char*)Marshal::StringToHGlobalAnsi(str).ToPointer();
    }

    void Helper::StringToCharArrayClean(char* charArray)
    {
        Marshal::FreeHGlobal((IntPtr)charArray);
    }

    void Helper::MarshalString(String ^input, string& output)
    {
        const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(input)).ToPointer();
        output = chars;
        Marshal::FreeHGlobal(IntPtr((void*)chars));
    }
}
#include "MDataFile.h"
using namespace System::Runtime::InteropServices;

namespace TileImagerySystem_CLR
{
    array<Byte>^ MDataFile::Read(String^ filePath, int imgOffset, int imgLength)
    {
        try
        {
            // create CFile Object
            string filePathUnmanaged;
            Helper::MarshalString(filePath, filePathUnmanaged);
            CFile dataFile(filePathUnmanaged, false);

            // read data
            array<Byte>^ arr =  gcnew array<Byte>(imgLength);
            pin_ptr<unsigned char> pUnmanagedArr = &arr[0];
            dataFile.SeekStrict(imgOffset, SEEK_SET);
            dataFile.ReadStrict(pUnmanagedArr, imgLength);
            return arr;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
}

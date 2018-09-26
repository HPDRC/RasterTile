#include "MMetaFile.h"
using namespace System::Runtime::InteropServices;

namespace TileImagerySystem_CLR
{
    MMetaFile::MMetaFile(String^ filePath)
    {
        try
        {
            string filePathUnmanaged;
            Helper::MarshalString(filePath, filePathUnmanaged);
            pMetaFile = new CMetaFile(filePathUnmanaged, false);
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    MMetaFile::~MMetaFile()
    {
        Close();
    }

    void MMetaFile::Close()
    {
        try
        {
            if (pMetaFile != NULL)
                delete pMetaFile, pMetaFile = NULL;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    MSTileMetaInfo MMetaFile::Read(int recordIndex)
    {
        try
        {
            STileMetaInfo metaInfo;
            MSTileMetaInfo metaInfoManaged;
            pMetaFile->Read(recordIndex, &metaInfo);
            memcpy(&metaInfoManaged, &metaInfo, sizeof(metaInfoManaged));
            return metaInfoManaged;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    int MMetaFile::GetRecordCnt()
    {
        try
        {
            return pMetaFile->GetRecordCnt();
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
}
#include "MLoader.h"
using namespace System::Runtime::InteropServices;

namespace TileImagerySystem_CLR
{
    MLoader::MLoader(String^ outputDir, MSSourceSetMetaInfo sourceSetMetaInfo)
    {
        try
        {
            // convert structure from managed to unmanaged
            SSourceSetMetaInfo metaInfo;
            memcpy(&metaInfo, &sourceSetMetaInfo, sizeof(metaInfo));

            // call unmanaged method
            string outputDirUnmanaged;
            Helper::MarshalString(outputDir, outputDirUnmanaged);
            pLoader = new CLoader(outputDirUnmanaged, metaInfo);
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    MLoader::~MLoader()
    {
        Close();
    }

    void MLoader::AddSource(MSSourceMetaInfo sourceMetaInfo, String^ dataPath)
    {
        try
        {
            // convert structure from managed to unmanaged
            SSourceMetaInfo metaInfo;
            memcpy(&metaInfo, &sourceMetaInfo, sizeof(metaInfo));

            string dataPathUnmanaged;
            Helper::MarshalString(dataPath, dataPathUnmanaged);
            pLoader->AddSource(metaInfo, dataPathUnmanaged);
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    void MLoader::Load()
    {
        try
        {
            pLoader->Load();
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    void MLoader::Abort()
    {
        try
        {
            pLoader->Abort();
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
    
    void MLoader::Close()
    {
        try
        {
            delete pLoader, pLoader = 0;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    MSLoaderStat MLoader::GetStat()
    {
        try
        {
            SLoaderStat stat = pLoader->GetStat();
            MSLoaderStat mstat;
            memcpy(&mstat, &stat, sizeof(mstat));
            return mstat;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
}
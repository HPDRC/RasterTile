#include "MUtilities.h"
#include "..\TileImagerySystem\Common\CUtilities.h"
#include "..\TileImagerySystem\Common\CConfig.h"
#include "..\TileImagerySystem\Common\CDoqq.h"
#include "..\TileImagerySystem\Common\CFile.h"
#include "..\TileImagerySystem\Common\CHelper.h"
#include "..\TileImagerySystem\FileSystem\CIndexBuilder.h"
#include "..\TileImagerySystem\Load\includes.h"
#include "..\TileImagerySystem\Projection\CProjection.h"
using namespace System::Runtime::InteropServices;

namespace TileImagerySystem_CLR
{
    MSSourceMetaInfo MUtilities::ReadUtmMetaFromTiff(String^ filePath)
    {
        try
        {
            CHelper::InitTiffLib();
            string filePathUnmanaged;
            Helper::MarshalString(filePath, filePathUnmanaged);
            SSourceMetaInfo metaInfo = CUtilities::ReadUtmMetaFromTif(filePathUnmanaged);

            MSSourceMetaInfo metaInfoManaged;
            memcpy(&metaInfoManaged, &metaInfo, sizeof(SSourceMetaInfo));
            return metaInfoManaged;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    void MUtilities::ReadUtmMetaFromDoqq(String^ filePath, MSSourceMetaInfo %sourceMetaInfo, String^ %imageDate)
    {
        try
        {
            string filePathUnmanaged;
            Helper::MarshalString(filePath, filePathUnmanaged);
			CFile doqqFile(filePathUnmanaged, false);
			char buff[DOQQ_MAX_HEADER_LEN];
			char imageDateUnmanaged[MAX_PATH_LEN];
			int readLength = doqqFile.Read(buff, DOQQ_MAX_HEADER_LEN);
			int headerLength = 0;
			SSourceMetaInfo smi;
            CDoqq::ReadHeader(filePathUnmanaged.c_str(), buff, readLength, &smi, imageDateUnmanaged, &headerLength);
			sourceMetaInfo.x = smi.x;
			sourceMetaInfo.y = smi.y;
			sourceMetaInfo.xScale = smi.xScale;
			sourceMetaInfo.yScale = smi.yScale;
			sourceMetaInfo.width = smi.width;
			sourceMetaInfo.height = smi.height;
			sourceMetaInfo.zone = smi.zone;
			imageDate = Marshal::PtrToStringAnsi((IntPtr)imageDateUnmanaged);
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    int MUtilities::TransformProjection(String^ srcProj, String^ dstProj, double %x, double %y)
    {
        try
        {
            string srcProjUnmanaged, dstProjUnmanaged;
            Helper::MarshalString(srcProj, srcProjUnmanaged);
            Helper::MarshalString(dstProj, dstProjUnmanaged);
            double x1 = x, y1 = y;
            int result = CUtilities::TransformProjection(srcProjUnmanaged.c_str(), dstProjUnmanaged.c_str(), &x1, &y1);
            x = x1, y = y1;
            return result;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
	
    void MUtilities::InitConfig(String^ configString)
    {
        try
        {
            string configStringUnmanaged;
            Helper::MarshalString(configString, configStringUnmanaged);
            CConfig::Init(configStringUnmanaged);
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    void MUtilities::ResetConfig()
    {
        try
        {
            CConfig::Reset();
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }


    int MUtilities::BuildIndex(String^ dirPath, String^ connectionString)
    {
        try
        {
            string pathUnmanaged, connUnmanaged;
            Helper::MarshalString(dirPath, pathUnmanaged);
            Helper::MarshalString(connectionString, connUnmanaged);
            return CIndexBuilder::Build(pathUnmanaged, connUnmanaged);
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }

    void MUtilities::LatLonRectangleExpandByUtm(int zone, array<double>^ rectOrg, array<double>^ rectNew)
    {
        try
        {
            SRectF rOrg;
            SLatLon tl, tr, bl, br;
            rOrg.top = rectOrg[0], rOrg.right = rectOrg[1], rOrg.bottom = rectOrg[2], rOrg.left = rectOrg[3];
            CProjection::LatLonRectangleExpandByUtm(zone, rOrg, tl, tr, bl, br);
            rectNew[0] = tl.lat, rectNew[1] = tl.lon;
            rectNew[2] = tr.lat, rectNew[3] = tr.lon;
            rectNew[4] = br.lat, rectNew[5] = br.lon;
            rectNew[6] = bl.lat, rectNew[7] = bl.lon;
        }
        catch (char * msg)
        {
            Helper::Throw(msg);
        }
    }
}
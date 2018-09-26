#include "CUtilities.h"
#include "..\..\libtiff\tiffio.h"
#include "..\..\libGeoTiff\geotiffio.h"
#include "..\..\libGeoTiff\xtiffio.h"
#include "..\..\libGeoTiff\geo_normalize.h"
#include "CHelper.h"
#include "..\..\Proj4\proj_api.h"
#include "..\Read\CDBCache.h"

SSourceMetaInfo CUtilities::ReadUtmMetaFromTif(string metaPath)
{
    SSourceMetaInfo metaInfo;
    memset(&metaInfo, 0, sizeof(metaInfo));
    
    // open tiff file
    TIFF *tiff = XTIFFOpen(metaPath.c_str(), "r");
    if (tiff==NULL)
        LogThrow("CTiffMetaReader: Open tiff fail: %s", metaPath.c_str());
	GTIF *gtif = GTIFNew(tiff);

	// try read width and height
	TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH, &metaInfo.width);
	TIFFGetField(tiff,TIFFTAG_IMAGELENGTH, &metaInfo.height);

	// try read upperleft utm x, y
    int listCnt = 0;
	double *list = 0;
	TIFFGetField(tiff, TIFFTAG_GEOTIEPOINTS, &listCnt, &list);
	if (listCnt >= 6)
	{
		metaInfo.x = list[3];
		metaInfo.y = list[4];
	}
    else
    {
        if (gtif)
            GTIFImageToPCS(gtif, &metaInfo.x, &metaInfo.y);
    }

	// try read resolution
	TIFFGetField(tiff, TIFFTAG_GEOPIXELSCALE, &listCnt, &list);
	if (listCnt >= 2)
	{
		metaInfo.xScale = list[0];
		metaInfo.yScale = -list[1];
	}

	// try read geo tiff info
	if (gtif)
    {
	    GTIFDefn def;
	    if (GTIFGetDefn(gtif, &def))
	    {
		    metaInfo.zone = abs(def.Zone);
		    if (def.MapSys == MapSys_UTM_South)
			    metaInfo.zone = -metaInfo.zone;
		    else if (def.MapSys == MapSys_UTM_North)
			    ;
		    else
			    metaInfo.zone = 0;
	    }
    }

    if (gtif)
        GTIFFree(gtif);
    XTIFFClose(tiff);
    return metaInfo;
}

int CUtilities::TransformProjection(const char *srcProj, const char *dstProj, double *x, double *y)
{
    projPJ src = CDBCache::GetProjCache(srcProj);
    projPJ dst = CDBCache::GetProjCache(dstProj);
    int result = pj_transform(src, dst, 1, 0, x, y, NULL);
    return result;
}
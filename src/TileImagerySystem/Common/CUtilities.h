#pragma once
#include "global_includes.h"

class CUtilities
{
public:
    static SSourceMetaInfo ReadUtmMetaFromTif(string metaPath);
    static int TransformProjection(const char *srcProj, const char *dstProj, double *x, double *y);
};
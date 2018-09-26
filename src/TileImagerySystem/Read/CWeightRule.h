#pragma once
#include "includes.h"

class CWeightRule
{
public:
    int originId;
    U8 zmin, zmax;
    int weight;

    bool IsApply(int originId, U8 z);
};
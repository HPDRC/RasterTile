#include "CWeightRule.h"

bool CWeightRule::IsApply(int originId, U8 z)
{
    return this->originId == originId && z>=zmin && z<=zmax;
}
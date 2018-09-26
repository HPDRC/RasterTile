#pragma once
#include "includes.h"

class CTableQualities
{
public:
    // get a tile's grid_completeness attribute 
    // return -1 if info not found
    static int GetGridCompleteness(CSqlConnection& conn, S64 tileId);

    // set a tile's grid_completeness attribute 
    static void SetGridCompleteness(CSqlConnection& conn, S64 tileId, int gridCompleteness);
};

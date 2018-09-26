#pragma once
#include "includes.h"

class CIndexBuilder
{
public:
    // start to build index for files in a dir
    static int Build(string dirPath, string connectionString);
};


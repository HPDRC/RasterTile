#pragma once

// include standard head files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <cmath>
#include <math.h>
using namespace std;

#include "global_config.h"
#include "global_defines.h"
#include "global_structures.h"
#include "global_functions.h"

// class in Common folder
class CConfig;
class CDoqq;
class CFile;
class CHelper;
class CImg;
class CLoggable;
class CMemory;
class CSemaphore;
class CUtilities;

// class in FileSystem folder
class CDataFileBuilder;
class CIndexBuilder;
class CMetaFile;
class CSqlConnection;
class CTablePathes;
class CTableQualities;
class CTableTiles;

// class in Load folder
class CLoader;
class CObjectPool;
class CPyramid;
class CPyramidNode;
class CSource;
class CSourceList;
class CTaskSet;

// class in Projection folder
class CProjection;
class CProjectionBing;
class CProjectionNASA;
class CProjectionUtm;

// class in Read folder
class CDBCache;
class CNASAReader;
class CReader;
class CTile;
class CUtmReader;
class CWeightRule;

// class in libraries
class CxImage;
class CxMemFile;


/*
    This system use R, G, B as default RGB order
    RGB order is configured in project jpeg, file Jmorecfg.h, line 320
*/
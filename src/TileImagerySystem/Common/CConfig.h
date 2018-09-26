#pragma once
#include "global_includes.h"

// process and store dynamic configurations passed from outside
class CConfig
{
private:
    static void ParseLine(char line[]);
    static void ParseKeyValue(char key[], char value[]);
    static char* Trim(char str[]);
    static char* GetStringValue(char value[]);
    static int GetIntValue(char value[]);
    static double GetDoubleValue(char value[]);

public:
    // initialize config from string
    static void Init(string configString);

    // for now, only masks is cleared
    static void Reset();

    ///////////// used by all components ////////////////////////////
    
    // where to write logs
    static char *LOG_PATH;

    ///////////// used only by loader ////////////////////////////////

    // remove null settings
    static bool TRIM_ENABLED;
    static int TRIM_PIXEL_VOID_RANGE[6];
    static int TRIM_MIN_CLUSTER_SIZE;
    static int TRIM_REMOVE_NOISE;
    static vector<SMask> masks;
    
    // projection settings
    static char *SOURCE_PROJECTION;
    static char *DEST_PROJECTION;

    // how many threads should be used
    static int PRELOAD_THREAD_CNT;
    static int LOAD_THREAD_CNT;
    static int CALC_THREAD_CNT;
    static int WRITE_THREAD_CNT;

    // memory limit on load tasks
    static double MEMORY_LIMIT_SOURCE;
    // memory limit on entire process
    static double MEMORY_LIMIT_TILE;
};

#pragma once
#include "global_includes.h"

// implement some commonly used support functions
class CHelper
{
public:
    static void InitTiffLib();

    static string PathChangeExt(string path, string extOrg, string extNew);
    static string FormatPath(string path);
    static void CreateDir(string path);
    static bool IsFileExist(string path);
    static void DirTraversal(string dir, vector<string> * outPathList);
    static void DirTraversal_EndsWith(string dir, vector<string> * outPathList, string endString);
    static string PathWithoutExt(string path, string ext);
    static string ToDataPath(string path);
    static string ToMetaPath(string path);

    static int Round(double value);
    static int Floor(double value);
    static int Ceil(double value);
};
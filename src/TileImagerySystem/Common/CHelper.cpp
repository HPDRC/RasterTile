#include "CHelper.h"
#include "..\..\libtiff\tiffio.h"
#include "..\..\libGeoTiff\geotiffio.h"
#include "..\..\libGeoTiff\xtiffio.h"
#include "..\..\libGeoTiff\geo_normalize.h"
#include "CConfig.h"
#include "..\Read\CDBCache.h"
#include <Windows.h>


void CHelper::CreateDir(string path)
{
    path = FormatPath(path);

    // if path exist, return
    if (IsFileExist(path)) return;

    // create father dir, then create this dir
    int pos = (int)path.rfind('\\');
    if (pos < 0)
        LogThrow("CreateDir: path not valid: %s", path.c_str());
    string pathFather = path.substr(0, pos);
    CreateDir(pathFather);
    if (!CreateDirectory(path.c_str(), NULL))
        LogThrow("CreateDir: CreateDirectory fails: %s", path.c_str());
}

// trim the slash on the tail
string CHelper::FormatPath(string path)
{
    if (path.at(path.length()-1) == '\\')
        return path.substr(0, path.length()-1);
    else
        return path;
}

bool CHelper::IsFileExist(string path)
{
    if (GetFileAttributes(path.c_str()) == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;
}

void CHelper::DirTraversal(string dir, vector<string> * outPathList)
{
    // find the first file
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = FindFirstFile((dir+"\\*").c_str(), &fdFile);
    if(hFind == INVALID_HANDLE_VALUE)
        return;

    // find the rest
    do
    {
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
        {
            // generate full path
            string path = dir + "\\" + fdFile.cFileName;

            // if this path is a directory, continue to search for files
            if (fdFile.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                DirTraversal(path, outPathList);
            // else, save the path into list
            else
                outPathList->push_back(path);
        }
    } while(FindNextFile(hFind, &fdFile));

    // clean up
    FindClose(hFind);
}

void CHelper::DirTraversal_EndsWith(string dir, vector<string> * outPathList, string endString)
{
    // get all files
    vector<string> pathList;
    DirTraversal(dir, &pathList);

    // enum pathList
    for (vector<string>::iterator it=pathList.begin(); it!=pathList.end(); it++)
    {
        // if current path ends with endString, add it to output
        if ((*it).compare((*it).length() - endString.length(), endString.length(), endString) == 0)
            outPathList->push_back((*it));
    }
}

string CHelper::PathWithoutExt(string path, string ext)
{
    return path.substr(0, path.length()-ext.length());
}

string CHelper::PathChangeExt(string path, string extOrg, string extNew)
{
    return path.substr(0, path.length()-extOrg.length()) + extNew;
}

string CHelper::ToDataPath(string path)
{
    return PathChangeExt(path, META_EXT_NAME, DATA_EXT_NAME);
}

string CHelper::ToMetaPath(string path)
{
    return PathChangeExt(path, DATA_EXT_NAME, META_EXT_NAME);
}

void tiffErrorHandler(const char *, const char *, va_list)
{
}

void CHelper::InitTiffLib()
{
    // init tiff lib
    TIFFSetWarningHandler(tiffErrorHandler);
    TIFFSetErrorHandler(tiffErrorHandler);
}

int CHelper::Round(double value)
{
    if (value == 0)
        return 0;
    else if (value > 0)
        return (int)(value+0.5);
    else
        return (int)(value-0.5);
}

int CHelper::Floor(double value)
{
    return (int)floor(value+0.000001);
}

int CHelper::Ceil(double value)
{
    return (int)ceil(value-0.000001);
}
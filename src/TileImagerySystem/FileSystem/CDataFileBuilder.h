#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

class CDataFileBuilder : CSemaphore
{
private:
    // all files will be written to this directory
    string _rootDir;

    // the file handle of currently used data file
    CFile * _pDataFile;

    // currently used meta file object
    CMetaFile *_pMetaFile;

    // the index of the next file
    int _nextFileIndex;

public:
    // constructor. It will ensure outDir is a valid directory. (if not, an string exception is thrown)
    // rootDir: the output directory for all data files.
    // *** Note that for one directory, there can be only one builder!
    CDataFileBuilder(string rootDir);
    
    // desctructor
    ~CDataFileBuilder();

    // write a tile
    void WriteTile(STileOutput *pTileOutput);
};

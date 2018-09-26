#include "CDataFileBuilder.h"
#include "CMetaFile.h"
#include "..\Common\CHelper.h"
#include "..\Common\CImg.h"
#include "..\..\CxImage\ximage.h"

CDataFileBuilder::CDataFileBuilder(string rootDir)
    :CSemaphore(1, 1)
{
    // initialize members
    _pDataFile = NULL;
    _pMetaFile = NULL;
    _nextFileIndex = 0;
    _rootDir = rootDir;

    // create dir
    CHelper::CreateDir(_rootDir);
}

CDataFileBuilder::~CDataFileBuilder()
{
    if (_pDataFile != NULL)
        delete _pDataFile;
    if (_pMetaFile != NULL)
        delete _pMetaFile;
}

void CDataFileBuilder::WriteTile(STileOutput *pTileOutput)
{
    // if imgLength is longer than allowed, return error
    if (pTileOutput->metaInfo.imgLength >= MAX_IMG_LEN)
        LogThrow("CDataFileBuilder: imgLength should not be longer than MAX_IMG_LEN");

    // the following codes shall be locked for multi-thread safety.
    Lock();

    // if _pDataFile==NULL, a new set of data file and meta file is to be created.
    if (_pDataFile == NULL)
    {
        // prepare file name
        char dataFileName[256];
        char metaFileName[256];
        sprintf(dataFileName, "%s\\%d%s", _rootDir.c_str(), _nextFileIndex, DATA_EXT_NAME);
        sprintf(metaFileName, "%s\\%d%s", _rootDir.c_str(), _nextFileIndex, META_EXT_NAME);

        // increase file index
        _nextFileIndex++;

        // create file
        _pDataFile = new CFile(dataFileName, true, 2);
        _pMetaFile = new CMetaFile(metaFileName, true);
    }

    // fill in STileMetaInfo and write it to file
    pTileOutput->metaInfo.imgOffset = (int)_pDataFile->Tell();
    _pDataFile->Write(((CxMemFile*)pTileOutput->pMemFile)->GetBuffer(false), pTileOutput->metaInfo.imgLength);
    _pMetaFile->Write(&pTileOutput->metaInfo);

    // if data file is to long, close this file and create a new one
    if (_pDataFile->Tell() > MAX_DATA_FILE_LEN)
    {
        delete _pDataFile, _pDataFile = NULL;
        delete _pMetaFile, _pMetaFile = NULL;
    }
    
    Unlock();
}

#include "CMetaFile.h"

CMetaFile::CMetaFile(string filePath, bool isWriteMode) : CFile(filePath, isWriteMode)
{
    // init params
    metaRecordCnt = 0;

    // if is write mode, write header
    if (isWriteMode)
        WriteStrict(&metaRecordCnt, sizeof(int));
    // if is read mode, read header
    else
        ReadStrict(&metaRecordCnt, sizeof(int));
}

CMetaFile::~CMetaFile()
{
    if (isWriteMode)
    {
        // write meta record count to the very beginning of the meta file
        SeekStrict(0, SEEK_SET);
        WriteStrict(&metaRecordCnt, sizeof(int));
    }
}

void CMetaFile::Write(const STileMetaInfo *pTileMetaInfo)
{
    // write record to meta file
    WriteStrict(pTileMetaInfo, sizeof(STileMetaInfo));

    // increate meta record count
    metaRecordCnt++;
}

void CMetaFile::Read(int recordIndex, STileMetaInfo *pTileMetaInfo)
{
    // check if recordIndex is valid
    if (recordIndex >= metaRecordCnt)
        LogThrow("CMetaFile: reading index is greater than record count");

    // seek to the offset
    SeekStrict(headerLength + recordIndex * sizeof(STileMetaInfo), SEEK_SET);
    ReadStrict(pTileMetaInfo, sizeof(STileMetaInfo));
}

int CMetaFile::GetRecordCnt()
{
    return metaRecordCnt;
}

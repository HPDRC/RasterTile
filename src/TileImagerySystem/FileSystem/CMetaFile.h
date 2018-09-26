#include "includes.h"
#include "..\Common\CFile.h"

class CMetaFile : CFile
{
private:
    // how many records in this file
    int metaRecordCnt;
    
    // meta file header format: int a
    // a: indicate how many meta records are in this file
    static const int headerLength = 4;

public:
    // create a new CMetaFile object, specify if is write mode or read mode
    // if isWriteMode == false, extraInfoLength is not necessary
    CMetaFile(string filePath, bool isWriteMode);
    ~CMetaFile();

    // write a new record to this meta file
    // pMetaRecord: meta record
    // pExtraInfo: extra info appended after meta record
    void Write(const STileMetaInfo *pTileMetaInfo);

    // read a record in this meta file
    void Read(int recordIndex, STileMetaInfo *pTileMetaInfo);

    // get the count of records
    int GetRecordCnt();
};
#pragma once
#include "global_includes.h"

// basic operation of files
class CFile
{
private:
    int fileHandle;

protected:
    bool isWriteMode;

public:
    // will open the file, optimization: 0/none, 1/random access, 2/sequential access
    CFile(string filePath, bool isWriteMode, int optimization = 0);
    // will close the file
    ~CFile();

    S64 Seek(S64 position, int origin);
    S64 Tell();
    int Read(void* destBuff, U32 readLength);
    int Write(const void* srcBuff, U32 writeLength);

    // seek to position, if failed, throw an exception
    void SeekStrict(S64 position, int origin);

    // read strictly readLength bytes, if not, throw an exception
    void ReadStrict(void* destBuff, U32 readLength);

    // write strictly writeLength bytes, if not, throw an exception
    void WriteStrict(const void* srcBuff, U32 writeLength);

    S64 GetSize();

	static S64 GetSize(string filePath);
};

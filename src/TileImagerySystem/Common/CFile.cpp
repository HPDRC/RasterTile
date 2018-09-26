#include "CFile.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <windows.h>

CFile::CFile(string filePath, bool isWriteMode, int optimization)
{
    this->isWriteMode = isWriteMode;
    int openMode = isWriteMode ? (_O_BINARY | _O_WRONLY | _O_CREAT) : (_O_BINARY | _O_RDONLY);
    if (optimization == 1)
        openMode |= O_RANDOM;
    else if (optimization == 2)
        openMode |= _O_SEQUENTIAL;
    fileHandle = _open(filePath.c_str(), openMode, _S_IWRITE);
    if (fileHandle == -1)
        LogThrow("CFile: open %s failed. errno: %d", filePath.c_str(), errno);
}

CFile::~CFile()
{
    _close(fileHandle);
}

S64 CFile::Seek(S64 position, int origin)
{
    return _lseeki64(fileHandle, position, origin);
}

int CFile::Read(void* destBuff, U32 readLength)
{
    // check if is mode valid
    if (isWriteMode)
        LogThrow("CFile: Can't call Read function under write mode");

    int result = _read(fileHandle, destBuff, readLength);
    if (result <= 0)
        LogThrow("CFile: Read error");
    return result;
}

int CFile::Write(const void* srcBuff, U32 writeLength)
{
    // check if is mode valid
    if (!isWriteMode)
        LogThrow("CFile: Can't call Write function under read mode");

    return _write(fileHandle, srcBuff, writeLength);
}

S64 CFile::Tell()
{
    return _telli64(fileHandle);
}

void CFile::SeekStrict(S64 position, int origin)
{
    if (Seek(position, origin) == -1)
        LogThrow("CFile: seek failed");
}


void CFile::ReadStrict(void* destBuff, U32 readLength)
{
    U32 len = Read(destBuff, readLength);
    if (len != readLength)
        LogThrow("CFile: read incomplete");
}

void CFile::WriteStrict(const void* srcBuff, U32 writeLength)
{
    if (Write(srcBuff, writeLength) != writeLength)
        LogThrow("CFile: write incomplete");
}

S64 CFile::GetSize()
{
    SeekStrict(0, SEEK_END);
    return Tell();
}

S64 CFile::GetSize(string filePath)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if (!GetFileAttributesEx(filePath.c_str(), GetFileExInfoStandard, &fileInfo))
		LogThrow("CFile.GetSize: call GetFileAttributesEx failed");
	S64 result = fileInfo.nFileSizeHigh;
	result <<= 32;
	result |= fileInfo.nFileSizeLow;
	return result;
}

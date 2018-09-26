#include "CSemaphore.h"
#include <Windows.h>

CSemaphore::CSemaphore(int initialCnt, int maxCnt)
{
    handle = CreateSemaphore(NULL, initialCnt, maxCnt, NULL);
}

CSemaphore::~CSemaphore()
{
    CloseHandle(handle);
}

void CSemaphore::Lock()
{
    WaitForSingleObject(handle, INFINITE);
}

void CSemaphore::Unlock(int cnt)
{
    ReleaseSemaphore(handle, cnt, NULL);
}
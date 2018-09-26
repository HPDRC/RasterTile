#pragma once
#include "global_includes.h"

class CSemaphore
{
private:
    void *handle;

public:
    CSemaphore(int initialCnt, int maxCnt);
    ~CSemaphore();

    void Lock();
    void Unlock(int cnt=1);

    inline void Wait() { Lock(); }
    inline void Release(int cnt=1) { Unlock(cnt); }
};
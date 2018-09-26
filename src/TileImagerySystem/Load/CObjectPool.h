#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

class CObjectPool : CSemaphore
{
private:
    vector<void*> pool;
    CLoader *pLoader;
    double singleObjectSize;
    int maxObjectInPool;
    int minObjectInPool;
    void* (*funcNew)(CLoader *pLoader);
    void (*funcDelete)(CLoader *pLoader, void *pObject);
    void (*funcReset)(void *pObject);


public:
    CObjectPool(CLoader *pLoader, double singleObjectSize, int maxObjectInPool,
        void* (*funcNew)(CLoader *pLoader), void (*funcDelete)(CLoader *pLoader, void *pObject), void (*funcReset)(void *pObject));
    ~CObjectPool();

    void* FetchObject(bool ignoreMemoryLimit);
    void ReturnObject(void *pObject);
};
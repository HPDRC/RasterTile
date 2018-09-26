#include "CObjectPool.h"
#include "CLoader.h"
#include "..\Common\CMemory.h"

CObjectPool::CObjectPool(CLoader *pLoader, double singleObjectSize, int maxObjectInPool,
        void* (*funcNew)(CLoader *pLoader), void (*funcDelete)(CLoader *pLoader, void *pObject), void (*funcReset)(void *pObject))
        : CSemaphore(1, 1)
{
    this->pLoader = pLoader;
    this->singleObjectSize = singleObjectSize;
    this->maxObjectInPool = maxObjectInPool;
    this->minObjectInPool = maxObjectInPool / 2;
    this->funcNew = funcNew;
    this->funcDelete = funcDelete;
    this->funcReset = funcReset;
}

CObjectPool::~CObjectPool()
{
    for (vector<void*>::iterator it=pool.begin(); it!=pool.end(); it++)
    {
        funcDelete(pLoader, *it);
        pLoader->pMemTile->Release(singleObjectSize);
    }
}

void* CObjectPool::FetchObject(bool ignoreMemoryLimit)
{
    void *pObject = NULL;
    Lock();

    // if no object in pool, return a new object
    if (pool.size() == 0)
    {
        Unlock();
        pLoader->pMemTile->Allocate(singleObjectSize, ignoreMemoryLimit);
        pObject = funcNew(pLoader);
    }
    // else, return an img in the pool
    else
    {
        pObject = pool.back();
        pool.pop_back();
        Unlock();
    }

    // reset object and return
    funcReset(pObject);
    return pObject;
}

void CObjectPool::ReturnObject(void *pObject)
{
    if (pObject == NULL) return;
    
    Lock();
    pool.push_back(pObject);

    if (pool.size() > maxObjectInPool)
    {
        int orgSize = (int)pool.size();
        for (int i=0; i<minObjectInPool; i++)
        {
            funcDelete(pLoader, pool[i]);
            pLoader->pMemTile->Release(singleObjectSize);
        }
        pool.erase(pool.begin(), pool.begin()+minObjectInPool);
        Log("pool shrink by %d", orgSize-pool.size());
    }
    Unlock();
}
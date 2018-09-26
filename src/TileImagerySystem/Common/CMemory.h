#pragma once
#include "global_includes.h"
#include "CSemaphore.h"

// static class, control memory usage
class CMemory : CSemaphore
{
public:
	double limitBytes;

private:
	double usedBytes;
    int threadCntBlocking;       // how many threads are waiting for memory

public:
	CMemory(double limitBytes);
	
	void Allocate(double bytes, bool ignoreMemoryLimit = false); // call this function before allocating memory, will be blocked if memory limit exceeded
	void Release(double bytes);  // call this function after releasing memory
	double GetUsedBytes();
	int GetThreadCntBlocking();
};

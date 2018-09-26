#include "CMemory.h"
#include "CSemaphore.h"
#include "CConfig.h"
#include <Windows.h>

CMemory::CMemory(double limitBytes) : CSemaphore(1, 1)
{
	this->limitBytes = limitBytes;
	this->threadCntBlocking = 0;
	this->usedBytes = 0;
}
void CMemory::Allocate(double bytes, bool ignoreMemoryLimit)
{
	if (ignoreMemoryLimit)
		Release(-bytes);
	else
	{
		bool waiting = false;
		while (true)
		{
			if (GetUsedBytes() + bytes > limitBytes)
			{
				if (waiting == false)
				{
					waiting = true;
					Lock(), threadCntBlocking++, Unlock();
				}
				Sleep(3000);
			}
			else
				break;
		}
		Release(-bytes);
		if (waiting)
			Lock(), threadCntBlocking--, Unlock();
	}
}

void CMemory::Release(double bytes)
{
	Lock(), usedBytes -= bytes, Unlock();
}

double CMemory::GetUsedBytes()
{
	double result = 0;
	Lock(), result = usedBytes, Unlock();
	return result;
}

int CMemory::GetThreadCntBlocking()
{
	int result = 0;
	Lock(), result = threadCntBlocking, Unlock();
	return result;
}


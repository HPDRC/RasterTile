#include "CLoader.h"
#include "CTaskSet.h"

CTaskSet::CTaskSet(CLoader *pLoader) : CSemaphore(1, 1), semQueue(0, INT32_MAX)
{
    this->pLoader = pLoader;
    this->totalAddedTaskCnt = 0;
    this->totalFinishedTaskCnt = 0;
    this->isClosed = false;
}

CTaskSet::~CTaskSet()
{
    Close();
}

void CTaskSet::PushBack(STask task)
{
	Lock();
    if (!isClosed)
        taskQueue.push_back(task), totalAddedTaskCnt++;
    Unlock();
    semQueue.Release();
}

STask CTaskSet::PopFront()
{
    STask task;
    Lock();
    if (!isClosed)
        task = taskQueue.front(), taskQueue.pop_front();
    Unlock();
    return task;
}


int CTaskSet::Length()
{
    int n = 0;
    Lock(), n = (int)taskQueue.size(), Unlock();
    return n;
}

bool CTaskSet::RunFrontTaskBlock()
{
    // wait until queue is not empty
    semQueue.Wait();

    Lock();
    
    // if task set is closed, or there is no task any more, return false
    if (isClosed || taskQueue.empty())
    {
        Unlock();
        return false;
    }

    STask task = taskQueue.front();
    taskQueue.pop_front();

    Unlock();
    
    // run the task
    if (task.pFunc != NULL)
        task.pFunc(task.pSource, task.pNode);
    else
        pLoader->OutputTile(task.tileOutput);

    // update statistics
    Lock(), totalFinishedTaskCnt++, Unlock();
	return true;
}

void CTaskSet::Close()
{
    // set as closed
    Lock();
    isClosed = true;
    taskQueue.clear();
    Unlock();

    // release sems so that threads waiting on Pop() functions can return
    semQueue.Release(INT32_MAX);
}

void CTaskSet::Run()
{
    while (true)
	{
		// run the front task. and if all tasks are done
		if (!RunFrontTaskBlock())
            break;
	}
}
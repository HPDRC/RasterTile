#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

// maintain a task list, multi-thread safe
class CTaskSet : CSemaphore
{
private:
    list<STask> taskQueue;    // store all tasks. key: id, value: pTask
    CSemaphore semQueue;        // this semaphore controls the queue
    bool isClosed;              // when this value == true, this task set is closed

    // pop the front task and run it, will block if task queue is empty
	// return false when this taskset is closed
	bool RunFrontTaskBlock();

public:
    CLoader *pLoader;
    int totalAddedTaskCnt;      // the total count of tasks have ever been added into this task set
    int totalFinishedTaskCnt;   // the total count of tasks have ever been finished in this task set

public:
    CTaskSet(CLoader *pLoader);
    ~CTaskSet();
    
    void PushBack(STask task);        // push a task from the end of the queue
    STask PopFront();                  // pop the front task, will return NULL if queue is empty
    
	int Length();   // get the count of tasks
    void Close();   // close this task set and no more task can be added or fetched
    void Run();     // run tasks in this set until there's no more task to run
};

#pragma once
#include "..\Common\global_includes.h"

// abstract class, base class for tasks
struct STask
{
	// pointer to the functions to be run in this task
	void (*pFunc)(CSource*, CPyramidNode*);
    CSource* pSource;
	CPyramidNode* pNode;
	STileOutput tileOutput;

    STask()
    {
        this->pFunc = NULL;
	    this->pSource = NULL;
	    this->pNode = NULL;
    }

	STask(void (*pFunc)(CSource*, CPyramidNode*), CSource* pSource, CPyramidNode* pNode)
    {
	    this->pFunc = pFunc;
	    this->pSource = pSource;
	    this->pNode = pNode;
    }

    STask(STileOutput tileOutput)
    {
	    this->pFunc = NULL;
	    this->pSource = NULL;
	    this->pNode = NULL;
	    this->tileOutput = tileOutput;
    }
};

#define MEMFILE_POOL_CAP    (50000)
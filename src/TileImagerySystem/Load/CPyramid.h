#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

struct SNodeMeta
{
    CPyramidNode *pNode;
    int dependencyCount;    // dependency count are temporarily stored here until the real node is created.
    int status; // 0: not created, 1: created, 2: deleted
};

class CPyramid : CSemaphore
{
private:
    // pyramid must has its loader
	CLoader *pLoader;

public:
    // store all nodes and its id
    map<STileXY, SNodeMeta> nodes;

public:
    CPyramid(CLoader *pLoader);
    ~CPyramid();

	// add a new node if not exist, all the father nodes will be also created
    // will increase the node's dependency by 1
    void AddNode(const STileXY &tilexy);

    CPyramidNode* GetNode(const STileXY &tilexy);

    void DeleteNode(const STileXY &tilexy);

    // check if dependency count matches
    void CheckDependencyCount(int dependencyCntFromSourceToNode);
};

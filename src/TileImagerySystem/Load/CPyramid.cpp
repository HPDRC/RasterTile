#include "CPyramid.h"
#include "CPyramidNode.h"
#include "CTaskSet.h"
#include "CLoader.h"
#include "CObjectPool.h"

CPyramid::CPyramid(CLoader *pLoader)
    : CSemaphore(1, 1)
{
	this->pLoader = pLoader;
}

CPyramid::~CPyramid(void)
{
    int remainCount = 0;
    for (map<STileXY, SNodeMeta>::iterator it=nodes.begin(); it!=nodes.end(); it++)
    {
        if (it->second.status != 2)
        {
            remainCount++;
            delete it->second.pNode;
        }
    }
    if (remainCount != 0)
        Log("!!!Warning!!! CPyramid has %d remaining nodes", remainCount);
}

void CPyramid::AddNode(const STileXY &tilexy)
{
    // if this node already exists, return it
    map<STileXY, SNodeMeta>::iterator it = nodes.find(tilexy);
    if (it != nodes.end())
    {
        it->second.dependencyCount++;
        return;
    }
    
    // create the node meta
    SNodeMeta nodeMeta;
    nodeMeta.pNode = NULL, nodeMeta.dependencyCount = 1, nodeMeta.status = 0;
    nodes[tilexy] = nodeMeta;
    
	// if reach the top, stop
    if (tilexy.level == TOP_TILE_LEVEL)
        return;

    // create its dad node
    AddNode(tilexy.Father());
}

CPyramidNode* CPyramid::GetNode(const STileXY &tilexy)
{
    CPyramidNode *pNode = NULL;
    Lock();
    map<STileXY, SNodeMeta>::iterator it = nodes.find(tilexy);
    if (it == nodes.end())
        LogThrow("CPyramid.GetNode: node not exist at x,y,z: %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    if (it->second.status == 0)
    {
        pNode = it->second.pNode = (CPyramidNode*)pLoader->pPoolPyramidNode->FetchObject(true);
        pNode->Reset(it->first, it->second.dependencyCount);
        it->second.status = 1;
    }
    else if (it->second.status == 1)
        pNode = it->second.pNode;
    else if (it->second.status == 2)
        LogThrow("CPyramid.GetNode: try to get a deleted node at x,y,z: %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    Unlock();
    return pNode;
}

void CPyramid::DeleteNode(const STileXY &tilexy)
{
    Lock();
    map<STileXY, SNodeMeta>::iterator it = nodes.find(tilexy);
    if (it == nodes.end())
        LogThrow("CPyramid.DeleteNode: node not exist at x,y,z: %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    if (it->second.status == 0)
        LogThrow("CPyramid.DeleteNode: try to delete a un-created node at x,y,z: %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    else if (it->second.status == 1)
    {
        pLoader->pPoolPyramidNode->ReturnObject(it->second.pNode);
        it->second.pNode = NULL;
        it->second.status = 2;
    }
    else if (it->second.status == 2)
        LogThrow("CPyramid.DeleteNode: try to delete a already deleted node at x,y,z: %d,%d,%d", tilexy.x, tilexy.y, (int)tilexy.level);
    Unlock();
}

void CPyramid::CheckDependencyCount(int dependencyCntFromSourceToNode)
{
    int sum = 0, n = (int)nodes.size();
    for (map<STileXY, SNodeMeta>::iterator it=nodes.begin(); it!=nodes.end(); it++)
    {
        if (it->first.level == TOP_TILE_LEVEL)
            sum++;
        sum += it->second.dependencyCount;
    }

    Log("CPyramid: dependency count = %d", sum);
    Log("CPyramid: node count = %d", n);

    // check if matches
    if (sum != n+dependencyCntFromSourceToNode)
        LogThrow("CPyramid: dependency count %d doesn't match with node count %d", sum+1, n+dependencyCntFromSourceToNode);
}

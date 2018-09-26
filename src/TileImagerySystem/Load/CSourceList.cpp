#include "CSourceList.h"
#include "CSource.h"
#include "CPyramid.h"
#include "..\Common\CConfig.h"
#include "..\Common\CMemory.h"

CSourceList::~CSourceList()
{
    int n = (int)this->size();
    for (int i=0; i<n; i++)
        delete this->at(i);
}

void CSourceList::AddSource(CSource* pSource)
{
    this->push_back(pSource);
}

void CSourceList::DeleteSource(CSource* pSource)
{
    int n = (int)this->size();
    for (int i=0; i<n; i++)
    {
        if (this->at(i) != pSource) continue;
        delete pSource;
        this->at(i) = NULL;
        return;
    }
    LogThrow("CSourceList.DeleteSource: pSource does not exist!");
}


bool CompareSource1(CSource *a, CSource *b)
{
    int ax, bx, ay, by;
    ax = a->tilexmin, ay = a->tileymin, bx = b->tilexmin, by = b->tileymin;
    if (a->loadLevel > b->loadLevel)
    {
        int diff = a->loadLevel - b->loadLevel;
        bx = bx<<diff;
        by = by<<diff;
    }
    else if (a->loadLevel < b->loadLevel)
    {
        int diff = b->loadLevel - a->loadLevel;
        ax = ax<<diff;
        ay = ay<<diff;
    }
    if (ax < bx)
        return true;
    else if (ax > bx)
        return false;
    else
        return ay < by;
}

bool CompareSource2(CSource *a, CSource *b)
{
    int ax, bx, ay, by;
    ax = a->tilexmin, ay = a->tileymin, bx = b->tilexmin, by = b->tileymin;
    if (a->loadLevel > b->loadLevel)
    {
        int diff = a->loadLevel - b->loadLevel;
        bx = bx<<diff;
        by = by<<diff;
    }
    else if (a->loadLevel < b->loadLevel)
    {
        int diff = b->loadLevel - a->loadLevel;
        ax = ax<<diff;
        ay = ay<<diff;
    }

    if (ay < by)
        return true;
    else if (ay > by)
        return false;
    else
        return ax < bx;
}

int CSourceList::CalcMaxNodeInMemory(map<STileXY, SNodeMeta> &nodes)
{
    int n = (int)this->size();

    // simulate loading process and calc max nodes in memory
    map<STileXY, SNodeMeta>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++)
        it->second.status = - it->second.dependencyCount;

    int nodeInMemory = 0, maxNodeInMemory = 0;
    for (int i=0; i<n; i++)
    {
        // a source starts to load, all its tiles are loaded
        vector<STileXY> *pList = &this->at(i)->pyramidNodesList;
        for (int j=0; j<pList->size(); j++)
        {
            STileXY tilexy = pList->at(j);
            while (true)
            {
                // find node
                it = nodes.find(tilexy);
                if (it == nodes.end())
                    LogThrow("Calc max nodes in memory: node not found!");

                // put node in memory
                if (it->second.status < 0)
                {
                    it->second.status = - it->second.status;
                    nodeInMemory++;
                    maxNodeInMemory = max(maxNodeInMemory, nodeInMemory);
                }

                // load finish, decrease link
                it->second.status--;

                // once node completes, remove from memory
                if (it->second.status == 0)
                {
                    nodeInMemory--;
                    // get its father and load
                    if (tilexy.level != TOP_TILE_LEVEL)
                        tilexy = it->first.Father();
                    else
                        break;
                }
                else
                    break;
            }
        }
    }

    // clear status
    for (it = nodes.begin(); it != nodes.end(); it++)
        it->second.status = 0;

    return maxNodeInMemory;
}

int CSourceList::CreatePreLoadTasks(map<STileXY, SNodeMeta> &nodes)
{
    int n = (int)this->size();

    sort(this->begin(), this->end(), CompareSource1);
    int cnt1 = CalcMaxNodeInMemory(nodes);
    Log("Max nodes in memory by sort1 would be: %d", cnt1);

    sort(this->begin(), this->end(), CompareSource2);
    int cnt2 = CalcMaxNodeInMemory(nodes);
    Log("Max nodes in memory by sort2 would be: %d", cnt2);

    if (cnt1 < cnt2)
        sort(this->begin(), this->end(), CompareSource1);
    
    // create tasks
    for (int i=0; i<n; i++)
    {
        this->at(i)->CreatePreloadTask();
    }
    return min(cnt1, cnt2);
}

int CSourceList::GetNodeLinkCnt()
{
    int n = (int)size(), sum = 0;
    for (int i=0; i<n; i++)
        sum += this->at(i)->ungeneratedTileCnt;
    return sum;
}

void CSourceList::AdjustMemoryLimit(CMemory *pMemSource, CMemory *pMemTile)
{
    int n = (int)this->size();
    double sizeMax = 0, sizeAvg = 0, sizeTotal = 0;

    for (int i=0; i<n; i++)
    {
        int size = this->at(i)->sourceMetaInfo.width * this->at(i)->sourceMetaInfo.height;
        if (size > sizeMax)
            sizeMax = size;
        sizeTotal += size;
    }
    sizeMax *= BYTE_PER_PIXEL + 1;
    sizeAvg = sizeTotal / n * (BYTE_PER_PIXEL + 1);

    double newLimit = pMemSource->limitBytes / 4; // the new source limit should be at least 1/4 of the original
    newLimit = max(newLimit, sizeMax * 1.2); // should be at least able to load the largest source
    newLimit = max(newLimit, sizeAvg * 8); // should be able to load 8 sources by average
    newLimit = min(newLimit, pMemSource->limitBytes); // should not exceed original limit

    // apply the new limit
    double excessBytes = pMemSource->limitBytes - newLimit;
    pMemSource->limitBytes -= excessBytes;
    pMemTile->limitBytes += excessBytes;
    Log("Memory limits for source and tile are adjusted to: %dMB, %dMB", (int)(pMemSource->limitBytes / 1024 / 1024), (int)(pMemTile->limitBytes / 1024 / 1024));
}
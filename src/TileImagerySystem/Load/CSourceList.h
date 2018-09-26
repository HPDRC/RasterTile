#pragma once
#include "includes.h"
#include "CPyramid.h"

// manage a list of sources
class CSourceList : public vector<CSource*>
{
public:
    ~CSourceList();

    void AddSource(CSource* pSource);

    void DeleteSource(CSource* pSource);

    int CalcMaxNodeInMemory(map<STileXY, SNodeMeta> &nodes);

    int CreatePreLoadTasks(map<STileXY, SNodeMeta> &nodes);

    int GetNodeLinkCnt();

    void AdjustMemoryLimit(CMemory *pMemSource, CMemory *pMemTile);
};

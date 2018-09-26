#include "CDBCache.h"
#include "..\FileSystem\CSqlConnection.h"
#include "..\Common\CSemaphore.h"
#include "CWeightRule.h"

CSemaphore CDBCache::sem(1,1);
map<string, CDBCache*> CDBCache::dbCaches;
map<string, projPJ> CDBCache::projCaches;

CDBCache* CDBCache::GetDBCache(string connString, CSqlConnection *pConn)
{
    map<string, CDBCache*>::iterator it = dbCaches.find(connString);
    if (it != dbCaches.end())
        return it->second;
    
    sem.Lock();
    // check again in case other thread has already added the element
    it = dbCaches.find(connString);
    if (it != dbCaches.end())
    {
        sem.Unlock();
        return it->second;
    }
    CDBCache *pCache = new CDBCache(pConn);
    dbCaches[connString] = pCache;
    sem.Unlock();
    return pCache;
}

projPJ CDBCache::GetProjCache(string projString)
{
    map<string, projPJ>::iterator it = projCaches.find(projString);
    if (it != projCaches.end())
        return it->second;
    
    sem.Lock();
    // check again in case other thread has already added the element
    it = projCaches.find(projString);
    if (it != projCaches.end())
    {
        sem.Unlock();
        return it->second;
    }
    projPJ proj = pj_init_plus(projString.c_str());
    projCaches[projString] = proj;
    sem.Unlock();
    return proj;
}

CDBCache::CDBCache(CSqlConnection *pConn)
{
    // cache weight rules table
    pConn->ResetStmt();
    pConn->ExecDirect("select origin_id, z_min, z_max, weight from weight_rules");
    while (pConn->Fetch())
    {
        CWeightRule rule;
        rule.originId = pConn->GetInt(1);
        rule.zmin = pConn->GetU8(2);
        rule.zmax = pConn->GetU8(3);
        rule.weight = pConn->GetInt(4);
        weightRuleList.push_back(rule);
    }

    // cache pathes table
    pConn->ResetStmt();
    pConn->ExecDirect("select id, path from pathes");
    while (pConn->Fetch())
    {
        int id = pConn->GetInt(1);
        char *path = new char[MAX_PATH_LEN];
        pConn->GetString(path, MAX_PATH_LEN, 2);
        pathList[id] = path;
    }

    // cache origins table
    pConn->ResetStmt();
    pConn->ExecDirect("select id, name from origins");
    while (pConn->Fetch())
    {
        int id = pConn->GetInt(1);
        char *name = new char[MAX_ORIGIN_NAME_LEN];
        pConn->GetString(name, MAX_ORIGIN_NAME_LEN, 2);
        originList[id] = name;
        originNameToId[name] = id;
    }
    originList[0] = "BEST_AVAILABLE";
    originNameToId["BEST_AVAILABLE"] = 0;

    Log("CDBCache: init success. %d pathes are cached", pathList.size());
}

int CDBCache::GetWeight(int originId, U8 z)
{
    for (int i=0; i<weightRuleList.size(); i++)
        if (weightRuleList[i].IsApply(originId, z))
            return weightRuleList[i].weight;
    return 0;
}


char * CDBCache::GetPath(int pathId)
{
    map<int, char*>::iterator it = pathList.find(pathId);
    if (it == pathList.end())
        return NULL;
    else
        return it->second;
}

char * CDBCache::GetOriginName(int originId)
{
    map<int, char*>::iterator it = originList.find(originId);
    if (it == originList.end())
        return NULL;
    else
        return it->second;
}

int CDBCache::GetOriginId(char * originName)
{
    if (originName == NULL) return 0;
    map<string, int>::iterator it = originNameToId.find(originName);
    if (it == originNameToId.end())
        return 0;
    else
        return it->second;
}

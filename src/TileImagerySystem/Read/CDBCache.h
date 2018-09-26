#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"
#include "..\..\Proj4\proj_api.h"

class CDBCache
{
/////////////////// static part //////////////////////
private:
    static map<string, CDBCache*> dbCaches;
    static map<string, projPJ> projCaches;
    static CSemaphore sem;

public:
    static CDBCache* GetDBCache(string connString, CSqlConnection *pConn);
    static projPJ GetProjCache(string projString);

/////////////////// non-static part ///////////////////
private:
    vector<CWeightRule> weightRuleList;
    map<int, char*> pathList;
    map<int, char*> originList;
    map<string, int> originNameToId;

public:
    // initialize cache, will pull data from database
    CDBCache(CSqlConnection *pConn);

    // get weight by originId and z
    int GetWeight(int originId, U8 z);

    // get path by id, return NULL if not found
    char * GetPath(int pathId);

    // get origin name by id, return NULL if not found
    char * GetOriginName(int originId);

    // get origin id by name, return 0 (best_availabe origin) if not found
    int GetOriginId(char * originName);
};
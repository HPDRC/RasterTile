#pragma once
#include "includes.h"

class CTablePathes
{
public:
    // add a path into table, do nothing if already exists
    static void AddPath(CSqlConnection& conn, const char path[]);

    // get a path by its id
    static void GetPathById(CSqlConnection& conn, int pathId, char *path);

    // get pathId by path
    static int GetIdByPath(CSqlConnection& conn, const char *path);
};
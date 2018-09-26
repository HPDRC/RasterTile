#include "CTablePathes.h"
#include "CSqlConnection.h"

void CTablePathes::AddPath(CSqlConnection& conn, const char path[])
{
    conn.BindString(path);
    conn.ExecDirect_SupressError("insert into pathes(path) values(?)");
}

void CTablePathes::GetPathById(CSqlConnection& conn, int pathId, char *path)
{
    conn.BindInt(&pathId);
    conn.ExecDirect("select path from pathes where id=?");
    if (!conn.Fetch())
        LogThrow("Table pathes: specified id has no path");
    conn.GetString(path, MAX_PATH_LEN);
}

int CTablePathes::GetIdByPath(CSqlConnection& conn, const char *path)
{
    conn.BindString(path);
    conn.ExecDirect("select top 1 id from pathes where path=?");
    if (!conn.Fetch())
        LogThrow("Table pathes: specified path has no id");
    return conn.GetInt();
}

#include "CTableQualities.h"
#include "CSqlConnection.h"

int CTableQualities::GetGridCompleteness(CSqlConnection& conn, S64 tileId)
{
    conn.BindBigInt(&tileId, 1);
    conn.ExecDirect("select top 1 grid_completeness from qualities where tile_id=?");
    if (!conn.Fetch()) return -1;
    return conn.GetInt();
}

void CTableQualities::SetGridCompleteness(CSqlConnection& conn, S64 tileId, int gridCompleteness)
{
    conn.BindBigInt(&tileId, 1);
    conn.BindInt(&gridCompleteness, 2);
    conn.ExecDirect_SupressError("insert into qualities(tile_id, grid_completeness) values(?, ?)");
}

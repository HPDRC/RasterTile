#include "CTableTiles.h"
#include "CSqlConnection.h"
#include "..\Read\CTile.h"

void CTableTiles::AddTilePrepare(CSqlConnection& conn)
{
    conn.PrepareStmt("insert into tiles(path_id, x, y, z, img_offset, img_length, date, origin_id, zone) values(?,?,?,?,?,?,?,?,?)");
}

// add tile
void CTableTiles::AddTile(CSqlConnection& conn, int pathId, STileMetaInfo *pTileMetaInfo)
{
    conn.ResetStmtParams();
    conn.BindInt(&pathId, 1);
    conn.BindInt(&(pTileMetaInfo->tilexy.x), 2);
    conn.BindInt(&(pTileMetaInfo->tilexy.y), 3);
    conn.BindTinyInt(&pTileMetaInfo->tilexy.level, 4);
    conn.BindInt(&pTileMetaInfo->imgOffset, 5);
    conn.BindInt(&pTileMetaInfo->imgLength, 6);
    int date = pTileMetaInfo->date.ToInt();
    conn.BindInt(&date, 7);
    conn.BindInt(&pTileMetaInfo->originId, 8);
    conn.BindInt(&pTileMetaInfo->zone, 9);
    conn.ExecPrepared();
}

void CTableTiles::AddTiles(CSqlConnection& conn, int pathId, STileMetaInfo tileMetaInfos[], int start, int end)
{
    conn.ResetStmt();
    string sql = "insert into tiles(path_id, x, y, z, img_offset, img_length, date, origin_id, zone)";
    char tmp[1024];
    for (int i=start; i<=end; i++)
    {
        sprintf(tmp, i==start ? " select %d,%d,%d,%d,%d,%d,%d,%d,%d" : " union select %d,%d,%d,%d,%d,%d,%d,%d,%d",
            pathId, tileMetaInfos[i].tilexy.x, tileMetaInfos[i].tilexy.y,
            (int)tileMetaInfos[i].tilexy.level, tileMetaInfos[i].imgOffset, tileMetaInfos[i].imgLength,
            tileMetaInfos[i].date.ToInt(), tileMetaInfos[i].originId, tileMetaInfos[i].zone);
        sql += tmp;
    }
    conn.ExecDirect(sql.c_str());
}

void CTableTiles::GetTiles(CSqlConnection& conn, STileXY& tilexy, int zone, int originId, int date, vector<CTile*>* pTileList, CDBCache *pCache)
{
    conn.ResetStmt();
    conn.BindInt(&tilexy.x, 1);
    conn.BindInt(&tilexy.y, 2);
    conn.BindTinyInt(&tilexy.level, 3);
    conn.BindInt(&zone, 4);
    if (originId == 0 && date == 0)
        conn.ExecDirect("select top 1000 id, path_id, img_offset, img_length, date, origin_id, grid_completeness from tiles left join qualities on tiles.id = qualities.tile_id where x=? and y=? and z=? and zone=?");
    else if (date == 0)
    {
        conn.BindInt(&originId, 5);
        conn.ExecDirect("select top 1000 id, path_id, img_offset, img_length, date, origin_id, grid_completeness from tiles left join qualities on tiles.id = qualities.tile_id where x=? and y=? and z=? and zone=? and origin_id=?");
    }
    else
    {
        conn.BindInt(&originId, 5);
        conn.BindInt(&date, 6);
        conn.ExecDirect("select top 1000 id, path_id, img_offset, img_length, date, origin_id, grid_completeness from tiles left join qualities on tiles.id = qualities.tile_id where x=? and y=? and z=? and zone=? and origin_id=? and date=?");
    }
    while (conn.Fetch())
    {
        CTile *pTile = new CTile(pCache);
        pTile->tilexy = tilexy;
        pTile->id = conn.GetS64(1);
        pTile->pathId = conn.GetInt(2);
        pTile->imgOffset = conn.GetInt(3);
        pTile->imgLength = conn.GetInt(4);
        pTile->date = conn.GetInt(5);
        pTile->originId = conn.GetInt(6);
        pTile->gridCompleteness = conn.GetInt(7);
        if (conn.isLastGetSuccess)
            pTile->isGridCompletenessLoaded = true;
        else
            pTile->gridCompleteness = 0;
        pTileList->push_back(pTile);
    }
}

void CTableTiles::GetOrigins(CSqlConnection& conn, STileXY& tilexy, int zone, vector<SOriginAndDate>* pOriginList)
{
    conn.ResetStmt();
    conn.BindInt(&tilexy.x, 1);
    conn.BindInt(&tilexy.y, 2);
    conn.BindTinyInt(&tilexy.level, 3);
    conn.BindInt(&zone, 4);
    conn.ExecDirect("select origin_id, date from tiles where x=? and y=? and z=? and zone=?");
    while (conn.Fetch())
    {
        SOriginAndDate data;
        data.originId = conn.GetInt(1);
        data.date = conn.GetInt(2);
        data.level = tilexy.level;
        pOriginList->push_back(data);
    }
}

void CTableTiles::GetOrigins(CSqlConnection& conn, U32 x1, U32 y1, U32 x2, U32 y2, U8 level, int zone, vector<SOriginAndDate> &originList)
{
    conn.ResetStmt();
    conn.BindInt(&x1, 1);
    conn.BindInt(&x2, 2);
    conn.BindInt(&y1, 3);
    conn.BindInt(&y2, 4);
    conn.BindTinyInt(&level, 5);
    conn.BindInt(&zone, 6);
    conn.ExecDirect("select origin_id, date from tiles where x>=? and x<=? and y>=? and y<=? and z=? and zone=?");
    while (conn.Fetch())
    {
        SOriginAndDate data;
        data.originId = conn.GetInt(1);
        data.date = conn.GetInt(2);
        data.level = level;
        originList.push_back(data);
    }
}
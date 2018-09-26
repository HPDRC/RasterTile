#include "CIndexBuilder.h"
#include "CSqlConnection.h"
#include "CTablePathes.h"
#include "CTableTiles.h"
#include "CMetaFile.h"
#include "..\Common\CHelper.h"
#include <ctime>

int CIndexBuilder::Build(string dirPath, string connectionString)
{
    vector<string> metaList;
    CHelper::DirTraversal_EndsWith(CHelper::FormatPath(dirPath), &metaList, string(META_EXT_NAME));
    int n = (int)metaList.size();
    int startTime = clock();
    Log("----------- %d files in dir %s ---------------", n, dirPath.c_str());

    CSqlConnection conn;
    conn.Open(connectionString.c_str());
    conn.StartTransaction();

    int recordTotal = 0;
	
    // foreach meta file, add its records into tiles table
    for (int i=0; i<n; i++)
    {
        // first, generate data path and ensure it exists
        string dataPath = CHelper::ToDataPath(metaList[i]);
        if (CHelper::IsFileExist(dataPath) == false)
        {
            Log("ERROR!!! meta file has no data file: %s", dataPath.c_str());
            continue;
        }
        
        // add data file's path into database
        conn.ResetStmt();
        CTablePathes::AddPath(conn, dataPath.c_str());
        conn.CommitTransaction();

        // get its path id
        conn.ResetStmt();
        int pathId = CTablePathes::GetIdByPath(conn, dataPath.c_str());
        conn.CommitTransaction();

        // read meta records
        CMetaFile metafile(metaList[i].c_str(), false);
        int recordCnt = metafile.GetRecordCnt();
        recordTotal += recordCnt;
        Log("%d/%d(%d minutes left): %d records in %s", i+1, n, (clock()-startTime)/60000*(n-i-1)/(i+1), recordCnt, metaList[i].c_str());
        STileMetaInfo *tileMetaInfos = new STileMetaInfo[recordCnt];
        for (int i=0; i<recordCnt; i++)
            metafile.Read(i, &tileMetaInfos[i]);

        // insert into table
        conn.ResetStmt();
        CTableTiles::AddTilePrepare(conn);
        for (int i=0; i<recordCnt; i++)
            CTableTiles::AddTile(conn, pathId, &tileMetaInfos[i]);

        // clean up
        conn.CommitTransaction();
        delete[] tileMetaInfos;
    }

    Log("--------------- All files are processed, %d records are inserted -----------------", recordTotal);
    conn.CommitTransaction();
    conn.Close();
    Log("----- Connection to database is closed successfully -----");
    return recordTotal;
}
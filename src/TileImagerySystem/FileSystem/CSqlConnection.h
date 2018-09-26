#pragma once
#include "includes.h"

// basic operation of microsoft sql server
class CSqlConnection
{
private:
    void *envHandle;    
	void *connectionHandle;
	void *statementHandle;

    // write warning or error msg to log
    void WriteLog(U32 handletype, const void *handle);

public:
    bool isLastGetSuccess;

public:
    CSqlConnection();
    ~CSqlConnection();

    void Open(const char *dbConnectionString);
    void Close();

    void StartTransaction();
    void CommitTransaction();
    
    void PrepareStmt(char sql[]);
    void ResetStmtParams();
    void ResetStmt();

    void BindString(const char param[]);
    void BindBigInt(void* param, int index = 1);
    void BindInt(void* param, int index = 1);
    void BindSmallInt(void* param, int index = 1);
    void BindTinyInt(void* param, int index = 1);

    bool Fetch();
    void GetString(char result[], int bufferLength, int index = 1);
    S64 GetS64(int index = 1);
    int GetInt(int index = 1);
    S16 GetS16(int index = 1);
    U8 GetU8(int index = 1);

    void ExecPrepared();
    void ExecDirect(const char sql[]);
    void ExecDirect_SupressError(char sql[]);
};

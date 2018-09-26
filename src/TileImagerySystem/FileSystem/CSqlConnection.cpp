#include "CSqlConnection.h"
#include <Windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

CSqlConnection::CSqlConnection()
{
    envHandle = NULL;
    connectionHandle = NULL;
    statementHandle = NULL;
    isLastGetSuccess = false;
}

CSqlConnection::~CSqlConnection()
{
    Close();
}

void CSqlConnection::Open(const char *dbConnectionString)
{
    if (!SQL_SUCCEEDED(SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, 0)))
        LogThrow("Can not enable ODBC Connection Pooling");

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle)
        || SQL_SUCCESS != SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)
        || SQL_SUCCESS != SQLSetEnvAttr(envHandle, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, 0)
        || SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connectionHandle))
        LogThrow("Can not initiate SQL driver");

    SQLCHAR retconstring[1024];
	switch(SQLDriverConnect(connectionHandle, NULL, (SQLCHAR*)dbConnectionString, SQL_NTS, retconstring, 1024, NULL,SQL_DRIVER_NOPROMPT))
	{
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_INVALID_HANDLE:
		case SQL_ERROR:
			WriteLog(SQL_HANDLE_DBC, connectionHandle);
            LogThrow("Can not connect to SQL Server");
			break;
		default:
			break;
	}
 
	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle))
        LogThrow("SQL: SQLAllocHandle error");
}

void CSqlConnection::Close()
{
    if (statementHandle != NULL)
        SQLFreeHandle(SQL_HANDLE_STMT, statementHandle), statementHandle = NULL;
    if (connectionHandle != NULL)
    {
        SQLDisconnect(connectionHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, connectionHandle);
        connectionHandle = NULL;
    }
    if (envHandle != NULL)
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle), envHandle = NULL;
}

void CSqlConnection::WriteLog(U32 handletype, const void *handle)
{
    char sqlstate[1024];
	char message[1024];
	if(SQL_SUCCESS == SQLGetDiagRec(handletype, (SQLHANDLE)handle, 1, (SQLCHAR*)sqlstate, NULL, (SQLCHAR*)message, 1024, NULL))
        Log("SQL Server: %s, %s", sqlstate, message);
}


void CSqlConnection::StartTransaction()
{
    if (SQLSetConnectAttr(connectionHandle, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, NULL) != SQL_SUCCESS)
        LogThrow("SQL: can not commit transaction");
}

void CSqlConnection::CommitTransaction()
{
    if (SQLEndTran(SQL_HANDLE_DBC, connectionHandle, SQL_COMMIT) != SQL_SUCCESS)
        LogThrow("SQL: can not commit transaction");
}

void CSqlConnection::ResetStmtParams()
{
    if (SQLFreeStmt(statementHandle, SQL_RESET_PARAMS) != SQL_SUCCESS)
        LogThrow("SQL: can not reset statement: free error");
}

void CSqlConnection::ResetStmt()
{
    if (SQLFreeHandle(SQL_HANDLE_STMT, statementHandle) != SQL_SUCCESS)
        LogThrow("SQL: can not reset statement: free error");
    statementHandle = NULL;
    if (SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle) != SQL_SUCCESS)
        LogThrow("SQL: can not reset statement: allocate error");
}

void CSqlConnection::BindString(const char param[])
{
    SQLBindParameter(statementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, MAX_PATH_LEN, 0, (SQLPOINTER)param, strlen(param), NULL);
}

void CSqlConnection::BindBigInt(void* param, int index)
{
    SQLBindParameter(statementHandle, index, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 10, 0, param, 8, NULL);
}

void CSqlConnection::BindInt(void* param, int index)
{
    SQLBindParameter(statementHandle, index, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, param, 4, NULL);
}

void CSqlConnection::BindSmallInt(void* param, int index)
{
    SQLBindParameter(statementHandle, index, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 10, 0, param, 2, NULL);
}

void CSqlConnection::BindTinyInt(void* param, int index)
{
    SQLBindParameter(statementHandle, index, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 10, 0, param, 1, NULL);
}

void CSqlConnection::PrepareStmt(char sql[])
{
    SQLPrepare(statementHandle, (SQLCHAR*)sql, SQL_NTS);
}

bool CSqlConnection::Fetch()
{
    return SQLFetch(statementHandle) == SQL_SUCCESS;
}

void CSqlConnection::GetString(char result[], int bufferLength, int index)
{
    isLastGetSuccess = SQL_SUCCESS == SQLGetData(statementHandle, index, SQL_C_CHAR, result, bufferLength, NULL);
    for (int i=(int)strlen(result)-1; i>=0; i--)
    {
        if (result[i] == ' ')
            continue;
        result[i+1] = 0;
        break;
    }
}

S64 CSqlConnection::GetS64(int index)
{
    S64 result = 0;
    isLastGetSuccess = SQL_SUCCESS == SQLGetData(statementHandle, index, SQL_C_SBIGINT, &result, 8, NULL);
    return result;
}

int CSqlConnection::GetInt(int index)
{
    int result = 0;
    isLastGetSuccess = SQL_SUCCESS == SQLGetData(statementHandle, index, SQL_C_SLONG, &result, 4, NULL);
    return result;
}

S16 CSqlConnection::GetS16(int index)
{
    S16 result = 0;
    isLastGetSuccess = SQL_SUCCESS == SQLGetData(statementHandle, index, SQL_C_SSHORT, &result, 2, NULL);
    return result;
}

U8 CSqlConnection::GetU8(int index)
{
    U8 result = 0;
    isLastGetSuccess = SQL_SUCCESS == SQLGetData(statementHandle, index, SQL_C_STINYINT, &result, 1, NULL);
    return result;
}

void CSqlConnection::ExecPrepared()
{
    if (SQLExecute(statementHandle) != SQL_SUCCESS)
    {
        WriteLog(SQL_HANDLE_STMT, statementHandle);
        LogThrow("SQL: SQLExecute error");
    }
}

void CSqlConnection::ExecDirect(const char sql[])
{
    if(SQLExecDirect(statementHandle, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
    {
        WriteLog(SQL_HANDLE_STMT, statementHandle);
        LogThrow("SQL: SQLExecDirect error");
    }
}

void CSqlConnection::ExecDirect_SupressError(char sql[])
{
    SQLExecDirect(statementHandle, (SQLCHAR*)sql, SQL_NTS);
}
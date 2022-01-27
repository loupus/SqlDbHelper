#pragma once
#include <iostream>
#include <wchar.h>
#include <vector>
#include "Globals.hpp"
#include "OdbcTypes.hpp"
#include "DbClasses.hpp"
#include "SqlDbHelper.h"



class cSqlDb : public ISqlDb
{
private:
    SQLHANDLE hEnv = SQL_NULL_HENV, hDBC = SQL_NULL_HDBC, hStmt = SQL_NULL_HSTMT;
    SQLRETURN ret = 0;
    SQLCHAR conStr[256] = { 0 };
    std::vector<ISParameter> params;
    std::vector<SColumn> columns;
    std::vector<RowData*> vd;

    BackObject LoadData();
    void GetCFieldType(SColumn& pCol);
    void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode, BackObject& pBack );
    void DumpLoaded();
    BackObject BindColumns();
    void ResetParams();
    void ResetColumns();
    BackObject Initiliaze();
    BackObject Connect();
    BackObject DisConnect();
    BackObject BindParameters();
    bool IsConnected();
    void AddParameter(ISParameter& p, bool bResetParams = false);

public:
    cSqlDb();
    ~cSqlDb();
    bool DoAddParameter(void* pValue, dbRetType pType, unsigned short pNumber, int pDir, bool bResetParams = false);
    bool DoInitiliaze(const char* pConstr, char* pErr, size_t pErrSize);
    bool DoExecute(const char* pQuery, char* pErr, size_t pErrSize);
    bool DoLoad(RowData** pRows);
};
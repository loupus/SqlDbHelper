#pragma once
#include <iostream>
#include <wchar.h>
#include <vector>
#include "Globals.hpp"
#include "OdbcTypes.hpp"
#include "DbClasses.hpp"

#define uniqueidensize 74
#define MAXCOLS 10

class cSqlDb
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

public:
    cSqlDb(const char* pConstr);
    ~cSqlDb();
    BackObject Initiliaze();
    BackObject Connect();
    BackObject DisConnect();
    BackObject BindColumns();
   



    template<typename T>
    bool AddParameter(T* pValuePtr,  unsigned short pParameterNumber, long long pColumnSize = 0,  PDirection pDir = PDirection::ParamInput);
    bool AddParameter(ISParameter& p);
    BackObject BindParameters();
    void ResetParams();
    void ResetColumns();
    BackObject Doit();

   
    


};
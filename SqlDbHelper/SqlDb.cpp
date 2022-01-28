
#include <typeinfo>
#include <time.h>
#include "SqlDb.hpp"

BackObject cSqlDb::Initiliaze()
{
    BackObject back;
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLAllocHandleENV failed";
        HandleDiagnosticRecord(hEnv, SQL_HANDLE_ENV, ret, back);
        back.Success = false;
        return back;
    }
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLSetEnvAttr failed";
        HandleDiagnosticRecord(hEnv, SQL_HANDLE_ENV, ret, back);
        back.Success = false;
        return back;
    }

    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLAllocHandleDBC failed";
        HandleDiagnosticRecord(hDBC, SQL_HANDLE_DBC, ret, back);
        back.Success = false;
        return back;
    }
    ret = SQLSetConnectAttr(hDBC, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLSetConnectAttr failed";
        HandleDiagnosticRecord(hDBC, SQL_HANDLE_DBC, ret, back);
        back.Success = false;
        return back;
    }

    back.Success = true;
    return back;

}

BackObject cSqlDb::Connect()
{
    BackObject back;
    SQLCHAR outstr[1024] = { 0 };
    SQLSMALLINT outstrlen = 0;
    SQLSMALLINT* outlenptr = nullptr;
    ret = SQLDriverConnectA(hDBC, NULL, conStr, SQL_NTS, outstr, sizeof(outstr), outlenptr, SQL_DRIVER_NOPROMPT);
    if(ret == SQL_SUCCESS_WITH_INFO)   HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLDriverConnectA failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);       
        back.Success = false;
        return back;
    }
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLAllocHandleSTMT failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        back.Success = false;
    } 
    return back;
}

BackObject cSqlDb::DisConnect()
{
    BackObject back;
    ret = SQLDisconnect(hDBC);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLDisconnect failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        back.Success = false;
    }
    return back;
}

void cSqlDb::AddParameter(ISParameter& p, bool bResetParams)
{
    if (bResetParams) ResetParams();
    params.push_back(p);
}

BackObject cSqlDb::BindParameters()
{
    BackObject back;
    if (params.size() > 0)
    {
        for (auto& j : params)
        {
            ret = SQLBindParameter(hStmt, j.ParameterNumber, j.InputOutputType, j.ValueType, j.ParameterType, j.ColumnSize, j.DecimalDigits, j.ParameterValuePtr, j.BufferLength, &j.StrLen_or_IndPtr);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
            {
                back.ErrDesc = "SQLBindParameter failed";
                HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
                return back;
            }
           
        }
    }    
    return back;
}

BackObject cSqlDb::GetRowCount(long long* pRowCount)
{
    BackObject back;
    SQLLEN rowCount = 0;
    ret = SQLRowCount(hStmt, &rowCount);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLRowCount failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        back.Success = false;
        return back;
    }
    *pRowCount = rowCount;
    return back;
}

bool cSqlDb::IsConnected()
{
    /*
    SQLRETURN SQLGetConnectAttr(
        SQLHDBC        ConnectionHandle,
        SQLINTEGER     Attribute,
        SQLPOINTER     ValuePtr,
        SQLINTEGER     BufferLength,
        SQLINTEGER * StringLengthPtr);
        */
    bool back = false;
    SQLINTEGER status = 0;
    ret = SQLGetConnectAttr(hDBC, SQL_ATTR_CONNECTION_DEAD, &status, SQL_IS_INTEGER , NULL);
    if (status == SQL_CD_TRUE)
        back = true;
    return back;
}

void cSqlDb::ResetParams()
{
    params.clear();
    params.shrink_to_fit();
}

void cSqlDb::ResetColumns()
{
    columns.clear();
    columns.shrink_to_fit();
}

cSqlDb::cSqlDb()
{

}

cSqlDb::~cSqlDb()
{
    if (hStmt)
    {
        SQLCloseCursor(hStmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    if (hDBC)
    {
        SQLDisconnect(hDBC);
        SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
    }

    if (hEnv)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

    ClearRowData();
}

void cSqlDb::GetCFieldType(SColumn& pCol)
{
    pCol.TargetValuePtr = (SQLPOINTER)malloc(pCol.ColumnSize);
    switch (pCol.ColumnType)
    {
    case SQL_BIT:
        pCol.TargetType = SQL_C_BIT;
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_TINYINT:
    case SQL_SMALLINT:
    case SQL_INTEGER:
        pCol.TargetType = SQL_C_SLONG;
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_REAL:
    case SQL_FLOAT:
        pCol.TargetType = SQL_C_FLOAT;
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_DOUBLE:
        pCol.TargetType = SQL_C_DOUBLE;
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_BIGINT:
        pCol.TargetType = SQL_C_SBIGINT;
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_DATE:
    case SQL_TIME:
    case SQL_TYPE_DATE:
    case SQL_TYPE_TIME:
    case SQL_TIMESTAMP:
    case SQL_TYPE_TIMESTAMP:
        pCol.TargetType = SQL_C_TIMESTAMP;
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
        pCol.TargetType = SQL_C_BINARY;
        break;

    case SQL_DECIMAL:
    case SQL_GUID:
    case SQL_LONGVARCHAR:
    case SQL_UNICODE_LONGVARCHAR:
    case SQL_CHAR:
    case SQL_VARCHAR:
    case SQL_UNICODE:
    case SQL_UNICODE_VARCHAR:
        pCol.TargetType = SQL_C_CHAR;
        pCol.StrLen_or_IndPtr = SQL_NTS;
        break;


    default:
        //	Unknown data type
        pCol.TargetType = SQL_C_CHAR;
        pCol.StrLen_or_IndPtr = SQL_NTS;
    }
}

void cSqlDb::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode, BackObject& pBack)
{
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iError;
    SQLCHAR       szMessage[1000];
    SQLCHAR       szState[SQL_SQLSTATE_SIZE + 1];


    if (RetCode == SQL_INVALID_HANDLE)
    {
       // fwprintf(stderr, L"Invalid handle!\n");
        return;
    }

    while (SQLGetDiagRecA(hType,
        hHandle,
        ++iRec,
        szState,
        &iError,
        szMessage,
        (SQLSMALLINT)(sizeof(szMessage) / sizeof(SQLCHAR)),
        (SQLSMALLINT*)NULL) == SQL_SUCCESS)
    {
        // Hide data truncated..
        if (strncmp((char*)szState, "01004", 5))
        {
            pBack.ErrDesc.append((char*)szMessage);
            //std::cout << "ErrMessage: " << szMessage << " State: " << szState << " ErrCode: " << iError << std::endl;
        }
    }

}

void cSqlDb::DumpLoaded()
{
    for (auto i : vd)
    {
        std::cout << "RowNumber: " << i->RowNumber << std::endl;
        for (auto j : i->Fields)
        {
            if (j)
                std::cout << j->fieldName << " : " << j->GetStrValue() << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
}

BackObject cSqlDb::BindColumns()
{
    BackObject back;
    SQLRETURN ret;
    SQLSMALLINT ncolumns;
    SQLINTEGER i = 0;
    SQLLEN displaysize;

    ret = SQLNumResultCols(hStmt, &ncolumns);


    for (i = 0; i < ncolumns; i++)
    {
        SColumn s1(hStmt);
        s1.ColumnNumber = i + 1;
        ret = SQLDescribeColA(s1.StatementHandle, s1.ColumnNumber, s1.ColumnName, sizeof(s1.ColumnName), &s1.ColumnNameLen, &s1.ColumnType, &s1.ColumnSize, &s1.DecimalDigits, &s1.IsNullable);


        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            back.ErrDesc = "SQLDescribeColA failed";
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);           
            back.Success = false;
            return back;
        }


        displaysize = 0;
        // get Maximum number of characters required to display data
        // from the column.
        ret = SQLColAttribute(hStmt, (SQLUSMALLINT)i + 1, SQL_COLUMN_DISPLAY_SIZE, NULL, 0, NULL, &displaysize);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            back.ErrDesc = "SQLColAttribute/SQL_COLUMN_DISPLAY_SIZE  failed";
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret,back);           
            back.Success = false;
            return back;
        }

        if (s1.ColumnSize > displaysize)
        {
            s1.ColumnSize = displaysize + 1;
        }
        else
        {
            s1.ColumnSize = s1.ColumnSize + 1;
        }


        if (s1.TargetType == SQL_CHAR || s1.TargetType == SQL_VARCHAR || s1.TargetType == SQL_GUID)
        {
            s1.ColumnSize = s1.ColumnSize * 2;  // cunku unicode character 2 bayt hikayesi var
        }

        columns.push_back(s1);
    }

    for (auto& d : columns)
    {
        GetCFieldType(d);
        ret = SQLBindCol(d.StatementHandle, d.ColumnNumber, d.TargetType, d.TargetValuePtr, d.ColumnSize, &d.StrLen_or_IndPtr);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            back.ErrDesc = "SQLBindCol  failed";
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);           
            back.Success = false;
            return back;
 
        }
    }
    return back;
}

BackObject cSqlDb::LoadData()
{
    BackObject back;
    SQLRETURN  retcode;
    IFieldData* dd = nullptr;
    RowData* rd = nullptr;

    ClearRowData();

    while (true)
    {
        rd = new RowData();
        rd->RowNumber = vd.size() + 1;

        ret = SQLFetch(hStmt);
        if (ret == SQL_NO_DATA_FOUND)
        {
           // std::cout << "No data back " << std::endl; // todo ?
            break;
        }

        if (ret == SQL_ERROR)
        {
            back.ErrDesc = "Something wrong";
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);            
            back.Success = false;
            break;
        }

        if (ret == SQL_SUCCESS_WITH_INFO)
        {           
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        }

        for (auto& d : columns)
        {
            switch (d.TargetType)
            {
            case SQL_C_WCHAR:
            case SQL_C_CHAR:
            {
                dd = new FieldData<std::string>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_String;
                dd->fnumber = d.ColumnNumber;
              //  ((FieldData<std::string>*)dd)->SetValue(d.toStr());
                ((FieldData<std::string>*)dd)->SetValue(d.AsString());
                break;
            }
          
            case SQL_C_LONG:
            case SQL_C_SLONG:
            {
                dd = new FieldData<int>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_Int;
                dd->fnumber = d.ColumnNumber;
               // ((FieldData<int>*)dd)->SetValue(*(int*)(d.TargetValuePtr));
                ((FieldData<int>*)dd)->SetValue(d.AsInt());
                break;
            }
            case SQL_C_FLOAT:
            {
                dd = new FieldData<float>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_Float;
                dd->fnumber = d.ColumnNumber;
                // ((FieldData<int>*)dd)->SetValue(*(int*)(d.TargetValuePtr));
                ((FieldData<float>*)dd)->SetValue(d.AsFloat());
                break;
            }
            case SQL_C_UBIGINT:
            case SQL_C_SBIGINT:
            {
                dd = new FieldData<long long>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_Int64;
                dd->fnumber = d.ColumnNumber;
                ((FieldData<long long>*)dd)->SetValue(*(long long*)(d.TargetValuePtr));
                ((FieldData<long long>*)dd)->SetValue(d.AsLongLong());
                break;
            }
            case SQL_C_GUID:
            {
                dd = new FieldData<std::string>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_String;
                dd->fnumber = d.ColumnNumber;
                ((FieldData<std::string>*)dd)->SetValue(d.AsString());
                break;
            }
            case SQL_C_TIMESTAMP:
            case SQL_C_DATE:
            case SQL_C_TIME:
            {
                dd = new FieldData<std::string>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_String;
                dd->fnumber = d.ColumnNumber;
                ((FieldData<std::string>*)dd)->SetValue(d.AsString());
                break;
            }
            case SQL_C_BINARY:
            {
                std::cout << "binary type: " << d.TargetType << std::endl;
                dd = new FieldData<byte*>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_Byte;
                dd->fnumber = d.ColumnNumber;
                ((FieldData<byte*>*)dd)->SetValue(d.AsByteP());
                break;
            }
            case SQL_C_BIT:
            {
                dd = new FieldData<bool>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::db_Bool;
                dd->fnumber = d.ColumnNumber;
                //bool* bp = reinterpret_cast<bool*>(d.TargetValuePtr);
                ((FieldData<bool>*)dd)->SetValue(d.AsBool());
                break;
            }
            case SQL_C_NUMERIC:
            {
                std::cout << "numeric type: " << d.TargetType << std::endl;
                break;
            }
            default:
            {
                std::cout << "unhandled type: " << d.TargetType << std::endl;
                break;
            }
               

            }
           // std::cout << d.ColumnName << " : " << d.AsString() << std::endl;
            if(dd)
                rd->Fields.push_back(dd);
        }
        vd.push_back(rd);
    }

    return back;
}

void cSqlDb::ClearRowData()
{
    for (auto r : vd)
    {   
        if (r) // field temizliği rowdatanın destructorında
        {
            delete r;
            r = nullptr;
        }
    }
    vd.clear();
    vd.shrink_to_fit();
}

bool cSqlDb::DoAddParameter(void* pValue, dbRetType pType, unsigned short pNumber, int pDir, bool bResetParams)
{
    bool back = true;
    ISParameter pp;
    switch (pType)
    {
    case dbRetType::db_Short:
    {
        pp = ISParameter::ParameterFactory::CreateShortParameter(reinterpret_cast<short*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    case dbRetType::db_Int:
    {
        pp = ISParameter::ParameterFactory::CreateIntParameter(reinterpret_cast<int*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    case dbRetType::db_Int64:
    {
        pp = ISParameter::ParameterFactory::CreateBigintIntParameter(reinterpret_cast<long long*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    case dbRetType::db_Float:
    {
        pp = ISParameter::ParameterFactory::CreateFloatParameter(reinterpret_cast<float*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    case dbRetType::db_Bool:
    {
        pp = ISParameter::ParameterFactory::CreateBoolParameter(reinterpret_cast<bool*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    case dbRetType::db_String:
    {
        pp = ISParameter::ParameterFactory::CreateCharParameter(reinterpret_cast<std::string*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    case dbRetType::db_Date:
    {
        pp = ISParameter::ParameterFactory::CreateDateParameter(reinterpret_cast<std::string*>(pValue), pNumber, pDir);
        AddParameter(pp, bResetParams);
        break;
    }
    default:
        back = false;
    }

    return back;
}

bool cSqlDb::DoInitiliaze(const char* pConstr, char* pErr, size_t pErrSize)
{
    bool back = false;
    if (pConstr)
        strncpy((char*)conStr, pConstr, 256);
    else
        return back;
    BackObject rb = Initiliaze();
    if (rb.Success == false)
    {
        back = false;
        memset(pErr, 0, pErrSize);
        strncpy(pErr,rb.ErrDesc.c_str(),pErrSize-1);
    }
    else
         back = true;
    return back;
}

bool cSqlDb::DoExecute(const char* pQuery, char* pErr, size_t pErrSize)
{
    bool back = false;
    BackObject rb;

    if (hEnv == SQL_NULL_HENV)
    {
        memset(pErr, 0, pErrSize);
        strncpy(pErr, "Odbc driver is not initilized", pErrSize - 1);
        return back;
    }

    SQLSMALLINT sNumResults;
    SQLLEN cRowCount;
    SQLRETURN ret;

    if (!IsConnected())
    {
        rb = Connect();
        if (!rb.Success)
        {
            memset(pErr, 0, pErrSize);
            strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
            return back;
        }
   }
        
    BindParameters();
    ret = SQLPrepareA(hStmt, (SQLCHAR*)pQuery, SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        rb.ErrDesc = "SQLPrepareA failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, rb);
        memset(pErr, 0, pErrSize);
        strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
        return back;
    }

    rb = BindColumns();
    if (!rb.Success)
    {
        memset(pErr, 0, pErrSize);
        strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
        return back;
    }



    ret = SQLExecDirectA(hStmt, (SQLCHAR*)pQuery, SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        rb.ErrDesc = "SQLExecDirectA failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, rb);
        memset(pErr, 0, pErrSize);
        strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
        DisConnect();
        return back;
    }
    else
    {
        ret = SQLNumResultCols(hStmt, &sNumResults);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            rb.ErrDesc = "SQLNumResultCols failed";
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, rb);
            memset(pErr, 0, pErrSize);
            strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
            DisConnect();
            return back;
        }
        if (sNumResults > 0)
        {
            rb = LoadData();
            if (!rb.Success)
            {
                memset(pErr, 0, pErrSize);
                strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
                DisConnect();
            }
            //DumpLoaded();
        }
        else
        {
            ret = SQLRowCount(hStmt, &cRowCount);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
            {
                rb.ErrDesc = "SQLRowCount failed";
                HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, rb);
                memset(pErr, 0, pErrSize);
                strncpy(pErr, rb.ErrDesc.c_str(), pErrSize - 1);
                DisConnect();
                return back;
            }
            if (cRowCount >= 0)
            {
                std::cout << "affected rows: " << cRowCount << std::endl;
            }
        }
    }

    DisConnect();
    back = true;
    return back;
}

bool cSqlDb::DoLoad(RowData** pRows)
{
    bool back = false;      
    int rowcount = 0;
    rowcount = vd.size();
    for (int i = 0; i<vd.size(); i++)
    {
        *pRows = new RowData();
        (*pRows)->RowNumber = vd[i]->RowNumber;
        (*pRows)->Fields.assign(vd[i]->Fields.begin(), vd[i]->Fields.end());
        pRows++;
    }
    back = true;
    return back;
}

void cSqlDb::DoGetRowCount(size_t* pRowCount)
{
    *pRowCount = vd.size();
}


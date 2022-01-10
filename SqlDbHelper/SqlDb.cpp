
#include <typeinfo>

#include "SqlDb.hpp"

BackObject cSqlDb::Initiliaze()
{
    BackObject back;
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLAllocHandleENV failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        back.Success = false;
        return back;
    }
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLSetEnvAttr failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        back.Success = false;
        return back;
    }

    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLAllocHandleDBC failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        back.Success = false;
        return back;
    }
    ret = SQLSetConnectAttr(hDBC, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLSetConnectAttr failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
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

bool cSqlDb::AddParameter(ISParameter& p)
{
    params.push_back(p);
    return true;

}


template<typename T>
bool cSqlDb::AddParameter(T* pValuePtr, unsigned short pParameterNumber, long long pColumnSize, PDirection pDir)
{
    ISParameter s1;
    s1.StatementHandle = hStmt;
    s1.ColumnSize = pColumnSize;
    s1.DecimalDigits = 0;
    s1.InputOutputType = pDir;
    s1.ParameterNumber = pParameterNumber;
    if (typeid(T) == typeid(short))
    {
        s1.ParameterType = SQL_SMALLINT;
        s1.ValueType = SQL_C_SHORT;
        s1.ParameterValuePtr = pValuePtr;
    }
    else if (typeid(T) == typeid(int))
    {
        s1.ParameterType = SQL_INTEGER;
        s1.ValueType = SQL_C_LONG;
        s1.ParameterValuePtr = pValuePtr;
    }
    else if (typeid(T) == typeid(std::string))
    {
        s1.ParameterType = SQL_CHAR;
        s1.ValueType = SQL_C_CHAR;
        s1.BufferLength = pColumnSize;
        s1.ParamToLocal();
    }
    params.push_back(s1);
    return true;
}


BackObject cSqlDb::BindParameters()
{
    BackObject back;
    SQLRETURN retcode;
    if (params.size() > 0)
    {
        for (auto& j : params)
        {
            retcode = SQLBindParameter(j.StatementHandle, j.ParameterNumber, j.InputOutputType, j.ValueType, j.ParameterType, j.ColumnSize, j.DecimalDigits, j.ParameterValuePtr, j.BufferLength, &j.StrLen_or_IndPtr);
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

//void cSqlDb::Doit()
//{
//    BackObject back;
//
//    SQLCHAR         colname[MAXCOLS][32];        // column name
//    SQLSMALLINT     coltype[MAXCOLS];            // column type
//    SQLSMALLINT     colnamelen[MAXCOLS];         // length of column name
//    SQLSMALLINT     nullable[MAXCOLS];           // whether column can have NULL value
//    SQLULEN         collen[MAXCOLS];             // array of column lengths
//    SQLSMALLINT     decimaldigits[MAXCOLS];               // no of digits if column is numeric
//    SQLLEN          outlen[MAXCOLS];             // lengths of column values returned
//    SQLPOINTER      data[MAXCOLS];               // buffers for retrieving column values
//    SQLRETURN       retcode;                     // general return code
//    SQLLEN          displaysize;                 // drivers column display size
//    SQLINTEGER      i, j;
//    SQLSMALLINT columns;
//
//    SQLRETURN ret;
//    int iType = 13;
//
//    back = Initiliaze();
//    if (!back.Success)
//    {
//        std::cout << back.ErrDesc << std::endl;
//        goto Exit;
//    }
//    back = Connect();
//    if (!back.Success)
//    {
//        std::cout << back.ErrDesc << std::endl;
//        goto Exit;
//    }
//
//   
//
//    AddParameter<int>(&iType, 1);
//    BindParameters();
//    ret = SQLPrepareA(hStmt, (SQLCHAR*)"select ID,Name, Type, ParentID from Assets where Type = ?",SQL_NTS);
//    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//    {
//        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret);
//        std::cout << "SQLPrepareA failed" << std::endl;
//        goto Exit;
//    }
//
//
//
//
//    // int cType = 0;
//
//     // Initialise data array
//    for (i = 0; i < MAXCOLS; i++)
//    {
//        data[i] = NULL;
//    }
//
//    ret = SQLNumResultCols(hStmt, &columns);
//
//
//    for (i = 0; i < columns; i++)
//    {
//        // for each column from the prepared statement in hstmt, get the
//        // column name, type, column size, decimal digits, and nullability
//        ret = SQLDescribeColA(hStmt,
//            (SQLUSMALLINT)i + 1,
//            colname[i],
//            sizeof(colname[i]),
//            &colnamelen[i],
//            &coltype[i],
//            &collen[i],
//            &decimaldigits[i],
//            &nullable[i]);
//
//        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//        {
//            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret);
//            std::cout << "SQLDescribeColA failed" << std::endl;
//            goto Exit;
//        }
//
//
//        displaysize = 0;
//        // get Maximum number of characters required to display data
//        // from the column.
//        ret = SQLColAttribute(hStmt, (SQLUSMALLINT)i + 1, SQL_COLUMN_DISPLAY_SIZE, NULL, 0, NULL, &displaysize);
//        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//        {
//            std::cout << "SQLColAttribute/SQL_COLUMN_DISPLAY_SIZE  failed" << std::endl;
//            goto Exit;
//        }
//
//
//        if (collen[i] > displaysize)
//        {
//            collen[i] = displaysize + 1;
//        }
//        else
//        {
//            collen[i] = collen[i] + 1;
//        }
//
//
//        if (coltype[i] == SQL_CHAR || coltype[i] == SQL_VARCHAR || coltype[i] == SQL_GUID)
//        {
//            collen[i] = collen[i] * 2;  // cunku unicode character 2 bayt hikayesi var
//        }
//
//       // std::cout << "name: " << colname[i] << "    namelen: " << colnamelen[i] << "    type: " << coltype[i] << "  len: " << collen[i] << "    decimaldigits: " << decimaldigits[i] << "   nullable: " << nullable[i] << std::endl;
//
//      //  printf("colname: %ls colnamelen: %ld coltype: %ld  collen: %ld  decimaldigits: %ld  nullable:%ld \n", colname[i], colnamelen[i], coltype[i], collen[i], decimaldigits[i], nullable[i]);
//    }
//
//    for (i = 0; i < columns; i++)
//    {
//        ret = SQLBindCol(hStmt, (SQLUSMALLINT)i + 1, GetCFieldType(coltype[i], &data[i], collen[i], outlen[i]), (SQLPOINTER)data[i], collen[i], &outlen[i]);
//        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//        {
//            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret);
//            std::cout << "SQLBindCol" << colname[i] <<" failed" << std::endl;
//            goto Exit;
//          
//        }
//    }
//
//
//    ret = SQLExecDirect(hStmt, (SQLTCHAR*)L"select ID,Name, Type, ParentID from Assets where Type = ?;", SQL_NTS);
//    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//    {
//        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret);
//        std::cout << "SQLExecDirect failed" << std::endl;
//        goto Exit;
//    }
//
//    while (true)
//    {
//        ret = SQLFetch(hStmt);
//        if (ret == SQL_NO_DATA_FOUND)
//        {
//            printf("End of data.\n");
//            break;
//        }
//
//        if (ret == SQL_ERROR)
//        {
//            printf("hata var\n");
//            //HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret);
//            break;
//        }
//
//        if (ret == SQL_SUCCESS_WITH_INFO)
//        {
//            printf("oldu gibi\n");
//            //HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret);
//
//        }
//
//
//        std::cout << "data0 - " << (char*)(data[0]) << std::endl;
//        std::cout << "data1 - " << (char*)data[1] << std::endl;
//        std::cout << "data2 - " << *(int*)data[2] << std::endl;
//        std::cout << "data3 - " << (char*)data[3] << std::endl;
//
//    }
//
//
//Exit:
//
//    /* free data buffers  */
//    for (i = 0; i < columns; i++)
//    {
//        if (data[i] != NULL) free(data[i]);
//    }
//
//    if (hStmt)
//    {
//        SQLCloseCursor(hStmt);
//        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//    }
//
//    if (hDBC)
//    {
//        SQLDisconnect(hDBC);
//        SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
//    }
//
//    if (hEnv)
//    {
//        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
//    }
//
//
//}




cSqlDb::cSqlDb(const char* pConstr)
{
    if (pConstr)
    {
        strncpy((char*)conStr, pConstr, 256);
    }
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

}



void cSqlDb::GetCFieldType(SColumn& pCol)
{
    pCol.TargetValuePtr = (SQLPOINTER)malloc(pCol.ColumnSize);
    switch (pCol.ColumnType)
    {
    case SQL_BIT:
        pCol.TargetType = SQL_C_BIT;
       // pCol.TargetValuePtr = (SQLPOINTER)new bool();
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_TINYINT:
    case SQL_SMALLINT:
    case SQL_INTEGER:
        pCol.TargetType = SQL_C_SLONG;
     //   pCol.TargetValuePtr = (SQLPOINTER)malloc(pCol.ColumnSize);
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_REAL:
        pCol.TargetType = SQL_C_FLOAT;
     //   pCol.TargetValuePtr = (SQLPOINTER)new float();
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_FLOAT:
    case SQL_DOUBLE:
        pCol.TargetType = SQL_C_DOUBLE;
    //    pCol.TargetValuePtr = (SQLPOINTER)new double();
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_DATE:
    case SQL_TIME:
    case SQL_TIMESTAMP:
    case SQL_TYPE_TIMESTAMP:
        pCol.TargetType = SQL_C_TIMESTAMP;
    //    pCol.TargetValuePtr = (SQLPOINTER) new TIMESTAMP_STRUCT();
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_NUMERIC:
    case SQL_DECIMAL:
    case SQL_BIGINT:
        pCol.TargetType = SQL_C_LONG;
  //      pCol.TargetValuePtr = (SQLPOINTER)new long();
        pCol.StrLen_or_IndPtr = 0;
        break;

    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
        pCol.TargetType = SQL_C_BINARY;
        //  pCol.TargetValuePtr = (SQLPOINTER)malloc(pCol.ColumnSize);
  //      pCol.TargetValuePtr = (SQLPOINTER)new byte[pCol.ColumnSize];
        break;

    case SQL_GUID:
    case SQL_LONGVARCHAR:
    case SQL_UNICODE_LONGVARCHAR:
    case SQL_CHAR:
    case SQL_VARCHAR:
    case SQL_UNICODE:
    case SQL_UNICODE_VARCHAR:
        pCol.TargetType = SQL_C_CHAR;
        // pCol.TargetValuePtr = (SQLPOINTER)malloc(collen * sizeof(char));
    //    pCol.TargetValuePtr = (SQLPOINTER)new char[pCol.ColumnSize];
        pCol.StrLen_or_IndPtr = SQL_NTS;
        break;


    default:
        //	Unknown data type
        pCol.TargetType = SQL_C_CHAR;
   //     pCol.TargetValuePtr = (SQLPOINTER)new char[pCol.ColumnSize];
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
            std::cout << j->fieldName << " : " << j->GetStrValue();
        }
        std::cout << std::endl << std::endl;
        //std::cout << "FieldName: " << (*i)->fieldName << " \t Type: " << (*i)->dbType  << " \t Value: " <<  ((DbData<std::string>*)(*i))->GetStrValue() << std::endl;
         //std::cout << "FieldName: " << (*i)->fieldName << " \t Type: " << (*i)->dbType  << " \t Value: " <<  (*i)->GetStrValue() << std::endl;
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

    while (true)
    {
        rd = new RowData();
        rd->RowNumber = vd.size() + 1;

        ret = SQLFetch(hStmt);
        if (ret == SQL_NO_DATA_FOUND)
        {
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
            case SQL_C_CHAR:
            {
                dd = new FieldData<std::string>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::String;
                dd->fnumber = d.ColumnNumber;
                ((FieldData<std::string>*)dd)->SetValue(d.toStr());
                break;
            }
            case SQL_C_SLONG:
            {
                dd = new FieldData<int>();
                dd->fieldName = (char*)(d.ColumnName);
                dd->dbType = dbRetType::Int;
                dd->fnumber = d.ColumnNumber;
                ((FieldData<int>*)dd)->SetValue(*(int*)(d.TargetValuePtr));
                break;
            }

            }
            std::cout << d.ColumnName << " : " << d.toStr() << std::endl;

            rd->Fields.push_back(dd);
        }
        vd.push_back(rd);
    }

    return back;
}

BackObject cSqlDb::Doit()
{
    BackObject back;

    //SQLLEN          displaysize;                 // drivers column display size
    SQLINTEGER      i, j;
    SQLSMALLINT sNumResults;
    SQLLEN cRowCount;

    SQLRETURN ret;
    int iType = 13;

    back = Initiliaze();
    if (!back.Success)
        return back;
    back = Connect();
    if (!back.Success)
        return back;



    AddParameter<int>(&iType, 1);
    BindParameters();
    ret = SQLPrepareA(hStmt, (SQLCHAR*)"select ID,Name, Type, ParentID from Assets where Type = ?", SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLPrepareA failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        return back;
    }

    back = BindColumns();
    if (!back.Success)
        return back;



    ret = SQLExecDirect(hStmt, (SQLTCHAR*)L"select ID,Name, Type, ParentID from Assets where Type = ?;", SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        back.ErrDesc = "SQLExecDirect failed";
        HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
        return back;
    }
    else
    {
        ret = SQLNumResultCols(hStmt, &sNumResults);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            back.ErrDesc = "SQLNumResultCols failed";
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
            return back;
        }
        if (sNumResults > 0)
        {
            back = LoadData();
            if (!back.Success)
                return back;
            DumpLoaded();
        }
        else
        {
            ret = SQLRowCount(hStmt, &cRowCount);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
            {
                back.ErrDesc = "SQLRowCount failed";
                HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, ret, back);
                return back;
            }
            if (cRowCount >= 0)
            {
                std::cout << "affected rows: " << cRowCount << std::endl;
            }
        }
    }

    DisConnect();

    return back;

}
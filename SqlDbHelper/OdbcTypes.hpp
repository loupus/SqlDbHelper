#pragma once
#define _SQLNCLI_ODBC_  
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h> 
#include "mssql/sqlncli.h"
#include <sstream>

#define MAX_COL_NAME_LEN  256

enum PDirection
{
    ParamUnkown = SQL_PARAM_TYPE_UNKNOWN,
    ParamInput = SQL_PARAM_INPUT,
    ParamInputOutput = SQL_PARAM_INPUT_OUTPUT,
    ParamResultCol = SQL_RESULT_COL,
    ParamOutput = SQL_PARAM_OUTPUT,
    ParamReturnValue = SQL_RETURN_VALUE

};

class ISParameter
{
public:
    ISParameter() : StatementHandle(SQL_NULL_HSTMT), ParameterNumber(0), InputOutputType(PDirection::ParamUnkown)
        , ValueType(0), ParameterType(SQL_UNKNOWN_TYPE), ColumnSize(0), DecimalDigits(0)
        , ParameterValuePtr(nullptr), BufferLength(0), StrLen_or_IndPtr(0)
    {

    }

    virtual ~ISParameter()
    {

    }
    SQLHSTMT        StatementHandle;
    SQLUSMALLINT    ParameterNumber;
    SQLSMALLINT     InputOutputType;
    SQLSMALLINT     ValueType;
    SQLSMALLINT     ParameterType;
    SQLULEN         ColumnSize;
    SQLSMALLINT     DecimalDigits;
    SQLPOINTER      ParameterValuePtr;
    SQLLEN          BufferLength;
    SQLLEN          StrLen_or_IndPtr;

    //void operator=(ISParameter const&) = delete;     //asignment operator
    //ISParameter(ISParameter const&) = delete;         // copy constructor
  //  ISParameter() = delete;

    bool ParamToLocal()
    {
        if (ostr == nullptr) return false;
        int paramlen = strlen(ostr->c_str());
        ClearLocal();
        sstr = new SQLCHAR[paramlen];
        strcpy((char*)sstr, ostr->c_str());
        ParameterValuePtr = sstr;
        return true;
    }

    bool LocalToParam()
    {
        if (ostr == nullptr) return false;
        ostr->clear();
        if (sstr)
            ostr->append((char*)sstr);
        return true;
    }

private:
    SQLCHAR* sstr = nullptr;
    std::string* ostr = nullptr;
    void ClearLocal()
    {
        if (sstr)
        {
            delete sstr;
            sstr = nullptr;
        }
    }
};

class SColumn
{
public:
    SColumn(SQLHSTMT pSqlHandle) : ColumnNumber(0), TargetType(SQL_UNKNOWN_TYPE)
        , TargetValuePtr(nullptr), ColumnSize(0), StrLen_or_IndPtr(0)
    {
        StatementHandle = pSqlHandle;
    }
    SQLHSTMT       StatementHandle;
    SQLCHAR        ColumnName[MAX_COL_NAME_LEN];
    SQLSMALLINT    ColumnNameLen;
    SQLUSMALLINT   ColumnNumber;
    SQLULEN        ColumnSize;                         // For character and binary C data, For all other types of C data, the BufferLength argument is ignored. 
    SQLSMALLINT    DecimalDigits;
    SQLSMALLINT    IsNullable;
    SQLSMALLINT    ColumnType;
    SQLSMALLINT    TargetType;
    SQLPOINTER     TargetValuePtr;
   // SQLLEN         TargetValueBufferLength;        // For character and binary C data, For all other types of C data, the BufferLength argument is ignored. 
    SQLLEN         StrLen_or_IndPtr;              // This is ignored except for character or binary C data.
    

    std::string toStr()
    {
        std::ostringstream dtss;
        if (TargetType == SQL_C_CHAR)
            dtss << (char*)TargetValuePtr;
        else if (TargetType == SQL_C_SLONG)
            dtss << *(int*)TargetValuePtr;
        return dtss.str();
    }
};

/*
 SQLRETURN SQLDescribeCol(  
      SQLHSTMT       StatementHandle,  
      SQLUSMALLINT   ColumnNumber,  
      SQLCHAR *      ColumnName,  
      SQLSMALLINT    ColumnNameLocalBufferLength,  
      SQLSMALLINT *  ColNameLengthReturnValuePtr,  
      SQLSMALLINT *  DataTypePtr,  
      SQLULEN *      ColumnSizePtr,  
      SQLSMALLINT *  DecimalDigitsPtr,  
      SQLSMALLINT *  NullablePtr);  


            SQLRETURN SQLBindCol(
      SQLHSTMT       StatementHandle,
      SQLUSMALLINT   ColumnNumber,
      SQLSMALLINT    TargetType,
      SQLPOINTER     TargetValuePtr,
      SQLLEN         TargetValueBufferLength,
      SQLLEN *       StrLen_or_IndPtr);

*/


/*
https://docs.microsoft.com/en-us/sql/odbc/reference/appendixes/c-data-types?view=sql-server-ver15
TABLE 1
C type identifier	ODBC C typedef	C type
SQL_C_CHAR	SQLCHAR *	unsigned char *
SQL_C_WCHAR	SQLWCHAR *	wchar_t *
SQL_C_SSHORT[j]	SQLSMALLINT	short int
SQL_C_USHORT[j]	SQLUSMALLINT	unsigned short int
SQL_C_SLONG[j]	SQLINTEGER	long int
SQL_C_ULONG[j]	SQLUINTEGER	unsigned long int
SQL_C_FLOAT	SQLREAL	float
SQL_C_DOUBLE	SQLDOUBLE, SQLFLOAT	double
SQL_C_BIT	SQLCHAR	unsigned char
SQL_C_STINYINT[j]	SQLSCHAR	signed char
SQL_C_UTINYINT[j]	SQLCHAR	unsigned char
SQL_C_SBIGINT	SQLBIGINT	_int64[h]
SQL_C_UBIGINT	SQLUBIGINT	unsigned _int64[h]
SQL_C_BINARY	SQLCHAR *	unsigned char *
SQL_C_BOOKMARK[i]	BOOKMARK	unsigned long int[d]
SQL_C_VARBOOKMARK	SQLCHAR *	unsigned char *
All C interval data types	SQL_INTERVAL_STRUCT	See the C Interval Structure section, later in this appendix.

*/
#pragma once

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h> 
#include <sstream>
#include <iomanip>

const int MaxColNameLen{ 64 };

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

private:



public:
    SQLUSMALLINT    ParameterNumber;
    SQLSMALLINT     InputOutputType;
    SQLSMALLINT     ValueType;
    SQLSMALLINT     ParameterType;
    SQLULEN         ColumnSize;
    SQLSMALLINT     DecimalDigits;
    SQLPOINTER      ParameterValuePtr;
    SQLLEN          BufferLength;
    SQLLEN          StrLen_or_IndPtr;


    SQLPOINTER      LocalValuePtr;

    /*
    SQLRETURN SQLBindParameter(
      SQLHSTMT        StatementHandle,
      SQLUSMALLINT    ParameterNumber,
      SQLSMALLINT     InputOutputType,
      SQLSMALLINT     ValueType,
      SQLSMALLINT     ParameterType,
      SQLULEN         ColumnSize,
      SQLSMALLINT     DecimalDigits,
      SQLPOINTER      ParameterValuePtr,
      SQLLEN          BufferLength,
      SQLLEN *        StrLen_or_IndPtr);

    */

    class ParameterFactory
    {
    public:

        /*
        static ISParameter* CreateShortParameter(SQLHSTMT pSqlHandle, short* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter* back;
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_SMALLINT;
            back->ValueType = SQL_C_SHORT;
            back->ParameterValuePtr = pValuePointer;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = 0;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            return back;
        }
        static ISParameter* CreateIntParameter(SQLHSTMT pSqlHandle, int* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter* back;
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_INTEGER;
            back->ValueType = SQL_C_LONG;
            back->ParameterValuePtr = pValuePointer;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = 0;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            return back;
        }
        static ISParameter* CreateBigintIntParameter(SQLHSTMT pSqlHandle, long long* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter* back;
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_BIGINT;
            back->ValueType = SQL_C_SBIGINT;
            back->ParameterValuePtr = pValuePointer;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = 0;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            return back;
        }
        static ISParameter* CreateFloatParameter(SQLHSTMT pSqlHandle, float* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter* back;
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_FLOAT;
            back->ValueType = SQL_C_FLOAT;
            back->ParameterValuePtr = pValuePointer;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = 0;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            return back;
        }
        static ISParameter* CreateCharParameter(SQLHSTMT pSqlHandle, std::string* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter* back;
            int istrlen = strlen(pValuePointer->c_str());
            SQLCHAR* lstr = new SQLCHAR[istrlen];
            strcpy((char*)lstr, pValuePointer->c_str());
            back->LocalValuePtr = pValuePointer;
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_CHAR;
            back->ValueType = SQL_C_CHAR;
            back->ParameterValuePtr = lstr;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = (SQLULEN)strlen;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            back->StrLen_or_IndPtr = SQL_NTS;
            return back;
        }
        static ISParameter* CreateDateParameter(SQLHSTMT pSqlHandle, std::string* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {

            //* https://docs.microsoft.com/en-us/sql/odbc/reference/appendixes/c-to-sql-timestamp?view=sql-server-ver15
            //When timestamp C data is converted to character SQL data, the resulting character data is in the "yyyy-mm-dd hh:mm:ss[.f...]" format.
            //


            ISParameter* back = new ISParameter();
            int istrlen = strlen(pValuePointer->c_str());
            SQLCHAR* lstr = new SQLCHAR[istrlen];
            strcpy((char*)lstr, pValuePointer->c_str());
            back->LocalValuePtr = pValuePointer;
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_TYPE_TIMESTAMP;
            back->ValueType = SQL_C_CHAR;
            back->ParameterValuePtr = lstr;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = (SQLULEN)strlen;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            back->StrLen_or_IndPtr = SQL_NTS;
            return back;
        }
        static ISParameter* CreateBoolParameter(SQLHSTMT pSqlHandle, bool* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {

            ISParameter* back = new ISParameter();
            back->StatementHandle = pSqlHandle;
            back->ParameterType = SQL_BIT;
            back->ValueType = SQL_C_BIT;
            back->ParameterValuePtr = pValuePointer;
            back->ParameterNumber = pParamNumber;
            back->InputOutputType = (PDirection)pDirection;
            back->ColumnSize = 1;
            back->DecimalDigits = 0;
            back->BufferLength = 0;
            return back;
        }
        */
        static ISParameter CreateShortParameter(short* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter back;
            back.ParameterType = SQL_SMALLINT;
            back.ValueType = SQL_C_SHORT;
            back.ParameterValuePtr = pValuePointer;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = 0;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            return back;
        }
        static ISParameter CreateIntParameter(int* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter back;
            back.ParameterType = SQL_INTEGER;
            back.ValueType = SQL_C_LONG;
            back.ParameterValuePtr = pValuePointer;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = 0;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            return back;
        }
        static ISParameter CreateBigintIntParameter(long long* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter back;
            back.ParameterType = SQL_BIGINT;
            back.ValueType = SQL_C_SBIGINT;
            back.ParameterValuePtr = pValuePointer;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = 0;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            return back;
        }
        static ISParameter CreateFloatParameter( float* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter back;
            back.ParameterType = SQL_FLOAT;
            back.ValueType = SQL_C_FLOAT;
            back.ParameterValuePtr = pValuePointer;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = 0;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            return back;
        }
        static ISParameter CreateCharParameter(std::string* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            ISParameter back;
            int istrlen = strlen(pValuePointer->c_str());
            SQLCHAR* lstr = new SQLCHAR[istrlen];
            strcpy((char*)lstr, pValuePointer->c_str());
            back.LocalValuePtr = pValuePointer;
            back.ParameterType = SQL_CHAR;
            back.ValueType = SQL_C_CHAR;
            back.ParameterValuePtr = lstr;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = (SQLULEN)strlen;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            back.StrLen_or_IndPtr = SQL_NTS;
            return back;
        }
        static ISParameter CreateDateParameter(std::string* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {

            //  * https://docs.microsoft.com/en-us/sql/odbc/reference/appendixes/c-to-sql-timestamp?view=sql-server-ver15
            //  When timestamp C data is converted to character SQL data, the resulting character data is in the "yyyy-mm-dd hh:mm:ss[.f...]" format.



            ISParameter back;
            int istrlen = strlen(pValuePointer->c_str());
            SQLCHAR* lstr = new SQLCHAR[istrlen];
            strcpy((char*)lstr, pValuePointer->c_str());
            back.LocalValuePtr = pValuePointer;
            back.ParameterType = SQL_TYPE_TIMESTAMP;
            back.ValueType = SQL_C_CHAR;
            back.ParameterValuePtr = lstr;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = (SQLULEN)strlen;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            back.StrLen_or_IndPtr = SQL_NTS;
            return back;
        }
        static ISParameter CreateBoolParameter(bool* pValuePointer, SQLUSMALLINT pParamNumber, int pDirection)
        {
            /*
            * https://docs.microsoft.com/en-us/sql/odbc/reference/appendixes/c-to-sql-timestamp?view=sql-server-ver15
            When timestamp C data is converted to character SQL data, the resulting character data is in the "yyyy-mm-dd hh:mm:ss[.f...]" format.
            */


            ISParameter back;
            back.ParameterType = SQL_BIT;
            back.ValueType = SQL_C_BIT;
            back.ParameterValuePtr = pValuePointer;
            back.ParameterNumber = pParamNumber;
            back.InputOutputType = (PDirection)pDirection;
            back.ColumnSize = 1;
            back.DecimalDigits = 0;
            back.BufferLength = 0;
            return back;
        }
    };

    ISParameter()
    {

    }

    ~ISParameter()
    {
        if (LocalValuePtr)          // local varsa ParameterValuePtr içerde oluşmuştur, o yüzden ParameterValuePtr sil, local dışardan gelen
        {
            if (ParameterValuePtr)      //todo ?
            {
                // delete ParameterValuePtr;
                // ParameterValuePtr = nullptr;
            }

        }
    }

    void LoadLocalPointer() // out parametre ise geri dönüşü vermen gerek
    {
        if (ParameterType == SQL_CHAR)
        {
            if (LocalValuePtr != nullptr)
            {
                if (ParameterValuePtr != nullptr)
                {
                    *(std::string*)LocalValuePtr = "";
                    ((std::string*)LocalValuePtr)->append((char*)ParameterValuePtr);
                }
            }
        }
        delete ParameterValuePtr;
        ParameterValuePtr = nullptr;
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
    SQLCHAR        ColumnName[MaxColNameLen];
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



    std::string AsString()
    {
        std::ostringstream dtss;
        if (TargetType == SQL_C_CHAR)
            dtss << (char*)TargetValuePtr;
        else if (TargetType == SQL_C_SLONG || TargetType == SQL_C_LONG)
            dtss << *(int*)TargetValuePtr;
        else if (TargetType == SQL_C_TIMESTAMP)
        {
            TIMESTAMP_STRUCT ts = *(TIMESTAMP_STRUCT*)TargetValuePtr;
            tm t;
            t.tm_year = ts.year - 1900;
            t.tm_mon = ts.month - 1;
            t.tm_mday = ts.day;
            t.tm_hour = ts.hour;
            t.tm_min = ts.minute;
            t.tm_sec = ts.second;
            // t.tm_isdst = 0;
            // t.tm_wday = 0;
            // t.tm_yday = 0;
            std::string strtm = Globals::tmToStr(&t);
            dtss << strtm.c_str();

        }
        return dtss.str();
    }

    // todo ya value bool değilse ???
    bool AsBool()
    {
        bool back = false;
        if (!(TargetType == SQL_C_BIT)) return back;
        bool* bp = reinterpret_cast<bool*>(TargetValuePtr);
        back = *bp;
        return back;
    }

    int AsInt()
    {
        int back = 0;
        if (!(TargetType == SQL_C_SLONG || TargetType == SQL_C_LONG)) return back;
        int* ip = reinterpret_cast<int*>(TargetValuePtr);
        back = *ip;
        return back;
    }

    float AsFloat()
    {
        float back = 0.0;
        if (!(TargetType == SQL_C_FLOAT)) return back;
        float* ip = reinterpret_cast<float*>(TargetValuePtr);
        back = *ip;
        return back;
    }

    long long AsLongLong()
    {
        long long back = 0;
        if (!(TargetType == SQL_C_UBIGINT || TargetType == SQL_C_SBIGINT)) return back;
        long long* llp = reinterpret_cast<long long*>(TargetValuePtr);
        back = *llp;
        return back;
    }

    byte* AsByteP()
    {
        byte* bp = nullptr;
        bp = reinterpret_cast<byte*>(TargetValuePtr);
        return bp;
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
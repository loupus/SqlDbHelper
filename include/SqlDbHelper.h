#ifndef SQLDBHELPER_H_
#define SQLDBHELPER_H_

#include "DbClasses.hpp"

#ifdef _DLLEXPORT
#define CLASS_DECLSPEC    __declspec(dllexport)
#else
#define CLASS_DECLSPEC    __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

class ISqlDb
{
public:
    virtual bool DoAddParameter(void* pValue, dbRetType pType, unsigned short pNumber, int pDir, bool bResetParams = false) = 0;
    virtual bool DoInitiliaze(const char* pQuery, char* pErr, size_t pErrSize) = 0;
    virtual bool DoExecute(const char* pQuery, char* pErr, size_t pErrSize) = 0;
    virtual bool DoLoad(RowData** pRows) = 0;
    virtual void DoGetRowCount(size_t* pRowCount) = 0;
};

 CLASS_DECLSPEC ISqlDb*  _cdecl GetDbInstance();
 CLASS_DECLSPEC void _cdecl ReleaseDbInstance(ISqlDb* pInstance);

#ifdef __cplusplus
}
#endif

#endif // SQLDBHELPER_H_

#include "SqlDbHelper.h"
#include "SqlDb.hpp"

ISqlDb* GetDbInstance()
{
	ISqlDb* back = nullptr;
	back = new cSqlDb();
	return back;
}

void ReleaseDbInstance(ISqlDb* pInstance)
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}
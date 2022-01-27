#include "SqlDbHelper.h"
#include "SqlDb.hpp"

ISqlDb* GetDbInstance()
{
	ISqlDb* back = nullptr;
	back = new cSqlDb();
	return back;
}
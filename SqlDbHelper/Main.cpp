#include <iostream>
#include "SqlDb.hpp"



void ff()
{
	//cSqlDb db("Driver={SQL Server Native Client 11.0};Server=GAMORA;Database=ORADMASTER_2020_24_43_TEST;Uid=xxx;Pwd=xxx;Trusted_Connection=yes;");
	cSqlDb db("Driver={SQL Server Native Client 11.0};Server=GAMORA;Database=ORADMASTER_2020_24_43_TEST;Trusted_Connection=yes;");
	db.Doit();
}

int main()
{
	std::cout << "basliyor" << std::endl;	
	ff();
	return 0;
}
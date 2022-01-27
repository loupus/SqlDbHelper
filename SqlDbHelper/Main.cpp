//#include <iostream>
//#include "SqlDb.hpp"
//
//
//
//void ff()
//{
//	//cSqlDb db("Driver={SQL Server Native Client 11.0};Server=GAMORA;Database=ORADMASTER_2020_24_43_TEST;Uid=xxx;Pwd=xxx;Trusted_Connection=yes;");
//	const char* strCon = "Driver={ODBC Driver 17 for SQL Server};Server=GAMORA;Database=24_TICKER_2018_YENI;Trusted_Connection=yes;";
//		//cSqlDb db("Driver={ODBC Driver 17 for SQL Server};Server=GAMORA;Database=24_TICKER_2018_YENI;Trusted_Connection=yes;");
//	//	BackObject back = db.Doit();
//	//	if (!back.Success)
//	//		std::cout << back.ErrDesc;
//		cSqlDb db;
//		char Err[150];
//		bool back = false;
//		back = db.DoInitiliaze(strCon, Err, 150);
//		if (!back)
//			std::cout << Err << std::endl;
//
//		//int Param = 52452;
//		int* iParam = new int(52452);
//		//iParam = &Param;
//		db.DoAddParameter(iParam, dbRetType::db_Int, 1, true);
//
//		back = db.DoExecute("select ID,dSmall from Hakan where dInt = ?", Err, 150);
//		if (!back)
//			std::cout << Err << std::endl;
//}
//
//int main()
//{
//	std::cout << "basliyor" << std::endl;	
//	ff();
//	return 0;
//}
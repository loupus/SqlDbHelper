// SqlTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DbClasses.hpp"
#include "SqlDbHelper.h"

int main()
{
    std::cout << "Hello World!\n";
    ISqlDb* sqldb = GetDbInstance();
    char Err[250];
    int* iParam = nullptr;
    RowData** data = nullptr;
    size_t rowcount = 0;
    int fcount = 0;
    const char* strCon = "Driver={ODBC Driver 17 for SQL Server};Server=GAMORA;Database=24_TICKER_2018_YENI;Trusted_Connection=yes;";
    bool back = sqldb->DoInitiliaze(strCon,Err,250);
    if (!back)
    {
        std::cout << Err << std::endl;
        goto exit;
    }
    iParam = new int(52452);
    back = sqldb->DoAddParameter(iParam, dbRetType::db_Int, 1, true);
    if (!back)
    {       
        std::cout << Err << std::endl;
        goto exit;
    }
    back = sqldb->DoExecute("select ID,dSmall from Hakan where dInt = ?", Err, 250);
    if (!back)
    {
        std::cout << Err << std::endl;
        goto exit;
    }

    sqldb->DoGetRowCount(&rowcount);
    data = (RowData**)malloc(sizeof(RowData*) * rowcount);
    memset(data, 0, sizeof(RowData*) * rowcount);
    back = sqldb->DoLoad(data);
    if (!back)
    {
        std::cout << Err << std::endl;
        goto exit;
    }

   for (int i = 0; i< rowcount; i++)
    {
       std::cout << std::endl;
       std::cout << data[i]->RowNumber << ":: ";
       fcount = data[i]->Fields.size();
       for (int j = 0; j < fcount; j++)
       {
           std::cout << data[i]->Fields[j]->fieldName.c_str() << " : " << data[i]->Fields[j]->GetStrValue() << " - ";
       }
       std::cout << std::endl;
      //  std::cout << (*data)->Fields.at(0)->fieldName <<   std::endl;
      //  data++;
    }


        
exit:
   ReleaseDbInstance(sqldb);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

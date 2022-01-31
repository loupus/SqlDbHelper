// SqlTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DbClasses.h"
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
    data = (RowData**)calloc(rowcount,sizeof(RowData*));
   // memset(data, 0, sizeof(RowData*) * rowcount);
    back = sqldb->DoLoad(data);
    if (!back)
    {
        std::cout << Err << std::endl;
        goto exit;
    }


    // dump


    std::cout << "TotalNumberOfRows: " << rowcount << std::endl;


   for (int i = 0; i< rowcount; i++)
    {
       std::cout << std::endl;
       std::cout << data[i]->RowNumber << ":: ";
       fcount = data[i]->nFields;
       std::cout << "TotalNumberOfFields: " << fcount << std::endl;
       RowData* row = data[i];
       if (!row) continue;
       for (int j = 0; j < fcount; j++)
       {
           Field* fi = row->Fields[j];
           if (!fi) break;
           std::cout << fi->fname << " - ";
           switch (fi->ftype)
           {
           case dbRetType::db_String:
           {
               char* aa = fi->AsChar();
               std::cout << aa << " - ";
               break;
           }
           case dbRetType::db_Short:
           {
               short aa = fi->AsShort();
               std::cout << aa << " - ";
               break;
           }
           case dbRetType::db_Int:
           {
               int aa = fi->AsInt();
               std::cout << aa << " - ";
               break;
           }
           case dbRetType::db_Int64:
           {
               long long aa = fi->AsLongLong();
               std::cout << aa << " - ";
               break;
           }
          }
       }
      
       std::cout << std::endl;
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

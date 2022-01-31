#pragma once
#include <stdlib.h>
#include <string.h>



enum ParameterDirection
{
	_ParamUnkown = 0,
	_ParamInput = 1,
	_ParamInputOutput = 2,
	_ParamResultCol = 3,
	_ParamOutput = 4,
	_ParamReturnValue = 5
};

enum dbRetType
{
	db_Null,
	db_Short,
	db_Int,
	db_Int64,
	db_String,
	db_Float,
	db_Bool,
	db_Date,
	db_Byte
};

class Field
{
private :
	void* fdata = nullptr;
	void ClearData()
	{
		if (fdata)
		{
			free (fdata);
			fdata = nullptr;
		}
	}
public:
	char* fname;
	short fnumber;
	dbRetType ftype;
	size_t fdatalen = 0;

	Field()
	{

	}
	~Field()
	{
		ClearData();
	}

	void SetData(void* pData, size_t pDataLen)
	{
		ClearData();
		fdatalen = pDataLen;
		fdata = malloc(pDataLen);
		if (!fdata) return;
		memset(fdata, 0, pDataLen);
		memcpy(fdata, pData, pDataLen);
	}

	void* GetData()
	{
		return fdata;
	}

	char* AsChar()
	{
		return (char*)fdata;
	}

	short AsShort()
	{
		short back = 0;
		if (ftype == dbRetType::db_Short)
			back = *((short*)fdata);
		return back;
	}

	int AsInt()
	{
		int back = 0;
		if (ftype == dbRetType::db_Int)
			back = *((int*)fdata);
		return back;
	}

	long long AsLongLong()
	{
		long long back = 0;
		if (ftype ==dbRetType::db_Int64)
			back = *((long long*)fdata);
		return back;
	}

	float AsFloat()
	{
		float back = 0;
		if (ftype == dbRetType::db_Float)
			back = *((float*)fdata);
		return back;
	}

	bool AsBool()
	{
		bool back = false;
		if (ftype == dbRetType::db_Bool)
			back = *((float*)fdata);
		return back;
	}
};

class RowData
{
public:
	int RowNumber;
	int nFields;
	Field** Fields = nullptr;

	RowData(int pnFields)
	{
		nFields = pnFields;
		Fields = (Field**)malloc(nFields * sizeof(Field));
	}

	~RowData()
	{
		for (int i = 0; i < nFields; i++)
		{
			delete Fields[i];
			Fields[i] = nullptr;
		}
		nFields = 0;
	}

};
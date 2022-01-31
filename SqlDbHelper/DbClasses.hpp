//#pragma once
//#include <string>
//#include <vector>
//#include <sstream>
//
//enum ParameterDirection
//{
//	_ParamUnkown = 0,
//	_ParamInput = 1,
//	_ParamInputOutput = 2,
//	_ParamResultCol = 3,
//	_ParamOutput = 4,
//	_ParamReturnValue = 5
//};
//
//
//enum dbRetType
//{
//	db_Null,
//	db_Short,
//	db_Int,
//	db_Int64,
//	db_String,
//	db_Float,
//	db_Bool,
//	db_Date,
//	db_Byte
//};
//
//
//
//
//class IFieldData
//{
//public:
//	IFieldData() = default;
//	virtual ~IFieldData()
//	{
//
//	};
//	std::string fieldName;
//	dbRetType dbType;
//	int fnumber;
//	virtual std::string GetStrValue()
//	{
//		return "";
//	}
//
//};
//
//template<typename T>
//class FieldData : public IFieldData
//{
//private:
//	T value;
//
//public:
//	T GetValue()
//	{
//		return value;
//	}
//	void SetValue(T pvalue)
//	{
//		value = pvalue;
//	}
//
//	virtual std::string GetStrValue()
//	{
//		std::ostringstream dtss;
//		dtss << value;
//		return dtss.str();
//	}
//
//	virtual ~FieldData()
//	{
//
//	}
//};
//
//
//
//class RowData
//{
//public:
//	RowData() = default;
//	int RowNumber;
//	std::vector<IFieldData*> Fields;
//	~RowData()
//	{
//		for (auto f : Fields)
//			if (f)
//			{
//				delete f;
//				f = nullptr;
//			}		
//		Fields.clear();
//		Fields.shrink_to_fit();
//	}
//
//};
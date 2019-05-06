

>QueryResult.h
```cpp
#pragma once

#include <assert.h>
#include <mysql/mysql.h>
#include <stdint.h>
#include <vector>
#include <map>

#include "Field.h"

class  QueryResult
{
    public:
        typedef std::map<uint32_t, std::string> FieldNames;

        QueryResult(MYSQL_RES* result, uint64_t rowCount, uint32_t fieldCount);
        virtual ~QueryResult();

        virtual bool NextRow();

        uint32_t GetField_idx(const std::string& name) const
        {
            for(FieldNames::const_iterator iter = GetFieldNames().begin(); iter != GetFieldNames().end(); ++iter)
            {
                if(iter->second == name)
                    return iter->first;
            }

            assert(false && "unknown field name");
            return uint32_t(-1);
        }

        Field *Fetch() const { return mCurrentRow; }

        const Field & operator [] (int index) const 
        { 
            return mCurrentRow[index];
        }

        const Field & operator [] (const std::string &name) const
        {
            return mCurrentRow[GetField_idx(name)];
        }

        uint32_t GetFieldCount() const { return mFieldCount; }
        uint64_t GetRowCount() const { return mRowCount; }
        FieldNames const& GetFieldNames() const {return mFieldNames; }

        vector<string> const& GetNames() const {return m_vtFieldNames;}

    private:
        enum Field::DataTypes ConvertNativeType(enum_field_types mysqlType) const;
	public:
        void EndQuery();

    protected:
        Field *             mCurrentRow;
        uint32_t            mFieldCount;
        uint64_t            mRowCount;
        FieldNames          mFieldNames;
        std::vector<string> m_vtFieldNames;

		MYSQL_RES*          mResult;
};
```

>QueryResult.cpp
```cpp
#include "QueryResult.h"
#include "../base/Logging.h"

QueryResult::QueryResult(MYSQL_RES *result, uint64_t rowCount, uint32_t fieldCount)
 : mFieldCount(fieldCount), mRowCount(rowCount)
{
    mResult = result;
	mCurrentRow = new Field[mFieldCount];
    assert(mCurrentRow);

    MYSQL_FIELD *fields = mysql_fetch_fields(mResult);

    for (uint32_t i = 0; i < mFieldCount; i++)
    {
        //TODO: 这个地方要不要判断为NULL？
		if (fields[i].name != NULL)
        {
            mFieldNames[i] = fields[i].name;
            m_vtFieldNames.push_back(fields[i].name);
        }
        else
        {
            mFieldNames[i] = "";
            m_vtFieldNames.push_back("");
        }
        
        mCurrentRow[i].SetType(ConvertNativeType(fields[i].type));
    }
}

QueryResult::~QueryResult(void)
{
    EndQuery();
}

bool QueryResult::NextRow()
{
    MYSQL_ROW row;

    if (!mResult)
        return false;

    row = mysql_fetch_row(mResult);
    if (!row)
    {
        EndQuery();
        return false;
    }

    unsigned long int *ulFieldLength;
    ulFieldLength = mysql_fetch_lengths(mResult);
    for (uint32_t i = 0; i < mFieldCount; i++)
    {
        if(row[i] == NULL)
        {
            mCurrentRow[i].m_bNULL = true;
            mCurrentRow[i].SetValue("", 0);
        }
        else
        {
            mCurrentRow[i].m_bNULL = false;
           mCurrentRow[i].SetValue(row[i], ulFieldLength[i]);
        }

        mCurrentRow[i].SetName(mFieldNames[i]);
    }

    return true;
}

void QueryResult::EndQuery()
{
    if (mCurrentRow)
    {
        delete [] mCurrentRow;
        mCurrentRow = 0;
    }

    if (mResult)
    {
		//LOG_INFO << "QueryResult::EndQuery, mysql_free_result";
        mysql_free_result(mResult);
        mResult = 0;
    }
}

enum Field::DataTypes QueryResult::ConvertNativeType(enum_field_types mysqlType) const
{
    switch (mysqlType)
    {
        case FIELD_TYPE_TIMESTAMP:
        case FIELD_TYPE_DATE:
        case FIELD_TYPE_TIME:
        case FIELD_TYPE_DATETIME:
        case FIELD_TYPE_YEAR:
        case FIELD_TYPE_STRING:
        case FIELD_TYPE_VAR_STRING:
        case FIELD_TYPE_BLOB:
        case FIELD_TYPE_SET:
        case FIELD_TYPE_NULL:
            return Field::DB_TYPE_STRING;
        case FIELD_TYPE_TINY:

        case FIELD_TYPE_SHORT:
        case FIELD_TYPE_LONG:
        case FIELD_TYPE_INT24:
        case FIELD_TYPE_LONGLONG:
        case FIELD_TYPE_ENUM:
            return Field::DB_TYPE_INTEGER;
        case FIELD_TYPE_DECIMAL:
        case FIELD_TYPE_FLOAT:
        case FIELD_TYPE_DOUBLE:
            return Field::DB_TYPE_FLOAT;
        default:
            return Field::DB_TYPE_UNKNOWN;
    }
}
```
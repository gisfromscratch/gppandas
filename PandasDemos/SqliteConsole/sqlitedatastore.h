#pragma once

#include "stdafx.h"

using namespace std;

class SqliteDatastore {

public:
	SqliteDatastore(const char* path) : 
		_opened(false), 
		_inTransaction(false)
	{
		if (SQLITE_OK != sqlite3_open(path, &_db))
		{
			_opened = false;
		}
	}

	virtual ~SqliteDatastore()
	{
		if (_opened)
		{
			if (_inTransaction)
			{
				endTransaction();
			}

			for_each(_preparedStatements.begin(), _preparedStatements.end(), [&](sqlite3_stmt *statement) -> void {
				sqlite3_finalize(statement);
			});

			sqlite3_close(_db);
			_opened = false;
		}
	}

	bool isOpen()
	{
		return _opened;
	}

	bool beginTransaction()
	{
		if (_inTransaction)
		{
			return false;
		}

		if (exec("BEGIN TRANSACTION"))
		{
			_inTransaction = true;
			return true;
		}

		return false;
	}

	bool endTransaction()
	{
		if (!_inTransaction)
		{
			return false;
		}

		if (exec("END TRANSACTION"))
		{
			_inTransaction = false;
			return true;
		}

		return false;
	}

	bool exec(const char *sql)
	{
		int retVal = sqlite3_exec(_db, sql, 0, 0, &_errorMsg);
		if (SQLITE_OK == retVal)
		{
			return true;
		}

		sqlite3_free(_errorMsg);
		return false;
	}

	sqlite3_stmt* prepare(const char *sql)
	{
		sqlite3_stmt *statement;
		sqlite3_prepare(_db, sql, -1, &statement, 0);
		_preparedStatements.push_back(statement);
		return statement;
	}

private:
	sqlite3 *_db;
	bool _opened;
	bool _inTransaction;
	char *_errorMsg;
	vector<sqlite3_stmt*> _preparedStatements;
};
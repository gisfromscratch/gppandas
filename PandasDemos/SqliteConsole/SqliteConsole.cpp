// SqliteConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "sqlitedatastore.h"
#include "utf8filesource.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		return -1;
	}

	//setlocale(LC_ALL, "en_US.UTF-8");
	cout << "Trying to access " << argv[1] << endl;

	// Try to open the file
	Utf8FileSource file(argv[1]);
	if (!file.isOpen())
	{
		return -1;
	}

	// Create in memory DB
	SqliteDatastore dataStore("memory.db");
	if (dataStore.isOpen())
	{
		return -1;
	}

	// Create table
	if (!dataStore.exec("CREATE TABLE IF NOT EXISTS Geonames(ID INTEGER, Name TEXT);"))
	{		
		return -1;
	}

	// Begin transaction
	if (!dataStore.beginTransaction())
	{
		return -1;
	}

	const size_t sizeOfBom = 3;

	char *sqlInsertStatement = "INSERT INTO Geonames VALUES(@ID, @NAME)";
	sqlite3_stmt *preparedStatement = dataStore.prepare(sqlInsertStatement);
	if (!preparedStatement)
	{
		return -1;
	}
	
	bool skippedBom = false;
	const wchar_t *delimiter = L"\t";
	Utf8Line *utf8Line;
	char *token;
	while (utf8Line = file.nextLine())
	{
		if (token = utf8Line->nextToken(delimiter, true))
		{
			int id = atoi(token);
			int parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@ID");
			if (SQLITE_OK != sqlite3_bind_int(preparedStatement, parameterIndex, id))
			{
				return -1;
			}

			if (token = utf8Line->nextToken(delimiter, false))
			{
				parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@NAME");
				if (SQLITE_OK != sqlite3_bind_text(preparedStatement, parameterIndex, token, -1, SQLITE_TRANSIENT))
				{
					return -1;
				}

				// Insert record
				sqlite3_step(preparedStatement);
			}

			// Clear bindings
			sqlite3_clear_bindings(preparedStatement);

			// Reset the prepared statement
			sqlite3_reset(preparedStatement);
		}
	}

	// End transaction
	if (!dataStore.endTransaction())
	{
		return -1;
	}
    return 0;
}


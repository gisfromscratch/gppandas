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

	setlocale(LC_ALL, "en_US.utf8");
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

	// Drop table
	if (!dataStore.exec("DROP TABLE IF EXISTS Geonames"))
	{
		return -1;
	}

	// Create table
	if (!dataStore.exec("CREATE TABLE Geonames(GeonameId INTEGER, Name TEXT, ASCIIName TEXT, AlternateNames TEXT, Latitude REAL, Longitude REAL);"))
	{		
		return -1;
	}

	// Begin transaction
	if (!dataStore.beginTransaction())
	{
		return -1;
	}

	const size_t sizeOfBom = 3;

	char *sqlInsertStatement = "INSERT INTO Geonames VALUES(@ID, @NAME, @ASCIIName, @AlternateNames, @Latitude, @Longitude)";
	sqlite3_stmt *preparedStatement = dataStore.prepare(sqlInsertStatement);
	if (!preparedStatement)
	{
		return -1;
	}
	
	bool skippedBom = false;
	const wchar_t *delimiter = L"\t";
	Utf8Line *utf8Line;
	char *utf8Token;	
	while (utf8Line = file.nextLine())
	{
		if (utf8Token = utf8Line->nextToken(delimiter, true))
		{
			// GeonamesId
			__int64 id = _atoi64(utf8Token);
			int parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@ID");
			if (SQLITE_OK != sqlite3_bind_int64(preparedStatement, parameterIndex, id))
			{
				return -1;
			}

			// Name
			if (utf8Token = utf8Line->nextToken(delimiter, false))
			{
				parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@NAME");
				if (SQLITE_OK != sqlite3_bind_text(preparedStatement, parameterIndex, utf8Token, strlen(utf8Token), SQLITE_TRANSIENT))
				{
					return -1;
				}
			}

			// ASCIIName
			if (utf8Token = utf8Line->nextToken(delimiter, false))
			{
				parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@ASCIIName");
				if (SQLITE_OK != sqlite3_bind_text(preparedStatement, parameterIndex, utf8Token, strlen(utf8Token), SQLITE_TRANSIENT))
				{
					return -1;
				}
			}

			// AlternateNames
			if (utf8Token = utf8Line->nextToken(delimiter, false))
			{
				parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@AlternateNames");
				if (SQLITE_OK != sqlite3_bind_text(preparedStatement, parameterIndex, utf8Token, strlen(utf8Token), SQLITE_TRANSIENT))
				{
					return -1;
				}
			}

			// Latitude
			if (utf8Token = utf8Line->nextToken(delimiter, false))
			{
				double latitude = atof(utf8Token);
				parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@Latitude");
				if (SQLITE_OK != sqlite3_bind_double(preparedStatement, parameterIndex, latitude))
				{
					return -1;
				}
			}

			// Longitude
			if (utf8Token = utf8Line->nextToken(delimiter, false))
			{
				double longitude = atof(utf8Token);
				parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@Longitude");
				if (SQLITE_OK != sqlite3_bind_double(preparedStatement, parameterIndex, longitude))
				{
					return -1;
				}
			}

			// Insert record
			sqlite3_step(preparedStatement);

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


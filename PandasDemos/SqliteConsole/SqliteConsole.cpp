// SqliteConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		return -1;
	}

	// Set locale supporting UTF-8
	//setlocale(LC_ALL, "en_US.utf8");

	cout << "Trying to access " << argv[1] << endl;

	// Try to open the file
	FILE *file = fopen(argv[1], "rt+, ccs=UTF-8");
	if (!file)
	{
		return -1;
	}

	// Create in memory DB
	sqlite3 *db;
	int sqliteRetVal = sqlite3_open("memory.db", &db);
	if (SQLITE_OK != sqliteRetVal)
	{
		fclose(file);
		return -1;
	}

	// Create table
	char *errorMsg;
	sqliteRetVal = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Geonames(ID INTEGER, Name TEXT);", 0, 0, &errorMsg);
	if (SQLITE_OK != sqliteRetVal)
	{
		fclose(file);
		sqlite3_free(errorMsg);
		sqlite3_close(db);
		return -1;
	}

	// Begin transaction
	sqliteRetVal = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &errorMsg);
	if (SQLITE_OK != sqliteRetVal)
	{
		fclose(file);
		sqlite3_free(errorMsg);
		sqlite3_close(db);
		return -1;
	}

	const size_t sizeOfBom = 3;
	const size_t maxLineBuffer = 256;
	wchar_t line[maxLineBuffer];
	char utf8Line[2*maxLineBuffer];

	char *sqlInsertStatement = "INSERT INTO Geonames VALUES(@ID, @NAME)";
	sqlite3_stmt *preparedStatement;
	sqlite3_prepare(db, sqlInsertStatement, -1, &preparedStatement, 0);
	if (SQLITE_OK != sqliteRetVal)
	{
		fclose(file);
		sqlite3_free(errorMsg);
		sqlite3_close(db);
		return -1;
	}
	
	bool skippedBom = false;
	const char *delimiter = "\t";
	while (fgetws(line, maxLineBuffer, file))
	{
		cout << line << endl;
		wcstombs(utf8Line, line, sizeof(utf8Line));
		cout << utf8Line << endl;
		//if (!skippedBom && utf8::starts_with_bom(line.begin(), line.end()))
		//{
		//	line = string(line.begin() + sizeOfBom, line.end());
		//	skippedBom = true;
		//}

		//// Convert it to utf-16
		//vector<unsigned short> utf16line;
		//utf8::utf8to16(line.begin(), line.end(), back_inserter(utf16line));

		//// And back to utf-8
		//string utf8line;
		//utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

		//size_t delimiterPos = line.find(delimiter);
		//if (string::npos != delimiterPos && 0 != delimiterPos)
		//{
		//	string token = line.substr(0, delimiterPos - 1);
		//	int id = atoi(token.c_str());
		//	int parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@ID");
		//	sqliteRetVal = sqlite3_bind_int(preparedStatement, parameterIndex, id);
		//	if (SQLITE_OK != sqliteRetVal)
		//	{
		//		fclose(file);
		//		sqlite3_free(errorMsg);
		//		sqlite3_finalize(preparedStatement);
		//		sqlite3_close(db);
		//		return -1;
		//	}

		//	size_t afterDelimiterPos = delimiterPos + 1;
		//	if (afterDelimiterPos < line.length())
		//	{
		//		delimiterPos = line.find(delimiter, afterDelimiterPos);
		//		if (string::npos != delimiterPos && 0 < delimiterPos - afterDelimiterPos)
		//		{
		//			token = line.substr(afterDelimiterPos, delimiterPos - afterDelimiterPos - 1);
		//			parameterIndex = sqlite3_bind_parameter_index(preparedStatement, "@NAME");
		//			sqliteRetVal = sqlite3_bind_text(preparedStatement, parameterIndex, token.c_str(), -1, SQLITE_TRANSIENT);
		//			if (SQLITE_OK != sqliteRetVal)
		//			{
		//				fclose(file);
		//				sqlite3_free(errorMsg);
		//				sqlite3_finalize(preparedStatement);
		//				sqlite3_close(db);
		//				return -1;
		//			}

		//			// Insert record
		//			sqlite3_step(preparedStatement);

		//			// Clear bindings
		//			sqlite3_clear_bindings(preparedStatement);

		//			// Reset the prepared statement
		//			sqlite3_reset(preparedStatement);
		//		}
		//	}
		//}

		/*string::iterator utf8End = utf8::find_invalid(line.begin(), line.end());
		if (line.end() != utf8End)
		{
			line = string(line.begin(), utf8End);
		}

		utf8::replace_invalid(line.begin(), line.end(), back_inserter(utf8Line));
		cout << utf8Line << endl;
		*/
	}

	// End transaction
	sqliteRetVal = sqlite3_exec(db, "END TRANSACTION", 0, 0, &errorMsg);
	if (SQLITE_OK != sqliteRetVal)
	{
		fclose(file);
		sqlite3_free(errorMsg);
		sqlite3_finalize(preparedStatement);
		sqlite3_close(db);
		return -1;
	}

	fclose(file);
	sqlite3_finalize(preparedStatement);
	sqlite3_close(db);
    return 0;
}


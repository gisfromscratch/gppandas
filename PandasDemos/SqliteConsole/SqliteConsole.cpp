// SqliteConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
	// Create in memory DB
	sqlite3 *db;
	int errCode = sqlite3_open(NULL, &db);
	if (errCode)
	{
		return -1;
	}

	sqlite3_close(db);
    return 0;
}


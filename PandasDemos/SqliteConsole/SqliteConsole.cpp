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

	cout << "Trying to access " << argv[1] << endl;

	// Try to open the file
	ifstream file(argv[1]);
	if (!file)
	{
		return -1;
	}

	const size_t sizeOfBom = 3;
	string line;
	string utf8Line;
	bool skippedBom = false;
	while (std::getline(file, line))
	{
		if (!skippedBom && utf8::starts_with_bom(line.begin(), line.end()))
		{
			line = string(line.begin() + sizeOfBom, line.end());
			skippedBom = true;
		}
		
		/*string::iterator utf8End = utf8::find_invalid(line.begin(), line.end());
		if (line.end() != utf8End)
		{
			line = string(line.begin(), utf8End);
		}

		utf8::replace_invalid(line.begin(), line.end(), back_inserter(utf8Line));
		cout << utf8Line << endl;
		*/
	}

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


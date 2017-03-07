// NativeConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

int main(int argc, char *argv[], char *envp[])
{
	if (argc < 2)
	{
		return - 1;
	}

	cout << "Trying to access " << argv[1] << endl;

	// Try to open the file
	ifstream input(argv[1]);
	if (!input)
	{
		return -1;
	}

	// Read the file into memory
	ostringstream stringstream;
	stringstream << input.rdbuf();
	string fileContent = stringstream.str();

	cout << fileContent.size() << " characters." << endl;
	system("PAUSE");
    return 0;
}


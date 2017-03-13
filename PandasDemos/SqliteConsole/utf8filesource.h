#pragma once
#include "stdafx.h"

class Utf8FileSource {

public:
	Utf8FileSource(const char *filePath)
	{
		_file = fopen(filePath, "rt+, ccs=UTF-8");
		_line = (wchar_t*) malloc(sizeof(wchar_t) * MaxLineBuffer);
		_utf8Line = (char*) malloc(sizeof(char) * 2 * MaxLineBuffer);
	}
 
	~Utf8FileSource()
	{
		if (_file)
		{
			fclose(_file);
		}
		if (_line)
		{
			free(_line);
			_line = 0;
		}
		if (_utf8Line)
		{
			free(_utf8Line);
			_utf8Line = 0;
		}
	}

	bool isOpen()
	{
		return _file;
	}

	char* nextLine()
	{
		if (!_file || !_line || !_utf8Line)
		{
			return 0;
		}

		if (!fgetws(_line, MaxLineBuffer, _file))
		{
			return 0;
		}

		if (!wcstombs(_utf8Line, _line, 2 * MaxLineBuffer))
		{
			return 0;
		}

		return _utf8Line;
	}

private:
	FILE *_file;
	const size_t MaxLineBuffer = 1024;
	
	wchar_t *_line;
	char *_utf8Line;
};
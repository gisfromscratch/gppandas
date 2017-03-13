#pragma once
#include "stdafx.h"

class Utf8Line {

public:
	Utf8Line()
	{
		_line = (wchar_t*) malloc(sizeof(wchar_t) * MaxLineBuffer);
		_utf8Line = (char*) malloc(sizeof(char) * 2 * MaxLineBuffer);
		_nextToken = (char*) malloc(sizeof(char) * 2 * MaxLineBuffer);
	}

	~Utf8Line()
	{
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

	char* nextToken(const wchar_t *delimiter, bool init)
	{
		/*wchar_t wideDelimiter[1024];
		if (!mbstowcs(wideDelimiter, delimiter, 1024))
		{
			return 0;
		}*/

		wchar_t *token;
		if (init)
		{
			token = wcstok(_line, delimiter);
		}
		else
		{
			token = wcstok(NULL, delimiter);
		}
		if (!token)
		{
			return 0;
		}

		if (!wcstombs(_nextToken, token, 2 * MaxLineBuffer))
		{
			return 0;
		}
		return _nextToken;
	}

	const size_t MaxLineBuffer = 1024;
	wchar_t *_line;
	char *_utf8Line;
	char *_nextToken;
};

class Utf8FileSource {

public:
	Utf8FileSource(const char *filePath)
	{
		_file = fopen(filePath, "rt+, ccs=UTF-8");
		_line = new Utf8Line();
	}
 
	~Utf8FileSource()
	{
		if (_file)
		{
			fclose(_file);
		}
		if (_line)
		{
			delete _line;
		}
	}

	bool isOpen()
	{
		return _file;
	}

	Utf8Line* nextLine()
	{
		if (!_file || !_line)
		{
			return 0;
		}

		if (!fgetws(_line->_line, _line->MaxLineBuffer, _file))
		{
			return 0;
		}

		if (!wcstombs(_line->_utf8Line, _line->_line, 2 * _line->MaxLineBuffer))
		{
			return 0;
		}

		return _line;
	}

private:
	FILE *_file;
	Utf8Line *_line;
};
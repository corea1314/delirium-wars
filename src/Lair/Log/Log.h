#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

class LogMan
{
public:
	LogMan();
	virtual	~LogMan();
	void Log( const char* in_szModule, const char* in_szLog, ... );

private:
	FILE*	m_pLogFile;
};

#endif//_LOG_H
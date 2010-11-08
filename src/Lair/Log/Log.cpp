
#include "Log.h"

#include <stdarg.h>

#define LOG_FILENAME	"system.log"

LogMan::LogMan() : m_pLogFile(0)
{
	fopen_s( &m_pLogFile, LOG_FILENAME, "w" );
}

LogMan::~LogMan()
{
	if(m_pLogFile)
		fclose(m_pLogFile);
}

void LogMan::Log( const char* in_szModule, const char* in_szLog, ... )
{
	fputs( in_szModule, m_pLogFile );
	va_list marker;
	va_start( marker, in_szLog );
	vfprintf( m_pLogFile, in_szLog, marker );
	va_end(marker);

	fflush( m_pLogFile );
}
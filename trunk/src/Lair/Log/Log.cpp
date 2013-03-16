
#include "Log.h"

#include <stdarg.h>

#define LOG_FILENAME	"system.log"

LogMan::LogMan() : m_pLogFile(0)
{
	fopen_s( &m_pLogFile, LOG_FILENAME, "w+" );
}

LogMan::~LogMan()
{
	if(m_pLogFile)
		fclose(m_pLogFile);
}

void LogMan::Log( const char* in_szModule, const char* in_szLog, ... )
{
	fprintf( m_pLogFile, "(%s): ", in_szModule );
	va_list marker;
	va_start( marker, in_szLog );
	vfprintf( m_pLogFile, in_szLog, marker );	// log file output
	vprintf( in_szLog, marker );				// console output
	va_end(marker);
	fputs("\n",m_pLogFile);
	puts("\n");

	fflush( m_pLogFile );
}
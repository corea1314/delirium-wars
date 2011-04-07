#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <map>
#include <string>

class ScriptParser
{
public:
	ScriptParser() : m_szSeparators(" ,\t\n"), m_szToken(0), m_szNextToken(0)
	{
	}

	void SetScript( const std::string& in_szScript )
	{
		m_szNextToken = 0;
		strcpy_s( m_szScript, 128, in_szScript.c_str() );
		m_szToken = strtok_s( m_szScript, m_szSeparators, &m_szNextToken);
	}

	bool ReadInt( int& out_nValue )
	{
		if( m_szToken == 0 )
			return false;
		out_nValue = atoi( m_szToken );
		m_szToken = strtok_s( 0, m_szSeparators, &m_szNextToken);
		return true;
	}
	bool ReadFloat( float& out_fValue )
	{
		if( m_szToken == 0 )
			return false;
		out_fValue = (float)atof(m_szToken);
		m_szToken = strtok_s( 0, m_szSeparators, &m_szNextToken);
		return true;
	}
	bool ReadString( std::string& out_szValue )
	{
		if( m_szToken == 0 )
			return false;
		out_szValue = m_szToken;
		m_szToken = strtok_s( 0, m_szSeparators, &m_szNextToken);
		return true;
	}

private:
	char*		m_szToken;
	char*		m_szNextToken;
	const char*	m_szSeparators;
	char		m_szTemp[32];
	char		m_szScript[128];
};

class Shell
{
public:
	virtual void RegisterCommand( const std::string& in_szCommand ) = 0;
	virtual void ExecuteCommand( const std::string& in_szParams ) = 0;
};

class Console
{
public:
	Console() : m_bActive(false) {}

	void Toggle();
	void Execute();

	void ProcessKey( unsigned char c );
	void Render();
	
private:
	bool	m_bActive;

	std::map<std::string,Shell*>	m_mapShell;
};


#endif//_CONSOLE_H

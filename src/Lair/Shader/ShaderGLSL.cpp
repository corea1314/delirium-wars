#include "ShaderGLSL.h"

#include "glee/GLee.h"

#include "../../macros.h"

#include "../../Lair/Lair.h"

#include <stdio.h>

const char*	ShaderGLSL::ms_szLoadFilename = 0;

char* GetBufferFromFile( const char* in_szFilename )
{
	FILE* pFile;
	if( fopen_s( &pFile, in_szFilename, "rt" ) == 0 )
	{
		fseek( pFile, 0, SEEK_END );
		unsigned int nSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		char* pBuffer = new char[nSize+1];
		fread( pBuffer, sizeof(char), nSize, pFile );
		fclose( pFile );

		pBuffer[nSize] = '\0';

		return pBuffer;				
	}

	return 0;
}


ShaderGLSL::ShaderGLSL()
{
	m_iVertShader = 0;
	m_iFragShader = 0;
	m_iProgram = 0;
	m_iUniformSampler[0] = m_iUniformSampler[1] = m_iUniformSampler[2] = m_iUniformSampler[3] = 0;

	m_iUniformDeltaTime = 0;
	m_iUniformTotalTime = 0;
}

ShaderGLSL::~ShaderGLSL()
{
	Destroy();
}

void ShaderGLSL::Bind() 
{
	glUseProgram( m_iProgram );

	glUniform1i( m_iUniformSampler[0], 0 );
	glUniform1i( m_iUniformSampler[1], 1 );
	glUniform1i( m_iUniformSampler[2], 2 );
	glUniform1i( m_iUniformSampler[3], 3 );

	float fTotalTime = 0.0f;
	glUniform1f( m_iUniformDeltaTime, 0.015f );
	glUniform1f( m_iUniformTotalTime, fTotalTime ); //fixme use system timer
}

void ShaderGLSL::Unbind() 
{
	glUseProgram(0);
}

bool ShaderGLSL::Create( const char* in_szFilename ) 
{ 	
	char szFilenameVS[MAX_PATH], szFilenameFS[MAX_PATH];
	
	#define _GLSL_CLEANUP	\
		SAFE_DELETE_ARRAY(vs);						\
		SAFE_DELETE_ARRAY(fs);						\
		if( m_iProgram && m_iVertShader )	glDetachShader(m_iProgram,m_iVertShader);	\
		if( m_iProgram && m_iFragShader )	glDetachShader(m_iProgram,m_iFragShader);	\
		if( m_iVertShader )	glDeleteShader(m_iVertShader);	\
		if( m_iFragShader )	glDeleteShader(m_iFragShader);	\
		if( m_iProgram )	glDeleteProgram(m_iProgram);

	char *vs = 0,*fs = 0;

	sprintf_s( szFilenameVS, "%s.vs", in_szFilename );
	if( vs = GetBufferFromFile(szFilenameVS) )
	{
		Lair::GetLogMan()->Log( "ShaderGLSL", "ShaderGLSL - Could not load vertex shader named \"%s\"", szFilenameVS );
		_GLSL_CLEANUP;
		return false;
	}
		
	sprintf_s( szFilenameFS, "%s.fs", in_szFilename );
	if( fs = GetBufferFromFile(szFilenameFS) )
	{
		Lair::GetLogMan()->Log( "ShaderGLSL", "ShaderGLSL - Could not load fragment shader named \"%s\"", szFilenameFS );
		_GLSL_CLEANUP;
		return false;
	}
	
	m_iVertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSourceARB(m_iVertShader, 1, (const char**)&vs, NULL);
	glCompileShader(m_iVertShader);
	
	m_iFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSourceARB(m_iFragShader, 1, (const char**)&fs, NULL);
	glCompileShader(m_iFragShader);

	SAFE_DELETE_ARRAY(fs);
	SAFE_DELETE_ARRAY(vs);

	if( ValidateShader(m_iVertShader,szFilenameVS) == false || 
		ValidateShader(m_iFragShader,szFilenameFS) == false )
	{
		_GLSL_CLEANUP;
		return false;
	}

	m_iProgram = glCreateProgram();
	
	glAttachShader(m_iProgram,m_iVertShader);
	glAttachShader(m_iProgram,m_iFragShader);

	glLinkProgram(m_iProgram);
	glValidateProgram(m_iProgram);

	vs = 0;
	fs = 0;
	
	if( ValidateProgram(m_iProgram) == false )
	{
		_GLSL_CLEANUP;
		return false;
	}

	m_iUniformSampler[0] = glGetUniformLocationARB( m_iProgram, "textureMap0" );
	m_iUniformSampler[1] = glGetUniformLocationARB( m_iProgram, "textureMap1" );
	m_iUniformSampler[2] = glGetUniformLocationARB( m_iProgram, "textureMap2" );
	m_iUniformSampler[3] = glGetUniformLocationARB( m_iProgram, "textureMap3" );

	m_iUniformDeltaTime = glGetUniformLocationARB( m_iProgram, "uDeltaTime" );
	m_iUniformTotalTime = glGetUniformLocationARB( m_iProgram, "uTotalTime" );
	
	return true;
}

void ShaderGLSL::Destroy()
{
	if( m_iProgram && m_iVertShader )
		glDetachShader(m_iProgram,m_iVertShader);
	if( m_iProgram && m_iFragShader )
		glDetachShader(m_iProgram,m_iFragShader);

	if( m_iVertShader )
		glDeleteShader(m_iVertShader);
	if( m_iFragShader )
		glDeleteShader(m_iFragShader);
	
	if( m_iProgram )
		glDeleteProgram(m_iProgram);
}

bool ShaderGLSL::ValidateShader( unsigned int in_iShader, const char* in_szShaderName )
{
	int nInfologLength = 0;
	int nCharsWritten  = 0;
	char * szInfoLog;

	int nCompileResult;

	glGetShaderiv( in_iShader, GL_COMPILE_STATUS, &nCompileResult );
	if( nCompileResult == 0 )
	{
		glGetShaderiv( in_iShader, GL_INFO_LOG_LENGTH, &nInfologLength );

		if (nInfologLength > 1)
		{
			szInfoLog = new char[nInfologLength];
			glGetShaderInfoLog( in_iShader, nInfologLength, &nCharsWritten, szInfoLog );
			Lair::GetLogMan()->Log( "ShaderGLSL", "ValidateShader(%s, %s) - Error(s) detected:\n%s", ms_szLoadFilename, in_szShaderName, szInfoLog );
			delete[] szInfoLog;
			return false;
		}
	}
	return true;
}

bool ShaderGLSL::ValidateProgram(unsigned int in_iProgram)
{
	int nInfologLength = 0;
	int nCharsWritten  = 0;
	char * szInfoLog;

	int nLinkResult;

	glGetProgramiv( in_iProgram, GL_LINK_STATUS, &nLinkResult );

	if( nLinkResult == 0 )
	{
		glGetProgramiv( in_iProgram, GL_INFO_LOG_LENGTH, &nInfologLength );

		if (nInfologLength > 1)
		{
			szInfoLog = new char[nInfologLength];
			glGetProgramInfoLog( in_iProgram, nInfologLength, &nCharsWritten, szInfoLog );
			Lair::GetLogMan()->Log( "ShaderGLSL", "ValidateProgram(%s) - Error(s) detected:\n%", ms_szLoadFilename, szInfoLog);
			delete[] szInfoLog;
			return false;
		}
	}

	return true;
}



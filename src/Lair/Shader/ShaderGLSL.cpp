#include "ShaderGLSL.h"

#include "glee/GLee.h"

#include "../../macros.h"

#include "../../Lair/Lair.h"

#include <stdio.h>

char* GetBufferFromFile( const char* in_szFilename )
{
	FILE* pFile;
	if( fopen_s( &pFile, in_szFilename, "rb" ) == 0 )
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
	m_iGeomShader = 0;
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

//	float fTotalTime = 0.0f;
//	glUniform1f( m_iUniformDeltaTime, Lair::Get );
//	glUniform1f( m_iUniformTotalTime, fTotalTime ); //fixme use system timer
}

void ShaderGLSL::Unbind() 
{
	glUseProgram(0);
}

bool ShaderGLSL::CreateShader( unsigned int & in_iShader, unsigned int in_eShaderType, const char* in_szFilename )
{
	char *s = 0;
	if( (s = GetBufferFromFile(in_szFilename)) == 0 )
	{
		Lair::GetLogMan()->Log( "ShaderGLSL", "Could not load shader named \"%s\"", in_szFilename );
		SAFE_DELETE_ARRAY(s);
		return false;
	}

	in_iShader = glCreateShader(in_eShaderType);
	glShaderSourceARB(in_iShader, 1, (const char**)&s, NULL);
	glCompileShader(in_iShader);

	if( ValidateShader(in_iShader,in_szFilename) == false )
	{
		SAFE_DELETE_ARRAY(s);
		return false;
	}
	SAFE_DELETE_ARRAY(s);
	return true;
}

bool ShaderGLSL::Create( const char* in_szFilename, unsigned int in_eInPrimType, unsigned int in_eOutPrimType, unsigned int in_nMaxOutVertices ) 
{ 	
	char szFilename[MAX_PATH];
		
	sprintf_s( szFilename, "%s.vs", in_szFilename );
	if( !CreateShader( m_iVertShader, GL_VERTEX_SHADER, szFilename ) )
	{
		Destroy();
		return false;
	}
	Lair::GetLogMan()->Log( "ShaderGLSL", "Load vertex shader named \"%s\"", in_szFilename );
	
	sprintf_s( szFilename, "%s.fs", in_szFilename );
	if( !CreateShader( m_iFragShader, GL_FRAGMENT_SHADER, szFilename ) )
	{
		Destroy();
		return false;
	}
	Lair::GetLogMan()->Log( "ShaderGLSL", "Loaded fragment shader named \"%s\"", in_szFilename );
	
	sprintf_s( szFilename, "%s.gs", in_szFilename );
	CreateShader( m_iGeomShader, GL_GEOMETRY_SHADER_EXT, szFilename );	//might not be present
	
	m_iProgram = glCreateProgram();
	
	glAttachShader(m_iProgram,m_iVertShader);
	glAttachShader(m_iProgram,m_iFragShader);

	if( m_iGeomShader )
	{
		glAttachShader(m_iProgram,m_iGeomShader);
		glProgramParameteriEXT(m_iProgram,GL_GEOMETRY_INPUT_TYPE_EXT,in_eInPrimType);
		glProgramParameteriEXT(m_iProgram,GL_GEOMETRY_OUTPUT_TYPE_EXT,in_eOutPrimType);
		glProgramParameteriEXT(m_iProgram,GL_GEOMETRY_VERTICES_OUT_EXT, in_nMaxOutVertices );

		Lair::GetLogMan()->Log( "ShaderGLSL", "Loaded geometry shader named \"%s\"", in_szFilename );
	}
	
	glLinkProgram(m_iProgram);
	if( LinkStatus(m_iProgram, in_szFilename ) == false )
	{
		Destroy();
		return false;
	}

	glValidateProgram(m_iProgram);

	if( ValidateProgram(m_iProgram, in_szFilename ) == false )
	{
		Destroy();
		return false;
	}

	Lair::GetLogMan()->Log( "ShaderGLSL", "Loaded program named \"%s\"", in_szFilename );

	m_iUniformSampler[0] = glGetUniformLocationARB( m_iProgram, "uTextureMap0" );
	m_iUniformSampler[1] = glGetUniformLocationARB( m_iProgram, "uTextureMap1" );
	m_iUniformSampler[2] = glGetUniformLocationARB( m_iProgram, "uTextureMap2" );
	m_iUniformSampler[3] = glGetUniformLocationARB( m_iProgram, "uTextureMap3" );

//	m_iUniformDeltaTime = glGetUniformLocationARB( m_iProgram, "uDeltaTime" );
//	m_iUniformTotalTime = glGetUniformLocationARB( m_iProgram, "uTotalTime" );
		
	return true;
}

void ShaderGLSL::Destroy()
{
	if( m_iProgram && m_iVertShader )	glDetachShader(m_iProgram,m_iVertShader);
	if( m_iProgram && m_iFragShader )	glDetachShader(m_iProgram,m_iFragShader);
	if( m_iProgram && m_iGeomShader )	glDetachShader(m_iProgram,m_iGeomShader);
	if( m_iVertShader )	{ glDeleteShader(m_iVertShader); m_iVertShader = 0;	}
	if( m_iFragShader )	{ glDeleteShader(m_iFragShader); m_iFragShader = 0;	}
	if( m_iGeomShader )	{ glDeleteShader(m_iGeomShader); m_iGeomShader = 0;	}
	if( m_iProgram )	{ glDeleteProgram(m_iProgram); m_iProgram = 0; }
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
			Lair::GetLogMan()->Log( "ShaderGLSL", "ValidateShader(%s) - Error(s) detected:\n%s", in_szShaderName, szInfoLog );
			delete[] szInfoLog;
			return false;
		}
	}
	return true;
}

bool ShaderGLSL::LinkStatus(unsigned int in_iProgram, const char* in_szFilename )
{
	int nInfologLength = 0;
	int nCharsWritten  = 0;
	char * szInfoLog;

	int nValidateStatus;
	glGetProgramiv( in_iProgram, GL_VALIDATE_STATUS, &nValidateStatus );

	if( nValidateStatus == 0 )
	{
		glGetProgramiv( in_iProgram, GL_INFO_LOG_LENGTH, &nInfologLength );

		if (nInfologLength > 1)
		{
			szInfoLog = new char[nInfologLength];
			glGetProgramInfoLog( in_iProgram, nInfologLength, &nCharsWritten, szInfoLog );
			Lair::GetLogMan()->Log( "ShaderGLSL", "LinkStatus(%s) - Error(s) detected:\n%", in_szFilename, szInfoLog);
			delete[] szInfoLog;
			return false;
		}
	}
	return true;
}

bool ShaderGLSL::ValidateProgram(unsigned int in_iProgram, const char* in_szFilename )
{
	int nInfologLength = 0;
	int nCharsWritten  = 0;
	char * szInfoLog;

	int nValidateStatus;
	glGetProgramiv( in_iProgram, GL_VALIDATE_STATUS, &nValidateStatus );

	if( nValidateStatus == 0 )
	{
		glGetProgramiv( in_iProgram, GL_INFO_LOG_LENGTH, &nInfologLength );

		if (nInfologLength > 1)
		{
			szInfoLog = new char[nInfologLength];
			glGetProgramInfoLog( in_iProgram, nInfologLength, &nCharsWritten, szInfoLog );
			Lair::GetLogMan()->Log( "ShaderGLSL", "ValidateProgram(%s) - Error(s) detected:\n%", in_szFilename, szInfoLog);
			delete[] szInfoLog;
			return false;
		}
	}

	return true;
}



#include "ShaderGLSL.h"

#include "glee/GLee.h"

#include "macros.h"

#include "Lair/Lair.h"

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
		Lair::GetLogMan()->Log( "ShaderGLSL", "Could not load shader named \"%s\".", in_szFilename );
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


void gl_Uniform1fv(GLint location, GLsizei count, const GLfloat * value) {	glUniform1fv( location, count, value ); }
void gl_Uniform2fv(GLint location, GLsizei count, const GLfloat * value) {	glUniform2fv( location, count, value ); }
void gl_Uniform3fv(GLint location, GLsizei count, const GLfloat * value) {	glUniform3fv( location, count, value ); }
void gl_Uniform4fv(GLint location, GLsizei count, const GLfloat * value) {	glUniform4fv( location, count, value ); }

void gl_UniformMatrix2fv (GLint location, GLsizei count, const GLfloat * value) { glUniformMatrix2fv(location, count, false, value); }
void gl_UniformMatrix3fv (GLint location, GLsizei count, const GLfloat * value) { glUniformMatrix3fv(location, count, false, value); }
void gl_UniformMatrix4fv (GLint location, GLsizei count, const GLfloat * value) { glUniformMatrix4fv(location, count, false, value); }


bool ShaderGLSL::Create( const char* in_szFilename, unsigned int in_eInPrimType, unsigned int in_eOutPrimType, unsigned int in_nMaxOutVertices ) 
{ 	
	char szFilename[MAX_PATH];
		
	sprintf_s( szFilename, "%s.vs", in_szFilename );
	if( !CreateShader( m_iVertShader, GL_VERTEX_SHADER, szFilename ) )
	{
		Destroy();
		return false;
	}
	Lair::GetLogMan()->Log( "ShaderGLSL", "Load vertex shader named \"%s\".", in_szFilename );
	
	sprintf_s( szFilename, "%s.fs", in_szFilename );
	if( !CreateShader( m_iFragShader, GL_FRAGMENT_SHADER, szFilename ) )
	{
		Destroy();
		return false;
	}
	Lair::GetLogMan()->Log( "ShaderGLSL", "Loaded fragment shader named \"%s\".", in_szFilename );
	
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

		Lair::GetLogMan()->Log( "ShaderGLSL", "Loaded geometry shader named \"%s\".", in_szFilename );
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

	Lair::GetLogMan()->Log( "ShaderGLSL", "Loaded program named \"%s\".", in_szFilename );

	m_iUniformSampler[0] = glGetUniformLocationARB( m_iProgram, "uTextureMap0" );
	m_iUniformSampler[1] = glGetUniformLocationARB( m_iProgram, "uTextureMap1" );
	m_iUniformSampler[2] = glGetUniformLocationARB( m_iProgram, "uTextureMap2" );
	m_iUniformSampler[3] = glGetUniformLocationARB( m_iProgram, "uTextureMap3" );

	GLint nActiveUniformCount = 0;
	glGetProgramiv( m_iProgram, GL_ACTIVE_UNIFORMS, &nActiveUniformCount ); 
	
	for( int i=0; i<nActiveUniformCount; i++ )
	{
		GLsizei length;
		GLint size;  
		GLenum type;

		glGetActiveUniform( m_iProgram, i, MAX_PATH, &length, &size, &type, szFilename );
		szFilename[length] = '\0';

		unsigned int nLocation = glGetUniformLocation( m_iProgram, szFilename );

		switch( type )
		{
		case GL_FLOAT:		m_mapUniform.insert( std::make_pair(szFilename, new Uniform( nLocation, size, 1, &gl_Uniform1fv ) ) );	break;																	   
		case GL_FLOAT_VEC2:	m_mapUniform.insert( std::make_pair(szFilename, new Uniform( nLocation, size, 2, &gl_Uniform2fv ) ) );	break;																	   
		case GL_FLOAT_VEC3:	m_mapUniform.insert( std::make_pair(szFilename, new Uniform( nLocation, size, 3, &gl_Uniform3fv ) ) );	break;																	   
		case GL_FLOAT_VEC4:	m_mapUniform.insert( std::make_pair(szFilename, new Uniform( nLocation, size, 4, &gl_Uniform4fv ) ) );	break;
			/*
		case GL_INT:
		case GL_INT_VEC2:
		case GL_INT_VEC3:
		case GL_INT_VEC4:
		case GL_BOOL:
		case GL_BOOL_VEC2:
		case GL_BOOL_VEC3:
		case GL_BOOL_VEC4:
			break;
			*/
			/*
		case GL_FLOAT_MAT2:
			m_mapUniform.insert( std::make_pair(szFilename, new UniformMatrix( glGetUniformLocation( m_iProgram, szFilename ), size, &gl_UniformMatrix2fv ) ) );
			break;
		case GL_FLOAT_MAT3:
			m_mapUniform.insert( std::make_pair(szFilename, new UniformMatrix( glGetUniformLocation( m_iProgram, szFilename ), size, &gl_UniformMatrix3fv ) ) );
			break;
			*/
		case GL_FLOAT_MAT4:
			m_mapUniform.insert( std::make_pair(szFilename, new Uniform( nLocation, size, 16, &gl_UniformMatrix4fv ) ) );
			break;
			
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			break;

		default:
			Lair::GetLogMan()->Log( "ShaderGLSL", "Uniform named %s is not yet implemented!", szFilename );
			assert(!"Not yet implemented!");
			break;
		}
	}

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



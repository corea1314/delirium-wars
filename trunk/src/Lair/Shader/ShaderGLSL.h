#ifndef _SHADERGLSL_H
#define _SHADERGLSL_H

#include "glee/GLee.h"

#include <map>
#include <string>

#include <assert.h>

class ShaderGLSL
{
public:
	ShaderGLSL();
	virtual ~ShaderGLSL();

	void Bind();
	void Unbind();
	
	bool Create( const char* in_szFilename, unsigned int in_eInPrimType = GL_TRIANGLES, unsigned int in_eOutPrimType = GL_TRIANGLE_STRIP, unsigned int in_nMaxOutVertices = 8 ) ;
	void Destroy();

protected:
	bool CreateShader( unsigned int & in_iShader, unsigned int in_eShaderType, const char* in_szFilename );

	bool ValidateShader( unsigned int in_iShader, const char* in_szFilename);
	bool ValidateProgram(unsigned int in_iProgram, const char* in_szFilename );
	bool LinkStatus(unsigned int in_iProgram, const char* in_szFilename );

private:
	unsigned int	m_iVertShader;
	unsigned int	m_iFragShader;
	unsigned int	m_iGeomShader;
	unsigned int	m_iProgram;

	unsigned int m_iUniformSampler[4];

	unsigned int	m_iUniformTotalTime;
	unsigned int	m_iUniformDeltaTime;

public:
	class Uniform
	{
	public:
		typedef void (*Function) (GLint location, GLsizei count, const GLfloat * value );

		Uniform( unsigned int in_nLocation, unsigned int in_nCount, unsigned int in_nValueCount, Function in_Function ) 
			: m_nLocation(in_nLocation), m_nCount(in_nCount), m_nValueCount(in_nValueCount), m_Function(in_Function)
		{
			m_pValues = new float[m_nValueCount];
		}

		virtual ~Uniform() 
		{ 
			delete[] m_pValues; 
		}

		void Set( float in_fValue ) 
		{ 
			assert( 1 == m_nValueCount );
			*m_pValues = in_fValue;
			Apply();
		}
		
		void Set( unsigned int in_nValueCount, float* in_pValue ) 
		{ 
			assert( in_nValueCount == m_nValueCount );
			memcpy( m_pValues, in_pValue, sizeof(float)*m_nValueCount );
			Apply();
		}

		void Apply()
		{
			(*m_Function)(m_nLocation, m_nCount, m_pValues );
		}
		
	private:
		GLuint	m_nLocation;
		float*	m_pValues;
		const unsigned int	m_nValueCount;
		const unsigned int	m_nCount;
		Function	m_Function;
	};

	std::shared_ptr<Uniform>	GetUniform( const char* in_szName )
	{
		std::map<std::string,std::shared_ptr<Uniform>>::iterator it = m_mapUniform.find(in_szName);
		
		if( it != m_mapUniform.end() )
		{
			return it->second;
		}
		return NULL;
	}

	// typedef void (APIENTRYP GLEEPFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint * value);

private:
	std::map<std::string,std::shared_ptr<Uniform>>	m_mapUniform;
};

#endif//_SHADERGLSL_H

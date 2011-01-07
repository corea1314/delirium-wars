#ifndef _SHADERGLSL_H
#define _SHADERGLSL_H

#include "glee/GLee.h"

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

private:
	unsigned int	m_iVertShader;
	unsigned int	m_iFragShader;
	unsigned int	m_iGeomShader;
	unsigned int	m_iProgram;

	unsigned int m_iUniformSampler[4];

	unsigned int	m_iUniformTotalTime;
	unsigned int	m_iUniformDeltaTime;
};

#endif//_SHADERGLSL_H

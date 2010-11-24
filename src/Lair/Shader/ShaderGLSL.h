#ifndef _SHADERGLSL_H
#define _SHADERGLSL_H


class TiXmlElement;

class ShaderGLSL
{
public:
	ShaderGLSL();
	virtual ~ShaderGLSL();

	void Bind();
	void Unbind();
	
	bool Create( const char* in_szFilename );
	void Destroy();

protected:
	bool LoadFromXML( TiXmlElement* in_pxmlProgram );

	bool ValidateShader( unsigned int, const char* );
	bool ValidateProgram( unsigned int );

private:
	unsigned int	m_iVertShader;
	unsigned int	m_iFragShader;
	unsigned int	m_iProgram;

	unsigned int m_iUniformSampler[4];

	unsigned int	m_iUniformTotalTime;
	unsigned int	m_iUniformDeltaTime;

	static const char*	ms_szLoadFilename;
};

#endif//_SHADERGLSL_H

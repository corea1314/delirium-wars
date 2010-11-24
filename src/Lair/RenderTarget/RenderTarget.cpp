
#include "RenderTarget.h"

#include "../../Lair/Lair.h"

#include "glee/GLee.h"


RenderTarget::RenderTarget()
{
	m_hFBO=0;
	m_hRB_Depth=0;
	m_hRB_Stencil=0;
	memset( m_hRB_Color, 0, sizeof(unsigned int)*MAX_COLOR_ATTACHMENTS);
	memset( m_pTexture, 0, sizeof(Texture*)*MAX_COLOR_ATTACHMENTS);
	m_nWidth=0;
	m_nHeight=0;
	m_nMaxAttachments = 0;
	m_nMaxDrawBuffers = 0;
}

RenderTarget::~RenderTarget()
{
	Destroy(); // safety
}

bool RenderTarget::Create( unsigned int in_nWidth, unsigned int in_nHeight, bool in_bDepthSupport, bool in_bStencilSupport )
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_nMaxAttachments);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_nMaxDrawBuffers);

	m_nWidth = in_nWidth;
	m_nHeight = in_nHeight;
	
	glGenFramebuffersEXT(1, &m_hFBO);
	
	Bind();
	
	if( in_bDepthSupport )
	{
		glGenRenderbuffersEXT(1, &m_hRB_Depth);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_hRB_Depth);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32, m_nWidth, m_nHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_hRB_Depth);
	}
	if( in_bStencilSupport )
	{
		glGenRenderbuffersEXT(1, &m_hRB_Stencil);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_hRB_Stencil);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_nWidth, m_nHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_hRB_Depth);
	}
	
	Unbind();
	
	return true;
}

void RenderTarget::Destroy()
{
	Bind();
	if( m_hRB_Depth )
	{
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
		glDeleteRenderbuffersEXT(1, &m_hRB_Depth);
		m_hRB_Depth=0;
	}
	if( m_hRB_Stencil )
	{
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
		glDeleteRenderbuffersEXT(1, &m_hRB_Stencil);
		m_hRB_Stencil=0;
	}
	Unbind();
	
	if( m_hFBO )
	{
		glDeleteFramebuffersEXT(1,&m_hFBO);
	}

	int i;
	for(i=0;i<MAX_COLOR_ATTACHMENTS;i++)
	{
		delete m_pTexture[i];
	}
}

void RenderTarget::Bind()
{
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport(0,0,m_nWidth,m_nHeight);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_hFBO);
}

void RenderTarget::Unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}

Texture* RenderTarget::SetTextureTarget( unsigned int in_nIndex, unsigned int in_nTarget, unsigned int in_nFormat )
{
	if( m_pTexture[in_nIndex] == NULL )
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_hFBO);

		Texture* pTexture = m_pTexture[in_nIndex] = new Texture; pTexture->LoadFromParam( m_nWidth, m_nHeight, 4 );
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+in_nIndex, in_nTarget, pTexture->GetId(), 0 );

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
    
	return m_pTexture[in_nIndex];
}

Texture* RenderTarget::GetTextureTarget( unsigned int in_nIndex )
{
	return m_pTexture[in_nIndex];
}

void RenderTarget::SetActiveTextureTarget( unsigned int in_nIndex )
{
	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT + in_nIndex );
}

void RenderTarget::SetActiveTextureTargets( unsigned int in_nCount, unsigned int in_nIndex[] )
{
	static unsigned int nColorAttachment[MAX_COLOR_ATTACHMENTS] = {0};

	for(unsigned int i=0;i<in_nCount;i++)
		nColorAttachment[i] = GL_COLOR_ATTACHMENT0_EXT + in_nIndex[i];

	glDrawBuffers( in_nCount, nColorAttachment );
}

int RenderTarget::GetMaxTextureTargets()
{
	return m_nMaxAttachments;
}

int RenderTarget::GetMaxDrawBuffers()
{
	return m_nMaxDrawBuffers;
}

bool RenderTarget::IsValid( )
{
	Bind();
	
	bool isOK = false;
	
	GLenum status;                                            
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status) {                                          
	case GL_FRAMEBUFFER_COMPLETE_EXT: // Everything's OK
		isOK = true;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT" );
		isOK = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT" );
		isOK = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" );
		isOK = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" );
		isOK = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT" );
		isOK = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT" );
		isOK = false;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: GL_FRAMEBUFFER_UNSUPPORTED_EXT" );
		isOK = false;
		break;
	default:
		Lair::GetLogMan()->Log( "RenderTarget", "FBO: CheckFramebufferStatus() ERROR: Unknown ERROR" );
		isOK = false;
	}
	
	Unbind();
	return isOK;
}

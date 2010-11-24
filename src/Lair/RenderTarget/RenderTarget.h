#ifndef _RENDERTARGET_H
#define _RENDERTARGET_H

/*
Framebuffer objects
	Renderbuffer objects
		Color Buffer
		Depth Buffer
		Stencil Buffer
	Color Attachments (aka textures)
*/

#define MAX_COLOR_ATTACHMENTS	16

class Texture;

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Bind();
	void Unbind();

	bool Create( unsigned int in_nWidth, unsigned int in_nHeight, bool in_bDepth, bool in_bStencil );
	void Destroy();

	Texture*	SetTextureTarget( unsigned int in_nIndex, unsigned int in_nTarget, unsigned int in_nFormat );
	Texture*	GetTextureTarget( unsigned int in_nIndex );
	void		SetActiveTextureTarget( unsigned int in_nIndex );
	void		SetActiveTextureTargets( unsigned int in_nCount, unsigned int in_nIndex[] );
	int			GetMaxTextureTargets();
	int			GetMaxDrawBuffers();

	bool IsValid();

private:
	unsigned int m_hFBO;			// id of the framebuffer object
	unsigned int m_hRB_Depth;		// id of the renderbuffer object for depth
	unsigned int m_hRB_Stencil;		// id of the renderbuffer object for stencil
	unsigned int m_hRB_Color[MAX_COLOR_ATTACHMENTS];	// id of the renderbuffer object for color

	Texture*	m_pTexture[MAX_COLOR_ATTACHMENTS];

	unsigned int m_nWidth;
	unsigned int m_nHeight;

	int m_nMaxAttachments;
	int m_nMaxDrawBuffers;
};

#endif//_RENDERTARGET_H
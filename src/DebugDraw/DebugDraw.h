#ifndef _DEBUG_DRAW_H
#define _DEBUG_DRAW_H

#include "Math/Vector2.h"

class Texture;

class CDebugFont
{
public:
	CDebugFont( const char* in_szFontTextureName ) // Default Font Texture is used if NULL is passed
	{
		m_nFontListId = 0;
		m_pFontTexture = 0;
		m_nGlyphWidth = 0;
		m_nGlyphHeight = 0;

		BuildFont(in_szFontTextureName);
	}

	virtual ~CDebugFont();
	//	void DrawText( Vector2 in_vPos, const char* in_szText );
	//	void DrawText( Vector2 in_vPos, Vector2 in_vScale, float in_cTextColor[4], const char* in_szText );
	void Text( Vector2 in_vPos, const char* in_szText )
	{
		static float s_fColor[4] = {1.0f,1.0f,1.0f,1.0f};
		Text( in_vPos, Vector2(1.0f,1.0f), s_fColor, in_szText );
	}

	void Text( Vector2 in_vPos, Vector2 in_vScale, float in_cTextColor[4], const char* in_szText );

	inline unsigned int GetGlyphWidth()		{ return m_nGlyphWidth;		}
	inline unsigned int GetGlyphHeight()	{ return m_nGlyphHeight;	}

private:
	void BuildFont( const char* in_szFontTextureName );

	unsigned int	m_nFontListId;		// fixme GL Display list
	Texture*		m_pFontTexture;

	unsigned int	m_nGlyphWidth;
	unsigned int	m_nGlyphHeight;
};


class CDebugDraw
{
public:
	class Color 
	{
	public:
		
		enum E
		{
			eBLACK	= 0xFF000000, eWHITE	= 0xFFFFFFFF, eGREY	= 0xFF7F7F7F, eRED	= 0xFF0000FF, eGREEN	= 0xFF00FF00,
			eBLUE	= 0xFFFF0000, eORANGE = 0xFF007FFF,	eYELLOW = 0xFF00FFFF, ePURPLE = 0xFFFF00FF,	eLIME	= 0xFF00FF7F,
			eCYAN	= 0xFFFFFF00
		};

		unsigned long c;
        Color():c(eWHITE) {}
        Color( unsigned long _c ) : c(_c) {}
        Color( unsigned int _c ) : c(_c) {}

		Color & operator = (const unsigned long & other ) { c = other; return *this; } 
		Color & operator = (const Color::E & other ) { c = other; return *this; } 

		
		static Color Random();	
	};

	class Vertex
	{
	public:
		Color color;
		float x,y,z;

		inline void Set( float _x, float _y, Color _color )
		{
			x = _x;
			y = _y;
			z = 0.0f;
			color = _color;
		}

	};

	CDebugDraw( unsigned int	in_nCountLineVB, unsigned int in_nCountFillVB, float in_fLineWidth );
	virtual ~CDebugDraw();

	void SetColor(const Color& color) { m_cCurrentColor = color; }
	void SetColor(const Color::E & color ) { m_cCurrentColor = color; }
	void SetDepth( float in_fDepth ) { m_fCurrDepth = in_fDepth; }

	// Lines
	void DrawCircle(const Vector2& pos, float radius);
	void DrawEllipse(const Vector2& pos, float w, float h);
	void DrawEllipse(const Vector2& pos, float w, float h, float a );
	void DrawTriangle( const Vector2& pos, float w, float h, float a );

	void DrawLine(const Vector2& posa, const Vector2& posb);
	void DrawLines( Vector2* pos, unsigned int count );

	void DrawRectangle(const Vector2& pos, float width, float height, float angle );

	void DrawPoly(const Vector2& pos, Vector2 poly[], unsigned int count );
	void DrawPoly(const Vector2& pos, Vector2 poly[], unsigned int count, float angle );
	
	// Fill
	/*
	void FillCircle(const Vector2& pos, float radius);
	void FillEllipse(const Vector2& pos, float w, float h);
	
	void FillRectangle(const Vector2& pos, float width, float height);
	
	void FillTriangle(const Vector2& pos, float width, float height);
	void FillTriangle(const Vector2& pos, float width, float height, float angle);
		
	void FillPoly(const Vector2& pos, Vector2 poly[], unsigned int count );
	void FillPoly(const Vector2& pos, Vector2 poly[], unsigned int count, float angle );
	*/

	void Text( int x, int y, char* text, ... );

	// Rendering
	void Flush();
	void BeginFrame();
	void EndFrame();

	unsigned int GetCurrVertexCount() const { return (m_pCurrVertex - m_pLineVB); }

private:
	void FlushCheck( unsigned int in_nCount );	// check the number to add is going to bust the vb
	void AddContourLinesFromPoints( Vector2* pts, unsigned int count );
	
private:
	Vertex*	m_pLineVB;	// vertex buffer used for line drawing
//	Vertex*	m_pFillVB;	// vertex buffer used for triangle filling

	Color	m_cCurrentColor;
	Vertex*	m_pCurrVertex;
	float	m_fCurrDepth;

	const unsigned int	m_nCountLineVB;
//	const unsigned int	m_nCountFillVB;

	float	m_fLineWidth;

	const unsigned int m_nWorkVectorCount;
	Vector2*	m_pWorkVector;

	CDebugFont*	m_pFont;
};


#endif//_DEBUG_DRAW_H

#ifndef _DEBUG_DRAW_H
#define _DEBUG_DRAW_H

#include "../Math/Vector2.h"

class CDebugDraw
{
public:
	class Color 
	{
	public:
		unsigned long c;
		Color & operator = (const unsigned long & other ) { c = other; return *this; } 
		
		enum E
		{
			eBLACK	= 0xFF000000, eWHITE	= 0xFFFFFFFF, eGREY	= 0xFF7F7F7F, eRED	= 0xFF0000FF, eGREEN	= 0xFF00FF00,
			eBLUE	= 0xFFFF0000, eORANGE = 0xFF007FFF,	eYELLOW = 0xFF00FFFF, ePURPLE = 0xFFFF00FF,	eLIME	= 0xFF00FF7F,
			eCYAN	= 0xFFFFFF00
		};

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

	void SetColor(const Color& color);

	// Lines
	void DrawCircle(const Vector2& pos, float radius);
	void DrawEllipse(const Vector2& pos, float w, float h);

	void DrawLine(const Vector2& posa, const Vector2& posb);
	void DrawLines( const Vector2* pos, unsigned int count );

	void DrawRectangle(const Vector2& pos, float width, float height);

	void DrawPoly(const Vector2& pos, Vector2 poly[], float count );
	void DrawPoly(const Vector2& pos, Vector2 poly[], float count, float angle );
	
	// Fill
	void FillCircle(const Vector2& pos, float radius);
	void FillEllipse(const Vector2& pos, float w, float h);
	
	void FillRectangle(const Vector2& pos, float width, float height);
	
	void FillTriangle(const Vector2& pos, float width, float height);
	void FillTriangle(const Vector2& pos, float width, float height, float angle);
		
	void FillPoly(const Vector2& pos, Vector2 poly[], float count );
	void FillPoly(const Vector2& pos, Vector2 poly[], float count, float angle );

	// Rendering
	void Flush();
	void BeginFrame();
	void EndFrame();

	unsigned int GetCurrVertexCount() const { return (m_pCurrVertex - m_pLineVB); }

private:
	void FlushCheck( unsigned int in_nCount );	// check the number to add is going to bust the vb
	
private:
	Vertex*	m_pLineVB;	// vertex buffer used for line drawing
//	Vertex*	m_pFillVB;	// vertex buffer used for triangle filling

	Color	m_cCurrentColor;
	Vertex*	m_pCurrVertex;


	const unsigned int	m_nCountLineVB;
//	const unsigned int	m_nCountFillVB;

	float			m_fLineWidth;
};



#endif//_DEBUG_DRAW_H

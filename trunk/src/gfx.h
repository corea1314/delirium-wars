#ifndef _GFX_H
#define _GFX_H

#include "fw.h"

typedef unsigned long Color;

typedef struct
{
	enum E
	{
		eBLACK	= 0xFF000000,
		eWHITE	= 0xFFFFFFFF,
		eGREY	= 0xFF7F7F7F,
		eRED	= 0xFF0000FF,
		eGREEN	= 0xFF00FF00,
		eBLUE	= 0xFFFF0000,
		eORANGE = 0xFF007FFF,
		eYELLOW = 0xFF00FFFF, 
		ePURPLE = 0xFFFF00FF,
		eLIME	= 0xFF00FF7F,
		eCYAN	= 0xFFFFFF00
	};

	static Color Random();
	
} COLORS;

typedef struct 
{
	Color color;
	float x,y,z;

	inline void Set( float _x, float _y, Color _color )
	{
		x = _x;
		y = _y;
		z = 0.0f;
		color = _color;
	}

} Vertex;


#define VB_FORMAT	GL_C4UB_V3F

void gl_SetColor( Color c );
void gl_RenderText( int x, int y, char* text, ... );
void gl_RenderVB( GLenum mode, Vertex* buffer, int start, int count );
void gl_RenderPoints( Vertex* buffer, int start, int count, float psize );
void gl_RenderLines( Vertex* buffer, int start, int count, float lwidth );
void gl_Init( void );


class CDebugGraphic
{
public:
	void SetColor(const Color& color);
	void SetLineWidth( float width );

	void DrawCircle(float x, float y, float radius);
	void FillCircle(float x, float y, float radius);

	void DrawEllipse(float x, float y, float w, float h);
	void FillEllipse(float x, float y, float w, float h);

	void DrawLine(float x1, float y1, float x2, float y2);

	void FillRectangle(float x, float y, float width, float height);
	void FillTriangle(float x, float y, float width, float height);
	void FillTriangle(float x, float y, float width, float height, float angle);

	void DrawPoly(float x, float y, Vertex poly[], float count );
	void FillPoly(float x, float y, Vertex poly[], float count );
	void DrawPoly(float x, float y, Vertex poly[], float count, float angle );
	void FillPoly(float x, float y, Vertex poly[], float count, float angle );


private:
	Vertex*	m_pLineVB;	// vertex buffer used for line drawing
	Vertex*	m_pFillVB;	// vertex buffer used for triangle filling

	unsigned int	m_nCountLineVB;
	unsigned int	m_nCountFillVB;
};


/*
RED			rouge 255.0.0

			magenta 255.0.128

PURPLE		violet 255.0.255

			bleu violac� 128.0.255

BLUE		bleu 0.0.255

BLUE_LIGHT	bleu ciel 0.128.255

			cyan 0.255.255

			bleu vert 0.255.128

GREEN		vert 0.255.0

LIME		vert lime 128.255.0

YELLOW		jaune 255.255.0

ORANGE		orange 255.128.0

*/

#endif//_GFX_H


#ifndef _GFX_H
#define _GFX_H

#ifdef WIN32
#include <windows.h>
#endif
#include "glee/GLee.h"	//todo: boot glee properly
#include "freeglut-2.4.0/include/gl/freeglut.h"

typedef unsigned long Color;

typedef struct
{
	enum E
	{
		eBLACK		= 0xFF000000,
		eWHITE		= 0xFFFFFFFF,
		eGREY		= 0xFF7F7F7F,
		eLIGHTGREY	= 0xFF808080,
		eDARKGREY	= 0xFF444444,
		eRED		= 0xFF0000FF,
		eGREEN		= 0xFF00FF00,
		eBLUE		= 0xFFFF0000,
		eORANGE		= 0xFF007FFF,
		eYELLOW		= 0xFF00FFFF, 
		ePURPLE		= 0xFFFF00FF,
		eLIME		= 0xFF00FF7F,
		eCYAN		= 0xFFFFFF00
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
void gl_SetColor( Color c, float a );
void gl_RenderText( int x, int y, char* text, ... );
void gl_RenderVB( GLenum mode, Vertex* buffer, int start, int count );
void gl_RenderPoints( Vertex* buffer, int start, int count, float psize );
void gl_RenderLines( Vertex* buffer, int start, int count, float lwidth );
void gl_Init( void );

/*
RED			rouge 255.0.0

			magenta 255.0.128

PURPLE		violet 255.0.255

			bleu violacé 128.0.255

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


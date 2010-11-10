
#include "gfx.h"

Color COLORS::Random()
{
	Color a = rand();
	Color b = a << 8;
	Color c = 0xFF000000 | (rand() << 16) | rand();  
	return c;
}

void gl_SetColor( Color c )
{
	glColor4ubv( (unsigned char*)&c );
}

void gl_RenderVB( GLenum mode, Vertex* buffer, int start, int count )
{
	glInterleavedArrays( VB_FORMAT, sizeof(Vertex), buffer );
	glDrawArrays( mode, start, count );
}

void gl_RenderPoints( Vertex* buffer, int start, int count, float psize )
{
	glPointSize(psize);
	gl_RenderVB( GL_POINTS, buffer, start, count );
	glPointSize(1.0f);
}

void gl_RenderLines( Vertex* buffer, int start, int count, float lwidth )
{
	glLineWidth(lwidth);
	gl_RenderVB( GL_LINES, buffer, start, count );
	glLineWidth(1.0f);
}

void gl_RenderText( int x, int y, char* text, ... )
{
	static char buffer[1024];
	static float pos[2];

	pos[0] = (float)x;
	pos[1] = (float)y;

	glRasterPos2fv( pos );

	va_list marker;
	va_start( marker, text );
	vsprintf( buffer, text, marker );
	va_end(marker);

	int len, i;
	for (i = 0, len = strlen(buffer); i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)buffer[i]);
	}
}
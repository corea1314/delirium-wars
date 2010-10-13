
#include "gfx.h"

Color COLORS::Random()
{
	Color a = rand();
	Color b = a << 8;
	Color c = 0xFF000000 | (rand() << 16) | rand();  
	return c;
}


Vertex* ExtrudeSet( Vertex* buffer, int count, float height )
{
	Vertex* extrusion = new Vertex[2+count*2];

	int i,k;
	for(i=0,k=0;i<count;i++,k+=2)
	{
		extrusion[k] = extrusion[k+1] = buffer[k];
		extrusion[k+1].z = extrusion[k+1].z + height;
	}
	extrusion[k] = extrusion[k+1] = buffer[0];
	extrusion[k+1].z = extrusion[k+1].z + height;
	
	return extrusion;
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

void gl_Init( void )
{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glShadeModel(GL_SMOOTH);

	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glEnable (GL_POINT_SMOOTH);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void gl_RenderText( int x, int y, char* text, ... )
{
	static char buffer[1024];
	static float pos[2];

	pos[0] = x;
	pos[1] = y;
	
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
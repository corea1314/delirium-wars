
#include "DebugDraw.h"

#include <glee/GLee.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Lair/Lair.h"

#define MAX_WORK_VECTOR_COUNT 32

static Vector2 g_vTrigo[32] = 
{
	Vector2(  1.0000f,	 0.0000f ),
	Vector2(  0.9808f,	 0.1951f ),
	Vector2(  0.9239f,	 0.3827f ),
	Vector2(  0.8315f,	 0.5556f ),
	Vector2(  0.7071f,	 0.7071f ),
	Vector2(  0.5556f,	 0.8315f ),
	Vector2(  0.3827f,	 0.9239f ),
	Vector2(  0.1951f,	 0.9808f ),
	Vector2(  0.0000f,	 1.0000f ),
	Vector2( -0.1951f,	 0.9808f ),
	Vector2( -0.3827f,	 0.9239f ),
	Vector2( -0.5556f,	 0.8315f ),
	Vector2( -0.7071f,	 0.7071f ),
	Vector2( -0.8315f,	 0.5556f ),
	Vector2( -0.9239f,	 0.3827f ),
	Vector2( -0.9808f,	 0.1951f ),
	Vector2( -1.0000f,	 0.0000f ),
	Vector2( -0.9808f,	-0.1951f ),
	Vector2( -0.9239f,	-0.3827f ),
	Vector2( -0.8315f,	-0.5556f ),
	Vector2( -0.7071f,	-0.7071f ),
	Vector2( -0.5556f,	-0.8315f ),
	Vector2( -0.3827f,	-0.9239f ),
	Vector2( -0.1951f,	-0.9808f ),
	Vector2(  0.0000f,	-1.0000f ),
	Vector2(  0.1951f,	-0.9808f ),
	Vector2(  0.3827f,	-0.9239f ),
	Vector2(  0.5556f,	-0.8315f ),
	Vector2(  0.7071f,	-0.7071f ),
	Vector2(  0.8315f,	-0.5556f ),
	Vector2(  0.9239f,	-0.3827f ),
	Vector2(  0.9808f,	-0.1951f ),
};

CDebugDraw::CDebugDraw( unsigned int in_nCountLineVB, unsigned int in_nCountFillVB, float in_fLineWidth ) 
	: m_nCountLineVB(in_nCountLineVB), m_fLineWidth(in_fLineWidth), m_nWorkVectorCount(32)
{
	m_pWorkVector = new Vector2[m_nWorkVectorCount];

	m_pCurrVertex = m_pLineVB = new Vertex[m_nCountLineVB];
	m_cCurrentColor = Color::eWHITE;
	m_fCurrDepth = 0.0f;

	m_pFont = new CDebugFont( "Font.png" );
}

CDebugDraw::~CDebugDraw()
{
	delete m_pFont;
	delete[] m_pLineVB;
	delete[] m_pWorkVector;
}

void CDebugDraw::FlushCheck( unsigned int in_nCount )
{
	// if we bust the buffer, flush first
	if( (GetCurrVertexCount() + in_nCount) > m_nCountLineVB )
		Flush();
}

void CDebugDraw::Flush()
{
	#define VB_FORMAT	GL_C4UB_V3F

	glInterleavedArrays( VB_FORMAT, sizeof(Vertex), m_pLineVB );
	glDrawArrays( GL_LINES, 0, GetCurrVertexCount() );
	
	m_pCurrVertex = m_pLineVB;	// reset buffer
}

void CDebugDraw::BeginFrame()
{
	m_pCurrVertex = m_pLineVB;

	glLineWidth( m_fLineWidth );
}

void CDebugDraw::EndFrame()
{
	if( (m_pCurrVertex - m_pLineVB) > 0 )
		Flush();

	glLineWidth( 1.0f );
}

void CDebugDraw::DrawCircle(const Vector2& pos, float radius)
{
	for( int i=0; i < MAX_WORK_VECTOR_COUNT/2; i++ )
	{
		m_pWorkVector[i].x = g_vTrigo[i*2].x * radius + pos.x;
		m_pWorkVector[i].y = g_vTrigo[i*2].y * radius + pos.y;
	}

	AddContourLinesFromPoints( m_pWorkVector, MAX_WORK_VECTOR_COUNT/2 );
}

void CDebugDraw::DrawEllipse(const Vector2& pos, float w, float h)
{
	for( int i=0; i < MAX_WORK_VECTOR_COUNT/2; i++ )
	{
		m_pWorkVector[i].x = g_vTrigo[i*2].x * w + pos.x;
		m_pWorkVector[i].y = g_vTrigo[i*2].y * h + pos.y;
	}
	AddContourLinesFromPoints( m_pWorkVector, MAX_WORK_VECTOR_COUNT/2 );
}

void CDebugDraw::DrawEllipse(const Vector2& pos, float w, float h, float a )
{
	for( int i=0; i < MAX_WORK_VECTOR_COUNT/2; i++ )
	{
		m_pWorkVector[i] = Vector2( g_vTrigo[i*2].x * w, g_vTrigo[i*2].y * h ).GetRotate(a) + pos;
	}
	AddContourLinesFromPoints( m_pWorkVector, MAX_WORK_VECTOR_COUNT/2 );
}

void CDebugDraw::DrawTriangle( const Vector2& pos, float w, float h, float a )
{
	m_pWorkVector[0] = Vector2(  h/2,    0 ).GetRotate(a) + pos;
	m_pWorkVector[1] = Vector2( -h/2, -w/2 ).GetRotate(a) + pos;
	m_pWorkVector[2] = Vector2( -h/2,  w/2 ).GetRotate(a) + pos;
	AddContourLinesFromPoints( m_pWorkVector, 3 );
}

void CDebugDraw::DrawLine(const Vector2& posa, const Vector2& posb)
{
	FlushCheck(2);
	m_pCurrVertex->color = m_cCurrentColor;
	m_pCurrVertex->x = posa.x;
	m_pCurrVertex->y = posa.y;
	m_pCurrVertex->z = m_fCurrDepth;
	++m_pCurrVertex;
	m_pCurrVertex->color = m_cCurrentColor;
	m_pCurrVertex->x = posb.x;
	m_pCurrVertex->y = posb.y;
	m_pCurrVertex->z = m_fCurrDepth;
	++m_pCurrVertex;
}

void CDebugDraw::AddContourLinesFromPoints( Vector2* pts, unsigned int count )
{
	FlushCheck(count*2);

	Vector2* p = pts;
		
	for( unsigned int i=0;i<count-1;i++)
	{
		m_pCurrVertex->color = m_cCurrentColor;
		m_pCurrVertex->x = p->x;
		m_pCurrVertex->y = p->y;
		m_pCurrVertex->z = m_fCurrDepth;
		++m_pCurrVertex;
		++p;
		
		m_pCurrVertex->color = m_cCurrentColor;
		m_pCurrVertex->x = p->x;
		m_pCurrVertex->y = p->y;
		m_pCurrVertex->z = m_fCurrDepth;
		++m_pCurrVertex;
	}

	m_pCurrVertex->color = m_cCurrentColor;
	m_pCurrVertex->x = p->x;
	m_pCurrVertex->y = p->y;
	m_pCurrVertex->z = m_fCurrDepth;
	++m_pCurrVertex;

	m_pCurrVertex->color = m_cCurrentColor;
	m_pCurrVertex->x = pts->x;
	m_pCurrVertex->y = pts->y;
	m_pCurrVertex->z = m_fCurrDepth;
	++m_pCurrVertex;
}

void CDebugDraw::DrawRectangle(const Vector2& pos, float width, float height, float angle )
{
	m_pWorkVector[0] = Vector2(-width/2,-height/2).GetRotate( angle ) + pos; 
	m_pWorkVector[1] = Vector2( width/2,-height/2).GetRotate( angle ) + pos; 
	m_pWorkVector[2] = Vector2( width/2, height/2).GetRotate( angle ) + pos; 
	m_pWorkVector[3] = Vector2(-width/2, height/2).GetRotate( angle ) + pos;

	AddContourLinesFromPoints( m_pWorkVector, 4 );
}

void CDebugDraw::DrawPoly(const Vector2& pos, Vector2 poly[], unsigned int count )
{
	unsigned int k = MIN( count, m_nWorkVectorCount );
	for( unsigned int i=0; i < k; i++ )
	{
		m_pWorkVector[i] = poly[i] + pos; 
	}
	AddContourLinesFromPoints( m_pWorkVector, k );
}

void CDebugDraw::DrawPoly(const Vector2& pos, Vector2 poly[], unsigned int count, float angle )
{
	unsigned int k = MIN( count, m_nWorkVectorCount );
	for( unsigned int i=0; i < k; i++ )
	{
		m_pWorkVector[i] = poly[i].GetRotate( angle ) + pos; 
	}	
	AddContourLinesFromPoints( m_pWorkVector, k );
}


void CDebugDraw::Text( int x, int y, char* text, ... )
{
	static char buffer[1024];
	
	va_list marker;
	va_start( marker, text );
	vsprintf( buffer, text, marker );
	va_end(marker);

	m_pFont->Text( Vector2(x,y), buffer );
}

///////////////////////////////////////////////////////////////////////////////

void CDebugFont::Text( Vector2 in_vPos, Vector2 in_vScale, float in_cTextColor[4], const char* in_szText )
{
	glPushAttrib( GL_ENABLE_BIT );

	glEnable( GL_TEXTURE_2D );
	m_pFontTexture->Bind();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glColor4fv( in_cTextColor );

	glPushMatrix();
	glTranslatef( in_vPos.x, in_vPos.y, 0.0f );
	glScalef( in_vScale.x, in_vScale.y, 1.0f );
	glListBase(m_nFontListId);
	glCallLists( strlen(in_szText), GL_UNSIGNED_BYTE, in_szText );
	glListBase(0);
	glPopMatrix();

	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glPopAttrib();
}

CDebugFont::~CDebugFont()
{
	if( m_pFontTexture )
	{
		SAFE_DELETE( m_pFontTexture );
	}

	if( m_nFontListId )
		glDeleteLists( m_nFontListId, 256 );
}

void CDebugFont::BuildFont( const char* in_szFontTextureName )
{
	if( m_nFontListId )
		glDeleteLists( m_nFontListId, 256 );

	if( in_szFontTextureName )
		m_pFontTexture = Lair::GetTextureMan()->Get( in_szFontTextureName );

	m_nFontListId = glGenLists(256);

#define FONT_CHAR_PER_ROW	16
#define FONT_CHAR_WIDTH 16
#define FONT_CHAR_HEIGHT 16

	m_nGlyphWidth = FONT_CHAR_WIDTH;		// fixme
	m_nGlyphHeight = FONT_CHAR_HEIGHT;		// fixme

	Vector2 uvMin, uvMax;

	int i=0;
	for( i=0; i<256; i++)
	{
		uvMax.x = (float)((uvMin.x = (float)((i % FONT_CHAR_PER_ROW) * FONT_CHAR_WIDTH)) + FONT_CHAR_WIDTH);
		uvMin.y = (float)((uvMax.y = (float)((16 - (i / FONT_CHAR_PER_ROW)) * FONT_CHAR_HEIGHT)) - FONT_CHAR_HEIGHT);

		uvMax.x /= 256.0f;
		uvMax.y /= 256.0f;
		uvMin.x /= 256.0f;
		uvMin.y /= 256.0f;

		//		uvMax.x = 1.0f;
		//		uvMax.y = 1.0f;
		//		uvMin.x = 0.0f;
		//		uvMin.y = 0.0f;

		glNewList( m_nFontListId + i, GL_COMPILE );

		glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( uvMax.x, uvMax.y ); glVertex3f( 8.0f, 16.0f, 0.0f  ); 
		glTexCoord2f( uvMax.x, uvMin.y ); glVertex3f( 8.0f,  0.0f, 0.0f  ); 
		glTexCoord2f( uvMin.x, uvMax.y ); glVertex3f(  0.0f, 16.0f, 0.0f  ); 
		glTexCoord2f( uvMin.x, uvMin.y ); glVertex3f(  0.0f,  0.0f, 0.0f  ); 
		glEnd();

		glTranslatef( FONT_CHAR_WIDTH/2, 0.0f, 0.0f );
		glEndList();
	}
}

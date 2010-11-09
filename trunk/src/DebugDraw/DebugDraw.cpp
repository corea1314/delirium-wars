
#include "DebugDraw.h"

CDebugDraw::CDebugDraw( unsigned int in_nCountLineVB, unsigned int in_nCountFillVB, float in_fLineWidth ) : m_nCountLineVB(in_nCountLineVB), m_fLineWidth(in_fLineWidth)
{
	m_pCurrVertex = m_pLineVB = new Vertex[m_nCountLineVB];
}

CDebugDraw::~CDebugDraw()
{
	delete[] m_pLineVB;
}


void CDebugDraw::SetColor(const Color& color)
{
	m_cCurrentColor = color;
}

void CDebugDraw::FlushCheck( unsigned int in_nCount )
{
	// if we bust the buffer, flush first
	if( (GetCurrVertexCount() + in_nCount) > m_nCountLineVB )
		Flush();

	m_pCurrVertex = m_pLineVB;	// reset buffer
}

void CDebugDraw::Flush()
{

}

void CDebugDraw::BeginFrame()
{

}

void CDebugDraw::EndFrame()
{
	if( (m_pCurrVertex - m_pLineVB) > 0 )
	Flush();
}

void CDebugDraw::DrawCircle(const Vector2& pos, float radius)
{

}

void CDebugDraw::DrawEllipse(const Vector2& pos, float w, float h)
{

}

void CDebugDraw::DrawLine(const Vector2& posa, const Vector2& posb)
{

}

void CDebugDraw::DrawLines( const Vector2* pos, unsigned int count )
{

}

void CDebugDraw::DrawRectangle(const Vector2& pos, float width, float height)
{

}

void CDebugDraw::DrawPoly(const Vector2& pos, Vector2 poly[], float count )
{

}

void CDebugDraw::DrawPoly(const Vector2& pos, Vector2 poly[], float count, float angle )
{

}


#include "CurveEdit.h"

#include "gfx.h"

#include "../Grid.h"

static const char* OPENFILE_BCF_EXTENSION = "bcf";
static const char* OPENFILE_BCF_FILTER = "Bezier Curve File\0*.bcf\0";

static const char* OPENFILE_TGA_EXTENSION = "tga";
static const char* OPENFILE_TGA_FILTER = "Targa Image\0*.tga\0";

void CurveEditor::Init()
{
	Editor::Init();

	selection = 0;
}

void CurveEditor::Exit()
{
	Editor::Exit();
}

#define ANIMATE_FACTOR	20.0f

void CurveEditor::Render()
{
	// Projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -0.5f / cam_zoom, 0.5f / cam_zoom, -0.5f / cam_zoom, 0.5f / cam_zoom );
	
	// Model view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef( -cam_pos_x, -cam_pos_y, 0.0f );

	{
		// Render background texture if any
		/*if( appTexture.pTexture )
		{
			
			glColor4f(1.0f,1.0f,1.0f,0.7f);
			glEnable( GL_TEXTURE_2D );
			appTexture.pTexture->Bind();
			glBegin( GL_QUADS );
				glTexCoord2f(0,0);	glVertex2f(0,0);
				glTexCoord2f(appTexture.u,0);	glVertex2f(1,0);
				glTexCoord2f(appTexture.u,appTexture.v);	glVertex2f(1,1);
				glTexCoord2f(0,appTexture.v);	glVertex2f(0,1);
			glEnd();
			glDisable( GL_TEXTURE_2D );
			glColor4f(1,1,1,1);
			
		}*/

		// Render grid
		GetGrid()->Render();
		
		glPointSize( 8.0f );
		glLineWidth( 2.0f );

		// Render Curve
		RenderCurve();
		
		// Render selection
		glPointSize( 12.0f );

		if( selection )
		{
			gl_SetColor( COLORS::eRED );
			glBegin( GL_POINTS );
				glVertex2f( c.GetKey(selection->GetSelectedPointIndex()).mPosition, c.GetKey(selection->GetSelectedPointIndex()).mValue );
			glEnd();
		}
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW);
	glPopMatrix();

	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %d -- Grid: %d -- Snap: %s", zoom, GetGrid()->GetGridSize(), GetGrid()->GetSnap() ? "true" : "false" );
	gl_RenderText( 8, h-16-8, "Curved v%d.%d.%d (%s at %s)", CRV_VERSION_MAIN, CRV_VERSION_MAJOR, CRV_VERSION_MINOR, __DATE__, __TIME__ );

	float SPAN = c.GetKey(c.GetKeyCount()-1).mPosition - c.GetKey(0).mPosition;
	float START_POS = c.GetKey(0).mPosition - SPAN;
	float MID_POS = c.GetKey(0).mPosition;
	float END_POS = c.GetKey(c.GetKeyCount()-1).mPosition + SPAN;

	if( animate )
	{
		float timePos = GetTime() / ANIMATE_FACTOR; // Slow it by a factor
		float p = START_POS + (START_POS-END_POS) * (timePos - (int)(timePos));
		
		float v = c.Evaluate( p );
		Vector2 rmp(p,v);
		int x,y;
		EditorToScreen( rmp, x, y );
			
		gl_RenderText( x + 12, y - 12, "t: %0.2f", p );
		gl_RenderText( x + 12, y - 24, "v: %0.2f, %0.2f", rmp.x, rmp.y );
	}

	if( selection )
	{
		int x,y;
		Vector2 v( c.GetKey(selection->GetSelectedPointIndex()).mPosition, c.GetKey(selection->GetSelectedPointIndex()).mValue );
		EditorToScreen( v, x, y );
		gl_RenderText( x + 12, y + 12, "%f, %f", v.x, v.y );
	}
}

void CurveEditor::RenderCurve()
{
	int i = 0;

	float SPAN = c.GetKey(c.GetKeyCount()-1).mPosition - c.GetKey(0).mPosition;
	float START_POS = c.GetKey(0).mPosition - SPAN;
	float MID_START_POS = c.GetKey(0).mPosition;
	float MID_END_POS = c.GetKey(c.GetKeyCount()-1).mPosition;
	float END_POS = c.GetKey(c.GetKeyCount()-1).mPosition + SPAN;

	// Curve line
	glBegin( GL_LINE_STRIP );
	gl_SetColor( COLORS::eDARKGREY );
	for(float t=START_POS;t<MID_START_POS; t+= 0.01f )		
		glVertex2f( t, c.Evaluate(t) );
	gl_SetColor( COLORS::eWHITE );
	for(float t=MID_START_POS;t<MID_END_POS; t+= 0.01f )		
		glVertex2f( t, c.Evaluate(t) );
	gl_SetColor( COLORS::eDARKGREY );
	for(float t=MID_END_POS;t<END_POS; t+= 0.01f )		
		glVertex2f( t, c.Evaluate(t) );
	glEnd();
	glVertex2f( END_POS, c.Evaluate(END_POS) );

	// Tangents
	// TODO

	gl_SetColor( COLORS::eGREEN );
	glBegin( GL_LINES );
	for(int i=0;i<c.GetKeyCount();i++)
	{
		Vector2 ta =c.GetKey(i).mTangentInVector;
		glVertex2f( c.GetKey(i).mPosition, c.GetKey(i).mValue );
		glVertex2f( c.GetKey(i).mPosition + ta.x/10.0f, c.GetKey(i).mValue + ta.y/10.0f );
	}
	glEnd();
	gl_SetColor( COLORS::eRED );
	glBegin( GL_LINES );
	for(int i=0;i<c.GetKeyCount();i++)
	{
		Vector2 ta =c.GetKey(i).mTangentOutVector;
		glVertex2f( c.GetKey(i).mPosition, c.GetKey(i).mValue );
		glVertex2f( c.GetKey(i).mPosition + ta.x/10.0f, c.GetKey(i).mValue + ta.y/10.0f );
	}
	glEnd();
	
	gl_SetColor( COLORS::eGREY );
	glBegin( GL_POINTS );
		for(int i=0;i<c.GetKeyCount();i++)
		glVertex2f( c.GetKey(i).mPosition, c.GetKey(i).mValue );
	glEnd();

	/*
	gl_SetColor( COLORS::eGREY );
	glBegin( GL_LINES );
	float t0 = c.GetCurveTangent(0.0f);
	float t1 = c.GetCurveTangent(1.0f);
	Vector2 v0( cos(t0), sin(t0) );
	Vector2 v1( cos(t1), sin(t1) );
	glVertex2f( 0, 0 );
	glVertex2f( 0 + v0.x/10.0f,  0 + v0.y/10.0f );
	glVertex2f( 1, 1 );
	glVertex2f( 1 + v1.x/10.0f,  1 + v1.y/10.0f );
	glEnd();
	*/
	

	// Handles and spline points computation
	/*
	for(i=0;i<c.keys.size()-3;i+=3)
	{	
		gl_SetColor( COLORS::eWHITE );
		glBegin( GL_LINE_STRIP );
			for(float t=0;t<1;t+=CCurveEx::STEP)		// Curve line
			{
				c.ComputePointAt(i,t,v);
				glVertex2f( v.x, v.y );
			}
			c.ComputePointAt(i,1,v);
			glVertex2f( v.x, v.y );
		glEnd();

		gl_SetColor( COLORS::eORANGE );

		glBegin( GL_POINTS );
			glVertex2f( c.PointList[i+1].x, c.PointList[i+1].y );	// Handle point A
			glVertex2f( c.PointList[i+2].x, c.PointList[i+2].y );	// Handle point B
		glEnd();

		glBegin( GL_LINES );
			glVertex2f(	c.PointList[i  ].x, c.PointList[i  ].y );	// Handle line A
			glVertex2f( c.PointList[i+1].x, c.PointList[i+1].y );
			glVertex2f( c.PointList[i+2].x, c.PointList[i+2].y );	// Handle line B
			glVertex2f( c.PointList[i+3].x, c.PointList[i+3].y );
		glEnd();

		gl_SetColor( COLORS::eWHITE );
		glBegin( GL_POINTS );
			glVertex2f( c.PointList[i  ].x, c.PointList[i  ].y );	// Curve point A
			glVertex2f( c.PointList[i+3].x, c.PointList[i+3].y );	// Curve point B
		glEnd();
	}
	*/

	if( animate )
	{
		float timePos = GetTime() / ANIMATE_FACTOR; // Slow it by a factor
		float p = START_POS + (END_POS-START_POS)*(timePos - (int)(timePos));
		float v = c.Evaluate( p );
		Vector2 rmp(p,v);
		int x,y;
		EditorToScreen( rmp, x, y );

		float fTangent = c.EvaluateTangent( p );

		glPointSize( 16.0f );

		gl_SetColor( COLORS::eGREEN );
		glBegin( GL_POINTS );
			glVertex2f( rmp.x, rmp.y );
		glEnd();

		glBegin( GL_LINES );
			Vector2 rmt( cos(atan(fTangent)), sin(atan(fTangent)) );
			glVertex2f( rmp.x, rmp.y );
			glVertex2f( rmp.x + rmt.x/10.0f,  rmp.y + rmt.y/10.0f );
		glEnd();


		gl_SetColor( COLORS::eYELLOW );
		glBegin( GL_LINES );
			Vector2 v0 ( p, c.Evaluate(0.0f) );
			Vector2 v1 ( p, c.Evaluate(1.0f) );
			float dx = v1.x-v0.x;
			
			glVertex2f( v0.x + dx * p, v0.y );
			glVertex2f( v0.x + dx * p, v1.y );
		glEnd();
	}

//		for(i=0;i<PointList.size();i++)
//		{
//			gl_RenderText( c.PointList[i].x + 4, c.PointList[i].y, "%d (%0.2f,%0.2f)", i, c.PointList[i].x, c.PointList[i].y );	// Numbers
//		}
}


///////////////////////////////////////////////////////////////////////////////


void CurveEditor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "File..." );
		ADD_MENU_ITEM( pFile, "Save", &CurveEditor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "Load", &CurveEditor::OnMenuFileSave, 0 );
	
	CREATE_MENU( pPreLoop, "PreLoop..." );
		ADD_MENU_ITEM( pPreLoop, "Constant",	&CurveEditor::OnMenuPreLoop, CurveLoopType::Constant );
		ADD_MENU_ITEM( pPreLoop, "Cycle",		&CurveEditor::OnMenuPreLoop, CurveLoopType::Cycle );
		ADD_MENU_ITEM( pPreLoop, "CycleOffset",	&CurveEditor::OnMenuPreLoop, CurveLoopType::CycleOffset );
		ADD_MENU_ITEM( pPreLoop, "Oscillate",	&CurveEditor::OnMenuPreLoop, CurveLoopType::Oscillate );
		ADD_MENU_ITEM( pPreLoop, "Linear",		&CurveEditor::OnMenuPreLoop, CurveLoopType::Linear );

	CREATE_MENU( pPostLoop, "PostLoop..." );
		ADD_MENU_ITEM( pPostLoop, "Constant",		&CurveEditor::OnMenuPostLoop, CurveLoopType::Constant );
		ADD_MENU_ITEM( pPostLoop, "Cycle",			&CurveEditor::OnMenuPostLoop, CurveLoopType::Cycle );
		ADD_MENU_ITEM( pPostLoop, "CycleOffset",	&CurveEditor::OnMenuPostLoop, CurveLoopType::CycleOffset );
		ADD_MENU_ITEM( pPostLoop, "Oscillate",		&CurveEditor::OnMenuPostLoop, CurveLoopType::Oscillate );
		ADD_MENU_ITEM( pPostLoop, "Linear",			&CurveEditor::OnMenuPostLoop, CurveLoopType::Linear );
	
	ADD_MENU_ITEM( GetMenu(), "Animate",	&CurveEditor::OnMenuAnimate, 0 );
	ADD_MENU_ITEM( GetMenu(), "Texture",	&CurveEditor::OnMenuTexture, 0 );	
}

void CurveEditor::OnMenuFileSave( int inUnused )
{

}

void CurveEditor::OnMenuFileLoad( int inUnused )
{

}

void CurveEditor::OnMenuAnimate( int inUnused )
{
	animate = !animate;
	ResetTime();
}

bool ismultpow2( unsigned int i )
{
	unsigned int x = i;

	// Count the number of bits
	x =   x - ( ( x >> 1 ) & 0x55555555 );
	x = ( x & 0x33333333 ) + ((x>>2) & 0x33333333);
	x = ( x + ( x >> 4 ) ) & 0X0F0F0F0F;
	x =   x + ( x >> 8 );
	x =   x + ( x >>16 );
	x =   x & 0x0000003F;

	return x == 1 && i != 1;	// Theres only one bit and its not number 1
}

inline unsigned short clp2( unsigned short in_nToConvert )
{
	in_nToConvert -= 1;
	in_nToConvert |= in_nToConvert >> 1;
	in_nToConvert |= in_nToConvert >> 2;
	in_nToConvert |= in_nToConvert >> 4;
	in_nToConvert |= in_nToConvert >> 8;
	return in_nToConvert + 1;
}

void CurveEditor::OnMenuTexture( int inUnused )
{
	const char* szFilename = GetFileLoad( OPENFILE_TGA_FILTER, OPENFILE_TGA_EXTENSION );

	if( szFilename )
	{
		/*
		// Here we make sure the image falls into the opengl standard (ie. texture size is multiple of power of 2)
		CZImage* pImage = 0;
		BLITZ_IM->Load( &pImage, filename );
		short w = (short)pImage->GetWidth();
		short h = (short)pImage->GetHeight();

		if( ismultpow2(w) == false )
			w = clp2( w );
		if( ismultpow2(h) == false )
			h = clp2( h );

		 g_App.appTexture.u = pImage->GetWidth() / (float)w;
		 g_App.appTexture.v = pImage->GetHeight() / (float)h;

		//pImage->CanvasSizeNearest( w, h );
		pImage->ResizeNearest( w, h );
		IZTexture* pTexture = BLITZ_TM->LoadFromImage( "appTexture", pImage );

		delete pImage;
		pImage = 0;
		
		if( pTexture )
		{
			if( g_App.appTexture.pTexture )
			{
				BLITZ_TM->Unload( &g_App.appTexture.pTexture );
			}
			g_App.appTexture.pTexture = pTexture;
		}
		*/
	}
}

void CurveEditor::OnMenuPreLoop( int inPreLoop )
{
	c.SetPreLoop( (CurveLoopType::E)inPreLoop );
}

void CurveEditor::OnMenuPostLoop( int inPostLoop )
{
	c.SetPreLoop( (CurveLoopType::E)inPostLoop );
}

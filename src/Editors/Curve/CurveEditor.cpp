
//TODO
//- Implement insertion/deletion/manipulation of keys


#include "CurveEditor.h"

#include "gfx.h"

#include "../Grid.h"
#include "Lair/Lair.h"
#include "Lair/Camera/Camera.h"

void CurveEditor::OnInit()
{
	GetCamera()->GetPos().Set( 256.0f, 256.0f );

	mCurveSelection = 0;

	mCurve.AddKey(   0,	  0.0f,	Vector2(-1.0f,	0.0f),	Vector2(1.0f,	0.0f),	CurveContinuity::Smooth );
	mCurve.AddKey( 128,	128.0f,	Vector2(-1.0f,	0.0f),	Vector2(1.0f,  -4.0f),	CurveContinuity::Smooth );
	mCurve.AddKey( 256,	256.0f,	Vector2(-1.0f,	0.0f),	Vector2(1.0f,	0.0f),	CurveContinuity::Smooth );
	mCurve.AddKey( 512,	512.0f,	Vector2(-1.0f,	0.0f),	Vector2(1.0f,	4.0f),	CurveContinuity::Smooth );
}

void CurveEditor::OnExit()
{
}

#define ANIMATE_FACTOR	20.0f

void CurveEditor::OnRender()
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

	glPointSize( 8.0f );
	glLineWidth( 2.0f );

	// Render Curve
	RenderCurve();
		
	// Render selection
	glPointSize( 12.0f );

	if( mCurveSelection )
	{
		gl_SetColor( COLORS::eRED );
		glBegin( GL_POINTS );
			glVertex2f( (GLfloat)mCurve.GetKey(mCurveSelection->GetSelectedPointIndex()).mPosition, mCurve.GetKey(mCurveSelection->GetSelectedPointIndex()).mValue );
		glEnd();
	}
}

void CurveEditor::OnRenderGUI()
{
	gl_SetColor(COLORS::eWHITE);
	gl_RenderText( 8, 8, "Zoom: %f -- Grid: %d -- Snap: %s", GetCamera()->GetZoom(), GetGrid()->GetGridSize(), GetGrid()->GetSnap() ? "true" : "false" );
	gl_RenderText( 8, 720-16-8, "Curved v%d.%d.%d (%s at %s)", CRV_VERSION_MAIN, CRV_VERSION_MAJOR, CRV_VERSION_MINOR, __DATE__, __TIME__ );

	float SPAN = (float)(mCurve.GetKey(mCurve.GetKeyCount()-1).mPosition - mCurve.GetKey(0).mPosition);
	float START_POS = (float)(mCurve.GetKey(0).mPosition - SPAN);
	float MID_START_POS = (float)(mCurve.GetKey(0).mPosition);
	float MID_END_POS = (float)(mCurve.GetKey(mCurve.GetKeyCount()-1).mPosition);
	float END_POS = (float)(mCurve.GetKey(mCurve.GetKeyCount()-1).mPosition + SPAN);
	
	if( mAnimate )
	{
		float timePos = GetTime() / ANIMATE_FACTOR; // Slow it by a factor
		float p = START_POS + (END_POS-START_POS) * (timePos - (int)(timePos));
		
		float v = mCurve.Evaluate( p );
		Vector2 rmp(p,v);
		int x,y;
		EditorToScreen( rmp, x, y );
			
		gl_RenderText( x + 12, y - 12, "t: %0.2f", p );
		gl_RenderText( x + 12, y - 24, "v: %0.2f, %0.2f", rmp.x, rmp.y );

		gl_SetColor( COLORS::eYELLOW );
		glBegin( GL_LINES );
			glVertex2f( (float)x, 0.0f );
			glVertex2f( (float)x, 720.0f );
		glEnd();
	}

	if( mCurveSelection )
	{
		int x,y;
		Vector2 v( (float)mCurve.GetKey(mCurveSelection->GetSelectedPointIndex()).mPosition, mCurve.GetKey(mCurveSelection->GetSelectedPointIndex()).mValue );
		EditorToScreen( v, x, y );
		gl_RenderText( x + 12, y + 12, "%f, %f", v.x, v.y );
	}
}

void CurveEditor::RenderCurve()
{
	int i = 0;

	float SPAN = (float)(mCurve.GetKey(mCurve.GetKeyCount()-1).mPosition - mCurve.GetKey(0).mPosition);
	float START_POS = (float)(mCurve.GetKey(0).mPosition - SPAN);
	float MID_START_POS = (float)(mCurve.GetKey(0).mPosition);
	float MID_END_POS = (float)(mCurve.GetKey(mCurve.GetKeyCount()-1).mPosition);
	float END_POS = (float)(mCurve.GetKey(mCurve.GetKeyCount()-1).mPosition + SPAN);

	float DELTA = 1.0f;

	// Curve line
	glBegin( GL_LINE_STRIP );
	gl_SetColor( COLORS::eDARKGREY );
	for(float t=START_POS;t<MID_START_POS; t+= DELTA )		
		glVertex2f( (GLfloat)t, mCurve.Evaluate(t) );
	gl_SetColor( COLORS::eWHITE );
	for(float t=MID_START_POS;t<MID_END_POS; t+= DELTA )		
		glVertex2f( (GLfloat)t, mCurve.Evaluate(t) );
	gl_SetColor( COLORS::eDARKGREY );
	for(float t=MID_END_POS;t<END_POS; t+= DELTA )		
		glVertex2f( (GLfloat)t, mCurve.Evaluate(t) );
	glEnd();
	glVertex2f( END_POS, mCurve.Evaluate(END_POS) );

	// Tangents
	// TODO
#define NORMAL_LENGTH	64.0f
	gl_SetColor( COLORS::eGREEN );
	glBegin( GL_LINES );
	for(unsigned int i=0;i<mCurve.GetKeyCount();i++)
	{
		Vector2 ta =mCurve.GetKey(i).mTangentInVector;
		glVertex2f( (GLfloat)mCurve.GetKey(i).mPosition, mCurve.GetKey(i).mValue );
		glVertex2f( (GLfloat)mCurve.GetKey(i).mPosition + ta.x*NORMAL_LENGTH, mCurve.GetKey(i).mValue + ta.y*NORMAL_LENGTH );
	}
	glEnd();
	gl_SetColor( COLORS::eRED );
	glBegin( GL_LINES );
	for(unsigned int i=0;i<mCurve.GetKeyCount();i++)
	{
		Vector2 ta =mCurve.GetKey(i).mTangentOutVector;
		glVertex2f( (GLfloat)mCurve.GetKey(i).mPosition, mCurve.GetKey(i).mValue );
		glVertex2f( (GLfloat)mCurve.GetKey(i).mPosition + ta.x*NORMAL_LENGTH, mCurve.GetKey(i).mValue + ta.y*NORMAL_LENGTH );
	}
	glEnd();
	
	gl_SetColor( COLORS::eGREY );
	glBegin( GL_POINTS );
		for(unsigned int i=0;i<mCurve.GetKeyCount();i++)
		glVertex2f( (GLfloat)mCurve.GetKey(i).mPosition, mCurve.GetKey(i).mValue );
	glEnd();

	if( mAnimate )
	{
		float timePos = GetTime() / ANIMATE_FACTOR; // Slow it by a factor
		float p = START_POS + (END_POS-START_POS)*(timePos - (int)(timePos));
		float v = mCurve.Evaluate( p );
		Vector2 rmp(p,v);
		int x,y;
		EditorToScreen( rmp, x, y );

		float fTangent = mCurve.EvaluateTangent( p );

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
	}
}

void CurveEditor::OnMouseClick( int button, int state, int x, int y, int mod )
{
	switch(button)
	{
	case 0:		
		{		
			Vector2 v;	
			ScreenToEditor( x, y, v );
			
			unsigned int nKeyIndex = -1;
			for( unsigned int i=0; i<mCurve.GetKeyCount(); i++ )
			{
				if( fabsf(v.x-mCurve.GetKey(i).mPosition) < 0.1f && 
					fabsf(v.y-mCurve.GetKey(i).mValue) < 0.1f )
				{
					nKeyIndex = i;
					break;
				}
			}

			if( nKeyIndex != -1 )
			{
				mCurveSelection = &mDummyCurveSelection;
				mCurveSelection->Set( &mCurve, nKeyIndex );
			}
			else
			{
				mCurveSelection = 0;
			}

			if( Lair::GetInputMan()->GetMouseButtonState(button).bState == false )
				mCurveSelection = 0;
		}
		break;
	case 1:
		{
			/* FIXME
			if( g_App.Buttons[b] == 0 )
			{	
				float r;	
				Vector2 newpt;
				if( g_App.c.FindClosestPoint( v, 0.01f, newpt, index, r ) )
				{
					g_App.c.AddPointAtR( index, r );
				}
			}
			*/
		}
		break;
	case 2:		
		{
		}
		break;
	}
}

void CurveEditor::OnMouseMotion( int x, int y, int dx, int dy, int mod )
{
	if( mCurveSelection )
	{
		Vector2 last; 
		ScreenToEditor( x-dx, y-dy, last );

		Vector2 v;	
		ScreenToEditor( x, y, v );
		Vector2 d = v - last;

		mCurveSelection->OnDrag(v,d);
	}
	else
	{
		Editor::OnMouseMotion(x,y,dx,dy,mod);
	}
}

void CurveEditor::OnCreateMenu()
{	
	CREATE_MENU( pFile, "  File..." );
		ADD_MENU_ITEM( pFile, "  Save  ", &Editor::OnMenuFileSave, 0 );
		ADD_MENU_ITEM( pFile, "  Load  ", &Editor::OnMenuFileLoad, 0 );
	
	CREATE_MENU( pPreLoop, "  PreLoop...  " );
		ADD_MENU_ITEM( pPreLoop, "  Constant  ",	&CurveEditor::OnMenuPreLoop, CurveLoopType::Constant );
		ADD_MENU_ITEM( pPreLoop, "  Cycle  ",		&CurveEditor::OnMenuPreLoop, CurveLoopType::Cycle );
		ADD_MENU_ITEM( pPreLoop, "  CycleOffset  ",	&CurveEditor::OnMenuPreLoop, CurveLoopType::CycleOffset );
		ADD_MENU_ITEM( pPreLoop, "  Oscillate  ",	&CurveEditor::OnMenuPreLoop, CurveLoopType::Oscillate );
		ADD_MENU_ITEM( pPreLoop, "  Linear  ",		&CurveEditor::OnMenuPreLoop, CurveLoopType::Linear );

	CREATE_MENU( pPostLoop, "  PostLoop...  " );
		ADD_MENU_ITEM( pPostLoop, "  Constant  ",		&CurveEditor::OnMenuPostLoop, CurveLoopType::Constant );
		ADD_MENU_ITEM( pPostLoop, "  Cycle  ",			&CurveEditor::OnMenuPostLoop, CurveLoopType::Cycle );
		ADD_MENU_ITEM( pPostLoop, "  CycleOffset  ",	&CurveEditor::OnMenuPostLoop, CurveLoopType::CycleOffset );
		ADD_MENU_ITEM( pPostLoop, "  Oscillate  ",		&CurveEditor::OnMenuPostLoop, CurveLoopType::Oscillate );
		ADD_MENU_ITEM( pPostLoop, "  Linear  ",			&CurveEditor::OnMenuPostLoop, CurveLoopType::Linear );
	
	ADD_MENU_ITEM( GetMenu(), "  Animate  ",	&CurveEditor::OnMenuAnimate, 0 );
	ADD_MENU_ITEM( GetMenu(), "  Texture  ",	&CurveEditor::OnMenuTexture, 0 );	
}

void CurveEditor::OnMenuAnimate( int inUnused )
{
	mAnimate = !mAnimate;
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
	/*const char* szFilename = GetFileLoad( OPENFILE_TGA_FILTER, OPENFILE_TGA_EXTENSION );

	if( szFilename )
	{		
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
		
	}*/
}

void CurveEditor::OnMenuPreLoop( int inPreLoop )
{
	mCurve.SetPreLoop( (CurveLoopType::E)inPreLoop );
}

void CurveEditor::OnMenuPostLoop( int inPostLoop )
{
	mCurve.SetPostLoop( (CurveLoopType::E)inPostLoop );
}


void CurveEditor::OnSerializeSave( TiXmlElement* inNode )
{
	TiXmlElement* pxmlCurve = new TiXmlElement("curve");

	const char *  szCurveLoopType[] = { "Constant", "Cycle", "CycleOffset", "Oscillate", "Linear" };

	pxmlCurve->SetAttribute( "PreLoop", szCurveLoopType[mCurve.GetPreLoop()] );
	pxmlCurve->SetAttribute( "PostLoop", szCurveLoopType[mCurve.GetPostLoop()] );

	const char *  szCurveContinuity[] = { "Smooth", "Step" };

	TiXmlElement* pxmlKey;
	for( unsigned int i=0;i<mCurve.GetKeyCount(); i++ )
	{
		pxmlKey = new TiXmlElement("key");
		pxmlKey->SetAttribute( "position", mCurve.GetKey(i).mPosition );
		pxmlKey->SetDoubleAttribute( "value", mCurve.GetKey(i).mValue );
		pxmlKey->SetDoubleAttribute( "tix", mCurve.GetKey(i).mTangentInVector.x );
		pxmlKey->SetDoubleAttribute( "tiy", mCurve.GetKey(i).mTangentInVector.y );
		pxmlKey->SetDoubleAttribute( "tox", mCurve.GetKey(i).mTangentOutVector.x );
		pxmlKey->SetDoubleAttribute( "toy", mCurve.GetKey(i).mTangentOutVector.y );
		pxmlKey->SetAttribute( "continuity", szCurveContinuity[mCurve.GetKey(i).mContinuity] );

		pxmlCurve->LinkEndChild(pxmlKey);
	}	

	inNode->LinkEndChild(pxmlCurve);
}

void CurveEditor::OnSerializeLoad( TiXmlElement* inNode )
{
	TiXmlElement* pxmlCurve = inNode->FirstChildElement("curve");

	if( pxmlCurve )
	{
		const char* szPreLoop = pxmlCurve->Attribute( "PreLoop" );
		if( szPreLoop )
		{
				 if( _stricmp(szPreLoop, "Constant" ) == 0 )	mCurve.SetPreLoop( CurveLoopType::Constant );
			else if( _stricmp(szPreLoop, "Cycle" ) == 0 )		mCurve.SetPreLoop( CurveLoopType::Cycle );
			else if( _stricmp(szPreLoop, "CycleOffset" ) == 0 )	mCurve.SetPreLoop( CurveLoopType::CycleOffset );
			else if( _stricmp(szPreLoop, "Oscillate" ) == 0 )	mCurve.SetPreLoop( CurveLoopType::Oscillate );
			else if( _stricmp(szPreLoop, "Linear" ) == 0 )		mCurve.SetPreLoop( CurveLoopType::Linear );
			else												mCurve.SetPreLoop( CurveLoopType::Linear );
		}
		
		const char* szPostLoop = pxmlCurve->Attribute( "PostLoop" );
		if( szPostLoop )
		{
				 if( _stricmp(szPostLoop, "Constant" ) == 0 )	mCurve.SetPostLoop( CurveLoopType::Constant );
			else if( _stricmp(szPostLoop, "Cycle" ) == 0 )		mCurve.SetPostLoop( CurveLoopType::Cycle );
			else if( _stricmp(szPostLoop, "CycleOffset" ) == 0 )mCurve.SetPostLoop( CurveLoopType::CycleOffset );
			else if( _stricmp(szPostLoop, "Oscillate" ) == 0 )	mCurve.SetPostLoop( CurveLoopType::Oscillate );
			else if( _stricmp(szPostLoop, "Linear" ) == 0 )		mCurve.SetPostLoop( CurveLoopType::Linear );
			else												mCurve.SetPostLoop( CurveLoopType::Linear );
		}		

		int nPosition;
		float fValue;
		double fAttribute;
		Vector2 vTangentIn;
		Vector2 vTangentOut;
		CurveContinuity::E eCurveContinuity;
		const char* szCurveContinuity;

		mCurve.ClearKeys();	// fixme should be better handled

		for( TiXmlElement* pxmlKey = pxmlCurve->FirstChildElement("key"); pxmlKey; pxmlKey = pxmlKey->NextSiblingElement("key") )
		{			
			pxmlKey->Attribute( "position", &nPosition );
			pxmlKey->Attribute( "value", &fAttribute ); fValue = (float)fAttribute;
			pxmlKey->Attribute( "tix", &fAttribute ); vTangentIn.x = (float)fAttribute;
			pxmlKey->Attribute( "tiy", &fAttribute ); vTangentIn.y = (float)fAttribute;
			pxmlKey->Attribute( "tox", &fAttribute ); vTangentOut.x = (float)fAttribute;
			pxmlKey->Attribute( "toy", &fAttribute ); vTangentOut.y = (float)fAttribute;
			szCurveContinuity = pxmlKey->Attribute( "continuity" );

			if( szCurveContinuity )
			{
				if( _stricmp( szCurveContinuity, "Smooth" ) == 0 )		eCurveContinuity = CurveContinuity::Smooth;
				else if( _stricmp( szCurveContinuity, "Step" ) == 0 )	eCurveContinuity = CurveContinuity::Step;
			}

			mCurve.AddKey( nPosition, fValue, vTangentIn, vTangentOut, eCurveContinuity );
		}	
	}
}
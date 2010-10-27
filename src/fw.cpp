
#include "fw.h"

int g_time = 0;
int g_delta_time = 0;

#include "app.h"

#include "Game/Engine.h"
#include "Game/Camera.h"

void Screen_2_App( int x, int y, Vector2& v )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = g_App.GetEngine()->GetCamera()->GetPos();
	float fZoom = g_App.GetEngine()->GetCamera()->GetZoom();

	// Construct matrices used in curver rendering and grab them to undo the transformations
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -g_App.w/2*fZoom, g_App.w/2*fZoom, -g_App.h/2*fZoom,  g_App.h/2*fZoom );
		glGetDoublev( GL_MODELVIEW_MATRIX, P );

		glLoadIdentity();
		glTranslatef( -vPos.x, -vPos.y, 0.0f );
		glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluUnProject( x,y,0, M, P, V, &X, &Y, &Z );

	v.x = (float)X;
	v.y = (float)Y;
}

void App_2_Screen( const Vector2& v, int& x, int& y )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = g_App.GetEngine()->GetCamera()->GetPos();
	float fZoom = g_App.GetEngine()->GetCamera()->GetZoom();

	// Construct matrices used in curver rendering and grab them to undo the transformations
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -g_App.w/2*fZoom, g_App.w/2*fZoom, -g_App.h/2*fZoom,  g_App.h/2*fZoom );
		glGetDoublev( GL_MODELVIEW_MATRIX, P );

		glLoadIdentity();
		glTranslatef( -vPos.x, -vPos.y, 0.0f );
		glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluProject( v.x,v.y,0, M, P, V, &X, &Y, &Z );

	x = (int)X;
	y = (int)Y;
}

void glut_OnDisplay( void )
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glLoadIdentity();

	g_App.Render();
	
	glutSwapBuffers();
}

void glut_OnReshape( int w, int h )
{
	//todo: properly redirect reshape to app
	g_App.w = w;
	g_App.h = h;

	glViewport(0,0,w,h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0, w, 0, h );
	glMatrixMode(GL_MODELVIEW);
}



void glut_OnMotion(int x,int y)
{
	y = g_App.h - y;

	int diffx=x-g_App.last_mx;
	int diffy=y-g_App.last_my;
	
	Vector2 last; 
	Screen_2_App( g_App.last_mx, g_App.last_my, last );

	g_App.mx=x;
	g_App.my=y;
	g_App.last_mx=x;
	g_App.last_my=y;

	Vector2 v;	
	Screen_2_App( x, y, v );
	Vector2 d = v - last;	

	//todo: properly redirect drag motion to app
	{
		if( g_App.Buttons[GLUT_LEFT_BUTTON] )
		{
			//todo: scrolling here using d.x, d.y
		}
	}

	glutPostRedisplay();
}

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif


void glut_OnMouse(int b,int s,int x,int y)
{
	y = g_App.h - y;

	g_App.mx = x;
	g_App.my = y;
	g_App.last_mx=x;
	g_App.last_my=y;
		
	Vector2 v; Screen_2_App( x, y, v );
	
	//todo: properly redirect button states to app
	g_App.Buttons[b] = ((GLUT_DOWN==s)?1:0);

	switch(b)
	{
	case GLUT_LEFT_BUTTON:		g_App.OnLeftClick(x, y, v);	break;	
	case GLUT_MIDDLE_BUTTON:	g_App.OnMiddleClick(x, y, v);	break;	
	case GLUT_RIGHT_BUTTON:		g_App.OnRightClick(x, y, v);	break;	
	case GLUT_WHEEL_UP:			g_App.OnWheelUp();		break;	
	case GLUT_WHEEL_DOWN:		g_App.OnWheelDown();	break;	
	default:
		break;
	}

	glutPostRedisplay();
}

void glut_OnKeyboard(unsigned char key, int posX, int posY )
{	
	g_App.OnKeyboard( key );
}

void glut_OnSpecialKey( int key, int posX, int posY )
{	
	//todo: redirect special keys to app
	const float KeyOffset = 0.01f;

	switch(key)
	{
		case GLUT_KEY_F1:			break;
		case GLUT_KEY_F2:			break;
		case GLUT_KEY_F3:			break;
		case GLUT_KEY_F4:			break;
		case GLUT_KEY_F5:			break;
		case GLUT_KEY_F6:			break;
		case GLUT_KEY_F7:			break;
		case GLUT_KEY_F8:			break;
		case GLUT_KEY_F9:			break;
		case GLUT_KEY_F10:			break;
		case GLUT_KEY_F11:			break;
		case GLUT_KEY_F12:			break;
		
		case GLUT_KEY_LEFT:			break;
		case GLUT_KEY_DOWN:			break;
		case GLUT_KEY_RIGHT:		break;
		case GLUT_KEY_UP:			break;

		case GLUT_KEY_PAGE_UP:		break;
		case GLUT_KEY_PAGE_DOWN:	break;
		case GLUT_KEY_HOME:			break;
		case GLUT_KEY_END:			break;
		case GLUT_KEY_INSERT:		break;
	}
}

void glut_OnJoystickEx( unsigned int gamepad, unsigned int buttons, int axe_count, float* axe_values )
{	
	g_App.OnGamepad( gamepad, buttons, axe_count, axe_values );
}

void glut_OnIdle()
{
	g_delta_time =  glutGet(GLUT_ELAPSED_TIME) - g_time;
	g_time =  glutGet(GLUT_ELAPSED_TIME);

	g_App.Update( g_delta_time / 1000.0f );
	
	glutPostRedisplay();
}

void glut_OnExit()
{
	g_App.Exit();

	 menu_Destroy();
}

int main( int argc, char** argv )
{
     glutInit(&argc, argv);
	 glutInitDisplayMode   ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH  );

     glutInitWindowSize    ( 1280 , 800 );
     glutInitWindowPosition( 25 , 25 );
     
	 glutCreateWindow( "Delirium Wars alpha" );
	 
	 menu_Create();

	 g_App.Init();
     
	 glutDisplayFunc(glut_OnDisplay);
     glutReshapeFunc(glut_OnReshape);
	 glutMouseFunc(glut_OnMouse);
	 glutMotionFunc(glut_OnMotion);
	 glutKeyboardFunc(glut_OnKeyboard);
	 glutSpecialFunc(glut_OnSpecialKey);
	 glutJoystickExFunc( glut_OnJoystickEx, 0 );
	 glutIdleFunc( glut_OnIdle );	 

     gl_Init();

	 atexit(glut_OnExit);

	 glutMainLoop();

	 return 0;
}


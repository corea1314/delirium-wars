
#include "fw.h"

int g_time = 0;
int g_delta_time = 0;

#define MAX_DELTA_TIME	15

#include "app.h"

#include "Lair/Lair.h"
#include "Engine/Engine.h"
#include "Engine/Clock/Clock.h"

void Screen_2_App( int x, int y, Vector2& v )
{
	double P[16];
	double M[16];
	int V[4];

	Vector2 vPos = Lair::GetCameraMan()->GetActiveCamera()->GetPos();
	float fZoom = Lair::GetCameraMan()->GetActiveCamera()->GetZoom();
	float fAngle = Lair::GetCameraMan()->GetActiveCamera()->GetAngle();

	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -1024/2*fZoom, 1024/2*fZoom, -1024/2*fZoom,  1024/2*fZoom );
		glGetDoublev( GL_MODELVIEW_MATRIX, P );

		glLoadIdentity();
		glRotatef( fAngle, 0.0f, 0.0f, 1.0f );
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

	Vector2 vPos = Lair::GetCameraMan()->GetActiveCamera()->GetPos();
	float fZoom = Lair::GetCameraMan()->GetActiveCamera()->GetZoom();
	float fAngle = Lair::GetCameraMan()->GetActiveCamera()->GetAngle();

	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -1024/2*fZoom,1024/2*fZoom, -1024/2*fZoom,  1024/2*fZoom );
		glGetDoublev( GL_MODELVIEW_MATRIX, P );

		glLoadIdentity();
		glRotatef( fAngle, 0.0f, 0.0f, 1.0f );
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
	case GLUT_LEFT_BUTTON:
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:	
		g_App.OnMouseClick( b, x, y, v );	
		break;
	case GLUT_WHEEL_UP:			g_App.OnWheelUp();		break;	
	case GLUT_WHEEL_DOWN:		g_App.OnWheelDown();	break;	
	default:
		break;
	}
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
}

void glut_OnExit()
{
	g_App.Exit();

	 menu_Destroy();
}

void gl_Init( void )
{
	GLeeInit();
	
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glShadeModel(GL_SMOOTH);

	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable (GL_POINT_SMOOTH);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_ALPHA_TEST );
}

void glut_OnTimer( int k )
{
	g_delta_time =  glutGet(GLUT_ELAPSED_TIME) - g_time;
	g_time =  glutGet(GLUT_ELAPSED_TIME);

	/*
	char szTitle[32];
	sprintf( szTitle, "%d", g_delta_time );
	glutSetWindowTitle( szTitle );
	*/
	int delta_time = g_delta_time;
	while( delta_time > MAX_DELTA_TIME )
	{
		g_App.Update( MAX_DELTA_TIME / 1000.0f );
		delta_time -= MAX_DELTA_TIME;
	}
	g_App.Update( delta_time / 1000.0f );

	glutTimerFunc( MAX_DELTA_TIME, glut_OnTimer, 0 );
	glutPostRedisplay();
}

int main( int argc, char** argv )
{
     glutInit(&argc, argv);
	 glutInitDisplayMode   ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL  );

     glutInitWindowSize    ( SCREEN_SIZE_X , SCREEN_SIZE_Y );
     glutInitWindowPosition( 0 , 0 );
     
	 glutCreateWindow( "Delirium Wars alpha" );
	 
	 menu_Create();

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

	 glutTimerFunc( MAX_DELTA_TIME, glut_OnTimer, 0 );

	 g_delta_time = 0;
	 g_time =  glutGet(GLUT_ELAPSED_TIME);
	 g_App.Init();

	 glutMainLoop();

	 return 0;
}


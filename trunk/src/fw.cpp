
#include "fw.h"

int g_time = 0;
int g_delta_time = 0;

#define MAX_DELTA_TIME	15

#include "app.h"

void glut_OnDisplay( void )
{
	g_App.Render();
	
	glutSwapBuffers();
}

void glut_OnReshape( int w, int h )
{
	g_App.OnReshape( w,h );
}

void glut_OnMotion(int x,int y)
{
	int m = glutGetModifiers();
	g_App.OnMouseMotion( x, y, m );
}

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

void glut_OnMouse(int b,int s,int x,int y)
{
	s = ((GLUT_DOWN==s)?1:0);

	int mod = glutGetModifiers();
	
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:	
		g_App.OnMouseClick( b, s, x, y, mod );	
		break;
	case GLUT_WHEEL_UP:		g_App.OnMouseWheel( 1, mod);	break;	
	case GLUT_WHEEL_DOWN:	g_App.OnMouseWheel(-1, mod);	break;	
	default:
		break;
	}
}

void glut_OnKeyboard(unsigned char key, int posX, int posY )
{	
	g_App.OnKeyboard( key, glutGetModifiers(), true );
}

void glut_OnKeyboardUp(unsigned char key, int posX, int posY )
{	
	g_App.OnKeyboard( key, glutGetModifiers(), false );
}

void glut_OnSpecialKey( int key, int posX, int posY )
{		
	g_App.OnSpecialKey( key, glutGetModifiers(), true );

	/*
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
	*/
}

void glut_OnSpecialKeyUp( int key, int posX, int posY )
{		
	g_App.OnSpecialKey( key, glutGetModifiers(), false );
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
     
	 glutCreateWindow( "Lair Engine" );
	 
	 glutDisplayFunc(glut_OnDisplay);
     glutReshapeFunc(glut_OnReshape);
	 glutMouseFunc(glut_OnMouse);
	 glutMotionFunc(glut_OnMotion);
	 glutKeyboardFunc(glut_OnKeyboard);
	 glutKeyboardUpFunc(glut_OnKeyboardUp);
	 glutSpecialFunc(glut_OnSpecialKey);
	 glutSpecialUpFunc(glut_OnSpecialKeyUp);
	 glutJoystickExFunc( glut_OnJoystickEx, 0 );
	 glutIdleFunc( glut_OnIdle );

	 glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

     gl_Init();

	 glutTimerFunc( MAX_DELTA_TIME, glut_OnTimer, 0 );

	 g_delta_time = 0;
	 g_time =  glutGet(GLUT_ELAPSED_TIME);
	 g_App.Init();

	 glutMainLoop();
	 
	 glut_OnExit();

	 return 0;
}


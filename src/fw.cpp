
#include "fw.h"

int g_time = 0;
int g_delta_time = 0;

#include "app.h"

void Screen_2_App( int x, int y, Vector2& v )
{
	double P[16];
	double M[16];
	int V[4];

	// Construct matrices used in curver rendering and grab them to undo the transformations
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -0.5f / g_App.cam_zoom, 0.5f / g_App.cam_zoom, -0.5f / g_App.cam_zoom, 0.5f / g_App.cam_zoom );
		glGetDoublev( GL_MODELVIEW_MATRIX, P );

		glLoadIdentity();
		glTranslatef( -g_App.cam_pos_x, -g_App.cam_pos_y, 0.0f );
		glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluUnProject( x,y,0, M, P, V, &X, &Y, &Z );

	v.x = X;
	v.y = Y;
}

void App_2_Screen( const Vector2& v, int& x, int& y )
{
	double P[16];
	double M[16];
	int V[4];

	// Construct matrices used in curver rendering and grab them to undo the transformations
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( -0.5f / g_App.cam_zoom, 0.5f / g_App.cam_zoom, -0.5f / g_App.cam_zoom, 0.5f / g_App.cam_zoom );
		glGetDoublev( GL_MODELVIEW_MATRIX, P );

		glLoadIdentity();
		glTranslatef( -g_App.cam_pos_x, -g_App.cam_pos_y, 0.0f );
		glGetDoublev( GL_MODELVIEW_MATRIX, M );
	glPopMatrix();

	glGetIntegerv( GL_VIEWPORT, V );

	double X,Y,Z;

	gluProject( v.x,v.y,0, M, P, V, &X, &Y, &Z );

	x = X;
	y = Y;
}

void glut_OnDisplay( void )
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glLoadIdentity();

	g_App.Render();

//	gl_RenderText( g_App.mx, g_App.my, "%d, %d", g_App.mx, g_App.my );
//	gl_RenderText( 8, 16, "t: %0.2fs (dt: %0.3fs)", time/1000.0f, delta_time/1000.0f );
	
	glutSwapBuffers();
}

void glut_OnReshape( int w, int h )
{
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

	{
		if( g_App.Buttons[GLUT_LEFT_BUTTON] )
		{
			g_App.cam_pos_x -= d.x;
			g_App.cam_pos_y -= d.y;
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
	
	g_App.Buttons[b] = ((GLUT_DOWN==s)?1:0);

	switch(b)
	{
	case GLUT_LEFT_BUTTON:		
		{
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		{
		}
		break;
	case GLUT_RIGHT_BUTTON:		
		{
			
		}
		break;

	case GLUT_WHEEL_UP:
		if( g_App.Buttons[b] == 0 )
		{
			g_App.ZoomIn();	
		}
		break;

	case GLUT_WHEEL_DOWN:
		if( g_App.Buttons[b] == 0 )
		{
			g_App.ZoomOut();
		}
		break;
	default:
		break;		
	}

	glutPostRedisplay();
}

void glut_OnKeyboard(unsigned char key, int posX, int posY )
{	
	switch(key)
	{
	case 127:	// delete
		break;
	case '-':
		g_App.DecreaseGridSize();
		break;

	case '=':
		g_App.IncreaseGridSize();
		break;

	case ' ':
		break;
	}
}

void glut_OnSpecialKey( int key, int posX, int posY )
{	
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
		
		case GLUT_KEY_LEFT:			g_App.cam_pos_x += KeyOffset;	break;
		case GLUT_KEY_DOWN:			g_App.cam_pos_y += KeyOffset;	break;
		case GLUT_KEY_RIGHT:		g_App.cam_pos_x -= KeyOffset;	break;
		case GLUT_KEY_UP:			g_App.cam_pos_y -= KeyOffset;	break;

		case GLUT_KEY_PAGE_UP:		break;
		case GLUT_KEY_PAGE_DOWN:	break;
		case GLUT_KEY_HOME:			break;
		case GLUT_KEY_END:			break;
		case GLUT_KEY_INSERT:		break;
	}
}

void glut_OnIdle()
{
	g_delta_time =  glutGet(GLUT_ELAPSED_TIME) - g_time;
	g_time =  glutGet(GLUT_ELAPSED_TIME);

	g_App.Update( g_delta_time / 1000.0f );
	
	glutPostRedisplay();
}

int main( int argc, char** argv )
{
     glutInit(&argc, argv);
	 glutInitDisplayMode   ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH  );

     glutInitWindowSize    ( 1000 , 1000 );
     glutInitWindowPosition( 25 , 25 );
     
	 glutCreateWindow( "Curve Editor by uberyoji" );
	 
	 menu_Create();
     
	 glutDisplayFunc(glut_OnDisplay);
     glutReshapeFunc(glut_OnReshape);
	 glutMouseFunc(glut_OnMouse);
	 glutMotionFunc(glut_OnMotion);
	 glutKeyboardFunc(glut_OnKeyboard);
	 glutSpecialFunc(glut_OnSpecialKey);
	 glutIdleFunc( glut_OnIdle );

     gl_Init();

	 glutMainLoop();

	 menu_Destroy();

	 return 0;
}


/*


  // sample menu

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4

void processMenuEvents(int option) 
{
	switch (option) 
	{
		case RED:
			break;
		case GREEN:
			break;
		case BLUE:
			break;
		case WHITE:
			break;
	}
}

void createGLUTMenus() 
{
	int menu,submenu;

	submenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("child0",RED);
	glutAddMenuEntry("child1",BLUE);
	glutAddMenuEntry("child2",GREEN);

	menu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("root0",WHITE);
	glutAddSubMenu("root1",submenu);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

 */
	
	

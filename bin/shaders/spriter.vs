// simple vertex shader

/*
Vector2 pos;		// world space position
float	depth;		// world space depth
float	angle;		// normalized angle

Vector2 size;		// world space size (width,height)
Vector2 offset;		// world space offset about pos

Vector2 uv_min;		// texture coordinates
Vector2 uv_max;
				
Vector2	vel;		// normalized velocity
float	avel;		// normalized angular velocity
		
unsigned int	color;	// modulation color

pos		-> gl_Vertex.xy
depth	-> gl_Vertex.z
angle	-> gl_Vertex.w

size	-> g_MultiTexCoord0.xy
offset	-> g_MultiTexCoord0.zw

uv_min	-> g_MultiTexCoord1.xy
uv_max	-> g_MultiTexCoord1.zw
		
vel		-> g_MultiTexCoord2.xy
avel	-> g_MultiTexCoord2.z
		
color	-> gl_Color
*/

void main()
{
	gl_Position		= gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor	= gl_Color;
}

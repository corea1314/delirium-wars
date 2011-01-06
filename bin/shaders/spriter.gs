// simple geometry shader

// these lines enable the geometry shader support.
#version 120
#extension GL_EXT_geometry_shader4 : enable

uniform vec4 color;

uniform float size_x;
uniform float size_y;

void main( void )
{
	for( int i = 0 ; i < 1/*gl_VerticesIn*/ ; i++ )
	{
		gl_FrontColor  = gl_FrontColorIn[ i ];
		gl_Position    = gl_PositionIn  [ i ];
		gl_TexCoord[0] = vec4(0,0,1,1);
		gl_Position.x += -size_x;
		gl_Position.y += -size_y;
		EmitVertex();

//		gl_FrontColor  = gl_FrontColorIn[ i ];
		gl_Position    = gl_PositionIn  [ i ];
		gl_Position.x += -size_x;
		gl_Position.y +=  size_y;
		gl_TexCoord[0] = vec4(0,1,1,1);;
		EmitVertex();

//		gl_FrontColor  = gl_FrontColorIn[ i ];
		gl_Position    = gl_PositionIn  [ i ];
		gl_Position.x +=  size_x;
		gl_Position.y += -size_y;
		gl_TexCoord[0] = vec4(1,0,1,1);;
		EmitVertex();

//		gl_FrontColor  = gl_FrontColorIn[ i ];
		gl_Position    = gl_PositionIn  [ i ];
		gl_Position.x += size_x;
		gl_Position.y += size_y;
		gl_TexCoord[0] = vec4(1,1,1,1);;
		EmitVertex();
	}
	EndPrimitive();
}

// simple geometry shader

// these lines enable the geometry shader support.
#version 120
#extension GL_EXT_geometry_shader4 : enable

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

void main( void )
{
	vec4 pos;
	pos.xyz = g_Vertex.xyz;
	pos.w = 1;
	
	float angle	= gl_Vertex.w;

	vec2 size	= g_MultiTexCoord0.xy;
	vec2 offset	= g_MultiTexCoord0.zw;

	vec2 uv_min	= g_MultiTexCoord1.xy;
	vec2 uv_max	= g_MultiTexCoord1.zw;
		
	vec2 vel = g_MultiTexCoord2.xy;
	float avel = g_MultiTexCoord2.z;
	
	// output same color for all vertices	
	gl_FrontColor  = gl_FrontColorIn[ 0 ]; // vec4 = color = gl_Color

	gl_Position    = gl_PositionIn  [ 0 ];
	gl_TexCoord[0] = vec4(uv_min.x,uv_min.y,1,1);
	gl_Position.x += -size_x;
	gl_Position.y += -size_y;
	EmitVertex();

	gl_Position    = gl_PositionIn  [ i ];
	gl_Position.x += -size_x;
	gl_Position.y +=  size_y;
	gl_TexCoord[0] = vec4(uv_min.x,uv_max.y,1,1);;
	EmitVertex();

	gl_Position    = gl_PositionIn  [ i ];
	gl_Position.x +=  size_x;
	gl_Position.y += -size_y;
	gl_TexCoord[0] = vec4(uv_max.x,uv_min.y,1,1);;
	EmitVertex();

	gl_Position    = gl_PositionIn  [ i ];
	gl_Position.x += size_x;
	gl_Position.y += size_y;
	gl_TexCoord[0] = vec4(uv_max.x,uv_max.y,1,1);;
	EmitVertex();
}

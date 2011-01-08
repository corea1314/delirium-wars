// simple geometry shader

// these lines enable the geometry shader support.
#version 120
#extension GL_EXT_geometry_shader4 : enable

/*
		Vector2 pos;		// world space position
		float	depth;		// world space depth

		Vector2 size;		// world space size (width,height)

		Vector2 offset;		// world space offset about pos
		float	angle;		// normalized angle

		Vector2 uv_min;		// texture coordinates

		Vector2 uv_max;
*/

//#define pos	gl_Vertex.xy
#define depth	gl_PositionIn[0].z

#define size	gl_TexCoordIn[0][0].xy

#define offset	gl_TexCoordIn[0][1].xy
#define angle	gl_TexCoordIn[0][1].z

#define uv_min	gl_TexCoordIn[0][2].xy
#define uv_max	gl_TexCoordIn[0][3].xy
		
//#define vel	gl_TexCoordIn[0][2].xy
//#define avel	gl_TexCoordIn[0][2].z
		
#define color	gl_FrontColorIn[0]

void RotateVector( out vec2 vo, in float x, in float y, in float cosa, in float sina )
{
	vo.x = x * cosa - y * sina;
	vo.y = x * sina + y * cosa;
}

//todo: optimize here since we know we are rotation extent of a box, we can flip coordinates and sign instead of computing all 4

void main( void )
{
	float cosa = cos(angle);
	float sina = sin(angle);

	vec4 pos = vec4( gl_PositionIn[0].x, gl_PositionIn[0].y, gl_PositionIn[0].z, 1 );
	vec2 extent;

	// output same color for all vertices	
	gl_FrontColor  = color;

	RotateVector( extent, -size.x, -size.y, cosa, sina );
	gl_Position = gl_ModelViewProjectionMatrix * ( pos + vec4( extent.x, extent.y, 0, 0 ) );
	gl_TexCoord[0].xy = uv_min;
	EmitVertex();

	RotateVector( extent, -size.x,  size.y, cosa, sina );
	gl_Position = gl_ModelViewProjectionMatrix * ( pos + vec4( extent.x, extent.y, 0, 0 ) );
	gl_TexCoord[0].xy = vec2( uv_min.x, uv_max.y );
	EmitVertex();

	RotateVector( extent,  size.x, -size.y, cosa, sina );
	gl_Position = gl_ModelViewProjectionMatrix * ( pos + vec4( extent.x, extent.y, 0, 0 ) );
	gl_TexCoord[0].xy = vec2( uv_max.x, uv_min.y );
	EmitVertex();

	RotateVector( extent,  size.x,  size.y, cosa, sina );
	gl_Position = gl_ModelViewProjectionMatrix * ( pos + vec4( extent.x, extent.y, 0, 0 ) );
	gl_TexCoord[0].xy = uv_max;
	EmitVertex();
}

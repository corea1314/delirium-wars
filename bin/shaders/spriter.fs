// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;

uniform sampler2D diffuse;
uniform sampler2D normal;

varying vec2 texcoord;

void main()
{
	gl_FragColor = gl_Color * texture2D( diffuse, gl_TexCoord[0].xy );
}

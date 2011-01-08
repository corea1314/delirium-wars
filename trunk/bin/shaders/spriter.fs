// simple fragment shader

// 'time' contains seconds since the program was linked.

// uniform float time;
uniform sampler2D uTextureMap0;
uniform sampler2D uTextureMap1;
uniform sampler2D uTextureMap2;
uniform sampler2D uTextureMap3;

void main()
{
	gl_FragColor = gl_Color * texture2D( uTextureMap0, gl_TexCoord[0].xy );
}

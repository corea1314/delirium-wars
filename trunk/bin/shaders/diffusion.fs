
uniform sampler2D uTextureMap0;
uniform sampler2D uTextureMap1;

uniform float fadeout;
uniform vec2 displacement;

#define lastMap	uTextureMap0
#define currMap	uTextureMap1

/*
const float viscocity = 0.001;
const float diffusion = 0.0;
const float delta = 1.0 / 60.0;

vec4 point(vec2 off)
{
    return texture2D(lastMap, (gl_FragCoord + off) / 1024);
}

void main(void){
    float a = delta * diffusion * 1024 * 1024;
    float r = delta * viscocity * 1024 * 1024;
    vec4 sample = point(vec2( 0,  0));
    vec4 left   = point(vec2(-1,  0));
    vec4 right  = point(vec2( 1,  0));
    vec4 top    = point(vec2( 0,  1));
    vec4 bottom = point(vec2( 0, -1));
    sample.xy = (sample.xy + (left.xy+right.xy+top.xy+bottom.xy)*r)/(1+4*r);
    sample.z = (sample.z + (left.z+right.z+top.z+bottom.z)*a)/(1+4*a);

	vec4 lastColor = sample;

//	lastColor.a *= 0.9999999f;

	vec4 currColor = texture2D( currMap, gl_TexCoord[0].xy );

	vec4 resultColor;

	resultColor.rgb = (lastColor.rgb * (1-currColor.a)) + (currColor.rgb * currColor.a);
	resultColor.a = (lastColor.a * (1-currColor.a)) + (currColor.a * currColor.a);

//	resultColor.rgb = lastColor.rgb * lastColor.a + currColor.rgb * currColor.a;
//	resultColor.a = max(lastColor.a, currColor.a);

	gl_FragColor = resultColor;

}
*/


/*
void main()
{	
	vec2 tcDiffusion = gl_TexCoord[0].st;

//	vec2 disp = vec2( (gl_FragCoord.x-512)/512, (gl_FragCoord.y-512)/512 ) / 128;

//	disp.x += 0.001f;

	vec4 lastColor = texture2D( lastMap, gl_TexCoord[0].xy + displacement.xy );
	//lastColor.rgba *= fadeout;
	// lastColor.a -= 0.000001f;

	vec4 currColor = texture2D( currMap, gl_TexCoord[0].xy );

	vec4 resultColor;

	resultColor.rgb = (lastColor.rgb * (1-currColor.a)) + (currColor.rgb * currColor.a);
	resultColor.a = (lastColor.a * (1-currColor.a)) + (currColor.a * currColor.a);
	
	resultColor.a *= 0.99f;

//	resultColor.rgb = lastColor.rgb * lastColor.a + currColor.rgb * currColor.a;
//	resultColor.a = max(lastColor.a, currColor.a);

	gl_FragColor = resultColor;
}
*/

void main()
{	
	gl_FragColor = texture2D( currMap, gl_TexCoord[0].st );
}
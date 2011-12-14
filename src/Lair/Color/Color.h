#ifndef _COLOR_H
#define _COLOR_H

typedef unsigned int Color;
typedef unsigned char Channel;

inline Channel RedValue( Color c )		{ return (Channel)((c >> 24) & 0xFF); }
inline Channel GreenValue( Color c )	{ return (Channel)((c >> 16) & 0xFF); }
inline Channel BlueValue( Color c )		{ return (Channel)((c >>  8) & 0xFF); }
inline Channel AlphaValue( Color c )	{ return (Channel)((c      ) & 0xFF); }

inline Color ColorValue( Channel r, Channel g, Channel b, Channel a = 0xFF ) 
{
	Color _r = r, _g = g, _b = b, _a = a;
	return ((_r << 24) + (_g << 16) + (_b <<  8) + _a);
}


#define MAX3( a, b, c )	\
	( MAX( MAX(a,b), MAX(a,c) ) )

#define MIN3( a, b, c )	\
	( MIN( MIN(a,b), MIN(a,c) ) )

static float Hue_2_RGB( float v1, float v2, float vH )
{
	assert( vH < 2.0f );
	if ( vH < 0.0f ) vH += 1.0f;
	if ( vH > 1.0f ) vH -= 1.0f;
	if ( ( 6.0f * vH ) < 1.0f ) return ( v1 + ( v2 - v1 ) * 6.0f * vH );
	if ( ( 2.0f * vH ) < 1.0f ) return ( v2 );
	if ( ( 3.0f * vH ) < 2.0f ) return ( v1 + ( v2 - v1 ) * ( ( 2.0f / 3.0f ) - vH ) * 6.0f );
	return ( v1 );
}



void HSL_to_RGB(float h, float s, float l, float* r, float* g, float* b)
{
	float v1, v2;

	if ( s == 0.0f )                       //HSL values = From 0 to 1
	{
		*r = l;                     //RGB results = From 0 to 255
		*g = l;
		*b = l;
	}
	else
	{
		if ( l < 0.5f )
			v2 = l * ( 1.0f + s );
		else          
			v2 = ( l + s ) - ( s * l );

		v1 = 2 * l - v2;

		*r = Hue_2_RGB( v1, v2, h + ( 1.0f / 3.0f ) );
		*g = Hue_2_RGB( v1, v2, h );
		*b = Hue_2_RGB( v1, v2, h - ( 1.0f / 3.0f ) );
	}
}

bool RGB_to_HSL(float r, float g, float b, float* h, float* s, float* l)
{
	float vmin, vmax, delta;
	float dr,dg,db;

	vmin = Z_MIN3( r, g, b );
	vmax = Z_MAX3( r, g, b );
	delta = vmax - vmin;

	*l = ( vmax + vmin ) / 2;

	if ( delta == 0 )
	{
		*h = 0.0f;
		*s = 0.0f;

		//	 return false;
	}
	else
	{
		if ( *l < 0.5f ) *s = delta / ( vmax + vmin );
		else             *s = delta / ( 2.0f - vmax - vmin );

		dr = ( ( ( vmax - r ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
		dg = ( ( ( vmax - g ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
		db = ( ( ( vmax - b ) / 6.0f ) + ( delta / 2.0f ) ) / delta;

		if      ( r == vmax ) *h = db - dg;
		else if ( g == vmax ) *h = ( 1.0f / 3.0f ) + dr - db;
		else if ( b == vmax ) *h = ( 2.0f / 3.0f ) + dg - dr;

		if ( *h < 0.0f ) *h += 1.0f;
		if ( *h > 1.0f ) *h -= 1.0f;
	}
	return true;
}

int RGB_to_HSV(float r, float g, float b, float* h, float* s, float* v)
{
	float var_Min;
	float var_Max;
	float del_Max;

	var_Min  = Z_MIN3( r, g, b );    //Min. value of RGB
	var_Max  = Z_MAX3( r, g, g );    //Max. value of RGB
	del_Max = var_Max - var_Min;    //Delta RGB value 

	*v = var_Max;

	if ( del_Max == 0 )                      // This is a gray, no chroma...
	{
		*h = 0;                               // HSV results = From 0 to 1
		*s = 0;
	}
	else                                     // Chromatic data...
	{
		float del_R = ( ( ( var_Max - r ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;
		float del_G = ( ( ( var_Max - g ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;
		float del_B = ( ( ( var_Max - b ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;

		*s = del_Max / var_Max;

		if      ( r == var_Max ) *h = del_B - del_G;
		else if ( g == var_Max ) *h = ( 1.0f/3.0f ) + del_R - del_B;
		else if ( g == var_Max ) *h = ( 2.0f/3.0f ) + del_G - del_R;

		if ( *h < 0.0f )  *h += 1;
		if ( *h > 1.0f )  *h -= 1;
	}

	return 1;
}

int HSV_to_RGB(float h, float s, float v, float* r, float* g, float* b)
{
	if ( s == 0 )                       // HSV values = From 0 to 1
	{
		*r = v;
		*g = v;
		*b = v;
	}
	else
	{
		float var_h = h * 6.0f;
		long var_i = (long)( var_h );
		float var_1 = v * ( 1 - s );
		float var_2 = v * ( 1 - s * ( var_h - var_i ) );
		float var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

		if      ( var_i == 0 ) { *r = v     ; *g = var_3 ; *b = var_1; }
		else if ( var_i == 1 ) { *r = var_2 ; *g = v     ; *b = var_1; }
		else if ( var_i == 2 ) { *r = var_1 ; *g = v     ; *b = var_3; }
		else if ( var_i == 3 ) { *r = var_1 ; *g = var_2 ; *b = v;     }
		else if ( var_i == 4 ) { *r = var_3 ; *g = var_1 ; *b = v;     }
		else                   { *r = v     ; *g = var_1 ; *b = var_2; }

	}

	return 1;
}

class RGBA
{

};

class HSLV
{

};

#endif//_COLOR_H

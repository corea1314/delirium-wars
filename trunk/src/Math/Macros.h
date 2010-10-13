// ----------------------------------------------------------------------------
// Proprietary and Confidential - Copyright 2008 DTI Software
// ----------------------------------------------------------------------------
//! \file	Macros.h
//!			Generic macros.
// ----------------------------------------------------------------------------

#ifndef _MACROS_H
#define _MACROS_H

///////////////////////////////////////////////////////////////////////////////
// Computer related

/////
// BYTE
//! Defines byte
//
#ifndef BYTE
#define BYTE	unsigned char
#endif//BYTE

//////
// BYTESWAP_16
//!		Inverts the byte order of a 16-bit value.
//
#ifndef BYTESWAP_16
#define BYTESWAP_16(x)	\
	( (((x) >> 8) & 0x00FF) |	\
	  (((x) << 8) & 0xFF00) )	
#endif

//////
// BYTESWAP_32
//!		Inverts the byte order of a 32-bit value.
//
#ifndef BYTESWAP_32
#define BYTESWAP_32(x)	\
	( (((x) >> 24) & 0x000000FF) |	\
	  (((x) >>  8) & 0x0000FF00) |	\
	  (((x) <<  8) & 0x00FF0000) |	\
	  (((x) << 24) & 0xFF000000) )	
#endif

//////
// _bz_swap_float
//!		Inverts the byte order of a 32-bit float value.
//
inline float _bz_swap_float(
	float x)
{
	union
	{
		long l;
		float f;
	}	cnv;
	cnv.f = x;
	cnv.l = BYTESWAP_32(cnv.l);
	return cnv.f;
}

//////
// LOBYTE
//!		Returns the low-byte of a WORD.
//
#ifndef LOBYTE
	#define LOBYTE(x)	((x) & 0xFF)
#endif

//////
// HIBYTE
//!		Returns the hi-byte of a WORD.
//
#ifndef HIBYTE
	#define HIBYTE(x)	LOBYTE((x) >> 8)
#endif

//////
// LOWORD
//!		Returns the low-word of a DWORD.
//
#ifndef LOWORD
	#define LOWORD(x)	((x) & 0xFFFF)
#endif

//////
// HIWORD
//!		Returns the low-word of a DWORD.
//
#ifndef HIWORD
	#define HIWORD(x)	LOWORD((x) >> 16)
#endif

//////
// SET_BITS
//!		Ones the bits specified in the mask.
//
#ifndef SET_BITS
#define SET_BITS(p_val, p_mask)			\
	((p_val)|= (p_mask))
#endif


//////
// RESET_BITS
//!		Zeroes the bits specified in the mask.
//
#ifndef RESET_BITS
#define RESET_BITS(p_val, p_mask)		\
	((p_val)&= ~(p_mask))
#endif

//////
// SAFE_DELETE
//!		Safely frees the memory and sets the pointer to NULL.
//
#ifndef SAFE_DELETE
#define SAFE_DELETE(_p_p) do { delete (_p_p); (_p_p) = NULL; } while(0)
#endif

//////
// SAFE_DELETE_ARRAY
//!		Safely frees the memory of an array and sets the pointer to NULL.
//
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(_p_p) do { delete[] (_p_p); (_p_p) = NULL; } while (0)
#endif


#ifdef _USE_DEPRECATED
//////
// SAFE_DELETE_NOSET
//!		Safely frees the memory and doesn't set the pointer to NULL.
//
#ifndef SAFE_DELETE_NOSET
#define SAFE_DELETE_NOSET(_p_p) delete (_p_p)
#endif

//////
// SAFE_DELETE_ARRAY_NOSET
//!		Safely frees the memory of an array and doesn't set the pointer to NULL.
//
#ifndef SAFE_DELETE_ARRAY_NOSET
#define SAFE_DELETE_ARRAY_NOSET(_p_p) delete[] (_p_p)
#endif
#endif	// _USE_DEPRECATED

//////
// SAFE_DELETE_P_ARRAY
//!		Safely frees the memory of pointers in an array and sets them to NULL.
//
#ifndef SAFE_DELETE_P_ARRAY
#define SAFE_DELETE_P_ARRAY(_p_p, _p_c)	\
do {										\
	if ((_p_p) != NULL)						\
	{										\
		for(int i=0;i<_p_c;i++)				\
			SAFE_DELETE(_p_p[i]);			\
	}										\
} while (0)
#endif

//////
// SAFE_DELETE_PP_ARRAY
//!		Safely frees the memory of pointers in an array and sets them to NULL.
//!	Also, safely frees the memory of an array and sets it to NULL.
//
#ifndef SAFE_DELETE_PP_ARRAY
#define SAFE_DELETE_PP_ARRAY(_p_p, _p_c)	\
do {										\
	SAFE_DELETE_P_ARRAY(_p_p,_p_c);		\
	SAFE_DELETE_ARRAY(_p_p);				\
} while (0)
#endif

//////
// SAFE_PTR
//!		For a safe use of a possibly NULL pointer.
//
#ifndef SAFE_PTR
#define SAFE_PTR(_p_p)	\
if (_p_p)					\
	(_p_p)
#endif
 
//////
// ARRAY_SIZE
//!		Calculates and returns a static array size.
//
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(p_array)					\
	(sizeof(p_array) / sizeof(p_array[0]))
#endif


///////////////////////////////////////////////////////////////////////////////
// Profiling Tools

/*****
 *	PROFILER_DEC
 *		Declare Commun variables needed for profiling
 *
 *	PROFILER_INIT
 *		Initiate Frequency
 *
 *	PROFILER_BEFORE
 *		Call before the code to test
 *
 *	PROFILER_AFTER
 *		Call after the code to test with a string for trace and true or false to force a cariage return
 *
 *
 *	ex. 
 *	void Foo1()
 *	{
 *		short toto=0;
 *		PROFILER_DEC
 *		PROFILER_INIT
 *	 
 *	 	PROFILER_BEFORE
 *	 	//code to benchmark
 *	 	//...
 *	 	PROFILER_AFTER("The code execute in : ", 1);
 *	}
 *
 * Note : Numbers shown are scale in nb ofTime freq wich is on all Pentium-based 0.8 us wich is 1250 times more accurate then timeGetTime
 */
//#ifdef _PROFILE
//	#define PROFILER_DEC															\
//		__int64 freq;																\
//		__int64 timePrev;															\
//		__int64 time;																\
//		char    buf[64];															\
//		char	m_string[1024];
//	#define PROFILER_INIT															\
//		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);							\
//		m_string[0]='\0';buf[0]='\0';
//	#define PROFILER_BEFORE															\
//		::QueryPerformanceCounter((LARGE_INTEGER*)&time);							\
//		timePrev= time;
//	#define PROFILER_AFTER(string, returnLine)										\
//		::QueryPerformanceCounter((LARGE_INTEGER*)&time);							\
//		sprintf(buf, "%s%f%s", string, (float)((int)(time-timePrev))*1000.0f/(float)freq, returnLine? "\r\n":"");\
//		BTRACE2(CHANNEL_USER, "%s%s", m_string, buf);
//#else
//	#define PROFILER_DEC						;
//	#define PROFILER_INIT						;
//	#define PROFILER_BEFORE						;
//	#define PROFILER_AFTER(string, returnLine)	;
//#endif

///////////////////////////////////////////////////////////////////////////////
// Maths

//////
// PI
//!		The value of PI.
//
#ifndef PI
#define PI			\
	3.141592653589793238462643383279502884197169399
#endif

//////
// SQUARE
//!		Computes the square (^2) of a value.
//
#ifndef SQUARE
#define SQUARE(_p_val)	\
	((_p_val) * (_p_val))
#endif

//////
// ABS
//!		Returns the absolute value of a given value
//
#ifndef ABS
#define ABS(_p_val)		\
	(_p_val > 0 ? (_p_val) : -(_p_val))
#endif

//////
// IS_ODD
//!      Indicates if a numeric value is odd the proper way
//
#ifndef IS_ODD
#define IS_ODD(p_nVal)						\
	(((unsigned char)p_nVal) & 1)
#endif

//////
// IS_EVEN
//!		Indicates if a numeric value is even.
//
#ifndef IS_EVEN
#define IS_EVEN(p_nVal)						\
	(!(((unsigned char)p_nVal) & 1))
#endif

//////
// NUM_SIGN
//!		If negative, returns -1; else, return 1.
//
#ifndef NUM_SIGN
#define NUM_SIGN(p_val)			\
	(((p_val) < 0) ? -1 : 1)
#endif

//////
// DEG_TO_RAD
//!		Converts an angle value from degree to radiant.
//
#ifndef DEG_TO_RAD
#define DEG_TO_RAD(p_ang)	\
	((p_ang) * PI / 180)
#endif

//////
// RAD_TO_DEG
//!		Converts an angle value from radiant to degree.
//
#ifndef RAD_TO_DEG
#define RAD_TO_DEG(p_rads)	\
	((p_rads) * 180 / PI)
#endif

//////
// RAND_RANGE
//!		Returns a random value included in a given range.
//
#ifndef RAND_RANGE
#define RAND_RANGE(p_min, p_max)					\
	((p_min) + (rand() % ((p_max) - (p_min) + 1)))
#endif

//////
// RAND_F
//!		Returns a random floating point value within [0,x].
//
#ifndef RAND_F
#define RAND_F(x)					\
	((rand() / (float) RAND_MAX) * (x) )
#endif

//////
// RAND_F_RANGE
//!		Returns a random floating point value included in a given range [p_min,p_max].
//
#ifndef RAND_F_RANGE
#define RAND_F_RANGE(p_min, p_max)					\
	( RAND_F((p_max)-(p_min)) + (p_min) )
#endif

///////////////////////////////////////////////////////////////////////////////
// Values

//////
// MIN
//!		Returns the minimum of two values.
//
#ifndef MIN
#define MIN(p_a, p_b)					\
	(((p_a) < (p_b)) ? (p_a) : (p_b))
#endif

//////
// MAX
//!		Returns the maximum of two values.
//
#ifndef MAX
#define MAX(p_a, p_b)					\
	(((p_a) > (p_b)) ? (p_a) : (p_b))
#endif

//////
// SWAP
//!		Swap two values using a temporary third value.
//
#ifndef SWAP
#define SWAP(p_a, p_b, p_temp)	\
	(p_temp)=	(p_a);				\
	(p_a)=		(p_b);				\
	(p_b)=		(p_temp);
#endif

//////
// XSWAP
//!		Swap two values WITHOUT using a temporary third value.
//
#ifndef XSWAP
#define XSWAP(p_a, p_b)			\
{									\
	if (p_a != p_b)					\
	{								\
		(p_a)^= (p_b);				\
		(p_b)^= (p_a);				\
		(p_a)^= (p_b);				\
	}								\
}
#endif

//////
// IS_DIGIT
//!		Indicates if the character is a valid decimal digit.
//
#ifndef IS_DIGIT
#define IS_DIGIT(p_cVal)					\
	((unsigned)CHAR_TO_DIGIT(p_cVal) <= 9)
#endif

//////
// IS_DIGIT_BIN
//!		Indicates if the character is a valid binary digit.
//
#ifndef IS_DIGIT_BIN
#define IS_DIGIT_BIN(p_cVal)				\
	((unsigned)CHAR_TO_DIGIT(p_cVal) <= 1)
#endif

//////
// IS_DIGIT_OCT
//!		Indicates if the character is a valid octal digit.
//
#ifndef IS_DIGIT_OCT
#define IS_DIGIT_OCT(p_cVal)				\
	((unsigned)CHAR_TO_DIGIT(p_cVal) <= 8)
#endif

//////
// IS_DIGIT_HEX
//!		Indicates if the character is a valid hexadecimal digit.
//
#ifndef IS_DIGIT_HEX
#define IS_DIGIT_HEX(p_cVal)					\
	(((unsigned)CHAR_TO_DIGIT(p_cVal) <= 8)	\
	 || ('a' <= (p_cVal) && (p_cVal) <= 'f')	\
	 || ('A' <= (p_cVal) && (p_cVal) <= 'F'))	
#endif

//////
// IS_STRING_EMPTY
//
#ifndef IS_STRING_EMPTY
#define IS_STRING_EMPTY(_p_szVal)	\
	('\0' == (_p_szVal[0]))
#endif

/////
// LERP
//!		Linearly interpolates a value between a,b with ratio r.
//
#ifndef LERP
#define LERP(a,b,r)	\
	((a) + ((b)-(a))*(r))
#endif


///////////////////////////////////////////////////////////////////////////////
// Display

//////
// RGB_16BITS_555
//!		Creates a 16-bits RGB value using the 5-5-5 scheme.
//
#ifndef RGB_16BITS_555
#define RGB_16BITS_555(p_r, p_g, p_b)							\
	(((p_b) & 31) + (((p_g) & 31) << 5) + (((p_r) & 31) << 10))
#endif

//////
// RGB_16BITS_565
//!		Creates a 16-bits RGB value using the 5-6-6 scheme.
//
#ifndef RGB_16BITS_565
#define RGB_16BITS_565(p_r, p_g, p_b)							\
	(((p_b) & 31) + (((p_g) & 63) << 6) + (((p_r) & 31) << 11))
#endif


///////////////////////////////////////////////////////////////////////////////
// Conversions

//////
// CHAR_TO_DIGIT
//!		Converts a character value to a digit.
//
#ifndef CHAR_TO_DIGIT
#define CHAR_TO_DIGIT(p_cVal)	\
	((p_cVal) - '0')
#endif

//////
// DIGIT_TO_CHAR
//!		Converts a numeric value to a char.
//
#ifndef DIGIT_TO_CHAR
#define DIGIT_TO_CHAR(p_nVal)	\
	((p_nVal) + '0')
#endif

//////
// ROUND
//!		Rounds a floating point value to the nearest integer.
//
#ifndef ROUND
#define ROUND( p_value )	\
	((int)((p_value)+0.5f))
#endif

//////
// CLAMP
//!		Clamps with min/max.
//
#ifndef CLAMP
#define CLAMP( p_value, p_min, p_max )	\
	(((p_value) < (p_min))? (p_min) : ((p_value) > (p_max))? (p_max) : (p_value))
#endif

//////
// WRAP
//!		Wraps around min/max.
//
#ifndef WRAP
#define WRAP( p_value, p_min, p_max )	\
	(((p_value) < (p_min))? ((p_value) - (p_min)) + (p_max) : ((p_value) > (p_max))? ((p_value) - (p_max)) + (p_min) : (p_value))
#endif


#endif	// _MACROS_H

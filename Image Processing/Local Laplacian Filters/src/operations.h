#pragma once

namespace Comp5900
{
	inline int Mod( int num, int modulus )
	{
		//true modulus (positive)
		return ( num % modulus + modulus ) % modulus;
	}

	inline int Round( float f )
	{
		return ( f > 0.0f ) ? floor( f + 0.5f) : ceil(f - 0.5f );
	}

	inline bool Equals( float f1, float f2 )
	{
		return (abs( f1 - f2) < 0.000001f );
	}

	inline float Sign( float f )
	{
		if( Equals(f,0.0f) ) return 0.0f;

		return ( f > 0.0f ) ? 1.0f : -1.0f;
	}

	inline float Saturate(float x)
	{
		return max(0.0f,min(1.0f,x));
	}

	inline float SmoothStep(float min, float max, float value)
	{
		//several smooth step functions are available
		//the paper implements x^4 - 4x^3 + 4x^2
		//we implement the following
		//6*x^5-15*x^4+10*x^3
		//from http://en.wikipedia.org/wiki/Smoothstep

		//scaled value
		value = Saturate((value-min)/(max-min));

		return 6*pow(value,5) - 15*pow(value,4) + 10*pow(value,3);

	}

};
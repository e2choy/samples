#include "stdafx.h"

BEGIN_NAMESPACE(Pups)
class Line;
END_NAMESPACE

BEGIN_NAMESPACE_2( Pups, Math )
//random number generated in [0,1]
float Random();

//square distance between two points
float SquareDistance( const Point& v0, const Point& v1 );
float Distance( const Point& v0, const Point& v1 );

//square distance between a line segment and a point
float SquareDistance( const Line& l, const Point& p );

//midpoint of two points
Point Midpoint( const Point& v0, const Point& v1 );

//clamp a value to within a range
float Clamp( float x, float a, float b );

//smoothstep functions
float Smoothstep( float x, float low = 0.0f, float high = 1.0f );
END_NAMESPACE_2
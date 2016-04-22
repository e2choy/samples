#include "stdafx.h"
#include "math.h"
#include "line.h"
#include <random>

static std::default_random_engine engine( 1 );
BEGIN_NAMESPACE_2(Pups,Math)
float Random(){
    std::uniform_real_distribution<float> dist;
    return dist( engine );
}

float SquareDistance( const Point& v0, const Point& v1 ){
    float ret = std::pow( v0.x - v1.x, 2 ) + std::pow( v0.y - v1.y, 2 );
    return ret;
}

float Distance( const Point& v0, const Point& v1 ){
    float ret = std::sqrt( SquareDistance( v0, v1 ) );
    return ret;
}

float SquareDistance( const Line& l, const Point& p ){
    Point v0 = l.Get( 0 );
    Point v1 = l.Get( 1 );
    Vec2f v = v0 - v1;
    float f = v.dot( v );

    //v0 == v1
    if ( f == 0.0f ) return SquareDistance( p, v0 );

    //consider the line extending the segment as v + t(w-v)
    const float t = (p - v0).dot( v1 - v0 ) / f;
    if( t < 0.0f ) 
        return SquareDistance( p, v0 );             //beyond the 'v1' end of the segment
    else if ( t > 1.0f ) 
        return SquareDistance ( p, v1 );            //beyond the 'v2' end of the segment

    //project onto the line segment
    Vec2f proj = v0 + t * ( v1 - v0 );
    return SquareDistance( p, proj );
}

Point Midpoint( const Point& v0, const Point& v1 ){
    Point p = v0 + v1;
    p *= 0.5f;
    return p;
}

float Clamp( float x, float a, float b ){
    return x < a ? a : ( x > b ? b : x);
}

float Smoothstep( float x, float low, float high ){
    // Scale, and clamp x to 0..1 range
    x = Clamp( (x - low) / (high - low), 0.0, 1.0 );

    // Evaluate polynomial
    x = x*x*x*(x*(x * 6 - 15) + 10);
    x = Clamp( x, 0.0f, 1.0f );
    return x;
}
END_NAMESPACE_2
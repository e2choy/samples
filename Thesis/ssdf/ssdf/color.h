#pragma once
#include "stdafx.h"
BEGIN_NAMESPACE(Pups)
class Color{
public:
    static Color WHITE;
    static Color BLACK;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color YELLOW;
    static Color CYAN;
    static Color PURPLE;
    static Color GRAY;
public:
    Color():r(0.0f),g(0.0f),b(0.0f),a(0.0f){
    }

    Color( float _b, float _g, float _r, float _a = 1.0f ):r(_r),g(_g),b(_b),a(_a){
    }

    Color( const Vec4f& v ){
        b = v[0];
        g = v[1];
        r = v[2];
        a = v[3];
    }

    Color( const Vec4b& p ){
        b =(float) p[0] / 255.0f;
        g =(float) p[1] / 255.0f;
        r =(float) p[2] / 255.0f;
        a =(float) p[3] / 255.0f;
    }

    Color( const Vec3b& p ){
        b =(float) p[0] / 255.0f;
        g =(float) p[1] / 255.0f;
        r =(float) p[2] / 255.0f;
        a =(float) 1.0f;
    }

    Color operator * (float K ) const{
        return Color( K*b, K*g, K*r, K*a );
    }

    Color& operator *=( float K ){
        *this = *this * K;
        return *this;
    }

    Color operator / (float K ) const{
        return Color( b/K, g/K, r/K, a/K );
    }

    Color& operator /= (float K ){
        r /= K;
        b /= K;
        g /= K;
        a /= K;
        return *this;
    }

    Color operator + ( const Color& rhs ) const{
        return Color( b + rhs.b, g + rhs.g, r + rhs.r, a + rhs.a );
    }

    Color& operator +=( const Color& rhs ){
        *this = *this + rhs;
        return *this;
    }

    bool operator == ( const Color& rhs ) const{
        if( r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a )
            return true;
        return false;
    }

    float B() const { return b; }
    float G() const { return g; }
    float R() const { return r; } 
    float A() const { return a; }

    operator cv::Scalar() const { return cv::Scalar( 255 * b, 255 * g, 255 * r, 255 * a ); }
    Vec4f ToVec4f() const {
        return Vec4f(b,g,r,a);
    }

    operator Vec4b() const { return cv::Vec4b( 255 * b, 255 * g, 255 * r, 255 * a ); }

    virtual ~Color(){}
private:
    float r;
    float g;
    float b;
    float a;
};

typedef std::vector<Color> Colors;
std::ostream& operator<<(std::ostream& oss, const Color& c );
END_NAMESPACE
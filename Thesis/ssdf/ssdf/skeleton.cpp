#include "stdafx.h"
#include "skeleton.h"
#include "math.h"
BEGIN_NAMESPACE(Pups)

Skeleton::Skeleton(){
}

void Skeleton::Fill( Mat& img, const RectI& roi ){
    float exp = 4.0f;
    float width = img.cols;
    float height = img.rows;

    //first zero out the roi
    PointI tl = roi.tl();
    PointI br = roi.br();
    for ( int y = tl.y; y < br.y; ++y ){
        for ( int x = tl.x; x < br.x; ++x ){
            img.at<float>( y, x ) = -1.0f;  //unknown distance
        }
    }

    //handle line segments//////////////////////////////////////
    //iterate through each line with the line roi
    for ( int i = 0; i < m_lines.size(); ++i ){
        RectI bound;        //bound based on image resolution
        Rect _bound;        //bound scaled to [0,1]

        if ( m_linesSup.size() ){
            _bound = MakeBound( m_lines[i], m_linesSup[i] );
        }
        else{
            _bound = Rect( 0.0, 0.0, 1.0f, 1.0f );
        }
        bound = RectI(
            _bound.x * width,
            _bound.y * height,
            _bound.width * width,
            _bound.height * height );
        bound = bound & roi;

        //fill the bound
        for ( int y = tl.y; y < br.y; ++y ){
            for ( int x = tl.x; x < br.x; ++x ){
                Point p( x / width, y / height );
                float d = SmoothDistanceLine( m_lines[i], p );
                if ( img.at<float>( y, x ) >= 0.0f ){
                    img.at<float>( y, x ) += std::powf( d, -exp );
                }
                else{
                    img.at<float>( y, x ) = std::powf( d, -exp );
                }
            }
        }
    }

    //handle points///////////////////////////////////////////////
    //iterate through each point with the point roi
    for ( int i = 0; i < m_pts.size(); ++i ){
        RectI bound;        //bound based on image resolution
        Rect _bound;        //bound scaled to [0,1]

        if ( m_ptsSup.size() ){
            _bound = MakeBound( m_pts[i], m_ptsSup[i] );
        }
        else{
            _bound = Rect( 0.0, 0.0, 1.0f, 1.0f );
        }
        bound = RectI(
            _bound.x * width,
            _bound.y * height,
            _bound.width * width,
            _bound.height * height );
        bound = bound & roi;

        //fill the bound for every point
        for ( int y = tl.y; y < br.y; ++y ){
            for ( int x = tl.x; x < br.x; ++x ){
                Point p( x / width, y / height );
                float d = SmoothDistancePoint( m_pts[i], p );
                if ( img.at<float>( y, x ) >= 0.0f ){
                    img.at<float>( y, x ) += std::powf( d, -exp );
                }
                else{
                    img.at<float>( y, x ) = std::powf( d, -exp );
                }
            }
        }
    }

    //find the final distance value
    //iterate throught the bound
    for ( int y = tl.y; y < br.y; ++y ){
        for ( int x = tl.x; x < br.x; ++x ){
            //1/d^n computed in img
            float val = img.at<float>( y, x );

            //unknown distance (far enough from other points)
            float at = 0.0f;
            if ( val < 0.0f ){
                at = -std::numeric_limits<float>::infinity();
            }
            else{
                at = std::powf( val, -1.0f / exp );
            }

            //check if we are at the zero set
            if ( !std::isfinite( at ) ){
                if ( at > 0.0f ){
                    img.at<float>( y, x ) = 0.0f;
                    continue;
                }
            }
            img.at<float>( y, x ) = at;
        }
    }
}

void Skeleton::Create(
    const Lines& lines,
    const Floats& linesSup,
    const Points& pts,
    const Floats& ptsSup
    ){
    m_lines = lines;
    m_linesSup = linesSup;
    m_pts = pts;
    m_ptsSup = ptsSup;
}

void Skeleton::Create( const Lines& lines, const Points& pts ){
    m_lines = lines;
    m_pts = pts;
}

//determine the bound of the skeleton
Rect Skeleton::MakeBound( const Line& line, float support ){
    //find the midpoint of the paper
    Point mid = Math::Midpoint( line.Get( 0 ), line.Get( 1 ) );
    Point tl = Point( mid.x - support, mid.y - support );
    Point br = Point( mid.x + support, mid.y + support );
    Rect bound( tl, br );
    bound &= Rect( 0, 0, 1.0f, 1.0f );
    return bound;
}

Rect Skeleton::MakeBound( const Point& pt, float support ){
    float r = support;
    float xm = pt.x;
    float ym = pt.y;
    Point tl;
    tl.x = std::max( (xm - r), 0.0f );
    tl.y = std::max( (ym - r), 0.0f );
    tl.x = std::min( tl.x, 1.0f );
    tl.y = std::min( tl.y, 1.0f );

    Point br;
    br.x = std::min( (xm + r), 1.0f );
    br.y = std::min( (ym + r), 1.0f );
    br.x = std::max( br.x, 0.0f );
    br.y = std::max( br.y, 0.0f );
    Rect bound = Rect( tl, br );
    return bound;
}

float Skeleton::SmoothDistanceLine( const Line& line, const Point& c ){
    float f = SignedDistanceInfiniteLine( line, c );
    float t = CircleTrimRegion( line, c );
    float h = f*f + std::powf( std::abs( t ) - t, 2.0f ) / 4.0f;
    h = std::sqrtf( h );
    return h;
}

float Skeleton::SignedDistanceInfiniteLine( const Line& line, const Point& c ){
    float x1 = line.Get( 0 ).x;
    float y1 = line.Get( 0 ).y;
    float x2 = line.Get( 1 ).x;
    float y2 = line.Get( 1 ).y;
    float x = c.x;
    float y = c.y;

    float f = (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1);
    f /= std::sqrt( std::powf( x2 - x1, 2 ) + std::powf( y2 - y1, 2 ) );
    return f;
}

float Skeleton::CircleTrimRegion( const Line& line, const Point& c ){
    Point v0 = line.Get( 0 );
    Point v1 = line.Get( 1 );

    Point m = Math::Midpoint( v0, v1 );
    float d = Math::Distance( v0, v1 );

    float x = c.x;
    float y = c.y;
    float t = (1 / d)*(std::powf( d / 2, 2 ) - std::powf( x - m.x, 2 ) - std::powf( y - m.y, 2));
    return t;
}

float Skeleton::SmoothDistancePoint( const Point& pt, const Point& c ){
    float d = Math::Distance( pt, c );
    return d;
}
END_NAMESPACE
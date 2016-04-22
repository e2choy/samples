#pragma once
#include "stdafx.h"
#include "line.h"
BEGIN_NAMESPACE(Pups)
class Skeleton{
public:
    Skeleton();

    //first parameter is matrix of floats, second parameter
    //is region of interest
    void Fill( Mat& img, const RectI& roi );

    //create with supports
    void Create( 
            const Lines& lines,
            const Floats& linesSup,
            const Points& pts,
            const Floats& ptsSup
        );
    void Create( const Lines& lines, const Points& pts );
    
private:

    //make the bound for the line segment
    Rect MakeBound( const Line& line, float support );
    Rect MakeBound( const Point& pt, float support );

    //smooth distance field of a line segment
    float SmoothDistanceLine( const Line& line, const Point& c );
    float SignedDistanceInfiniteLine( const Line& line, const Point& c );
    float CircleTrimRegion( const Line& line, const Point& c );

    //smooth distance from point
    float SmoothDistancePoint( const Point& pt, const Point& c );

    //line segments
    Lines   m_lines;        //line segments of the skeleton
    Floats  m_linesSup;     //support of the line segments

    //points
    Points  m_pts;
    Floats  m_ptsSup;
};
END_NAMESPACE
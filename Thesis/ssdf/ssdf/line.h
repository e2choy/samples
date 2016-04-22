#pragma once
#include "stdafx.h"

BEGIN_NAMESPACE( Pups )
class Line{
public:
    Line();
    Line( const Point& p0, const Point& p1 );
    const Point& Get( int index ) const;
private:
    Points m_pts;
};

typedef std::vector<Line> Lines;
END_NAMESPACE
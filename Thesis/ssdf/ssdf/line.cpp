#include "stdafx.h"
#include "line.h"

BEGIN_NAMESPACE(Pups)

Line::Line(){
    m_pts.resize( 2 );
}

Line::Line( const Point& p0, const Point& p1 ){
    m_pts.resize( 2 );
    m_pts[0] = p0;
    m_pts[1] = p1;
}

const Point& Line::Get( int index ) const{
    return m_pts[index];
}

END_NAMESPACE
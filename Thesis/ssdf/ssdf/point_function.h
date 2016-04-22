#ifndef POINT_FUNCTION
#define POINT_FUNCTION
#include "stdafx.h"

BEGIN_NAMESPACE_2(Pups,PointFunction)
///////////////////////////////////////////////////////////
class Base{
public:
    virtual ~Base(){}
    virtual void GeneratePoints( Points& pts ) = 0;
};
///////////////////////////////////////////////////////////
class PoissonDisk : public Base{
public:
    PoissonDisk( float dist );
    PoissonDisk( float dist, const Vec2f& min, const Vec2f& max );
    virtual ~PoissonDisk();
    virtual void GeneratePoints( Points& pts );
private:
    int		m_seed;
    float	m_dist;
    Vec2f	m_min;
    Vec2f	m_max;
};
///////////////////////////////////////////////////////////

END_NAMESPACE_2

#endif
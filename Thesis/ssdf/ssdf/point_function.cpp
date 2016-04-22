#include "stdafx.h"
#include "point_function.h"
#include "poisson_disk/bluenoise.h"

BEGIN_NAMESPACE_2(Pups,PointFunction)

PoissonDisk::PoissonDisk( float dist ):
m_dist(dist),
m_seed( 0 )
{
    m_min = Vec2f( -m_dist, -m_dist );
    m_max = Vec2f( 1.0f + m_dist, 1.0f + m_dist );
}

PoissonDisk::PoissonDisk( 
    float dist, 
    const Vec2f& min, 
    const Vec2f& max ):m_dist(dist),m_min(min),m_max(max),m_seed(0){
}

PoissonDisk::~PoissonDisk(){
}

void PoissonDisk::GeneratePoints( Points& pts ){
    ::PoissonDisk::Vec2f min = ::PoissonDisk::Vec2f( m_min[0], m_min[1] );
    ::PoissonDisk::Vec2f max = ::PoissonDisk::Vec2f( m_max[0], m_max[1] );
    std::vector<::PoissonDisk::Vec2f> samples;
    ::PoissonDisk::bluenoise_sample( m_dist, min, max, samples, m_seed );
    for ( auto& sample : samples ){
        pts.push_back( Point( sample[0], sample[1] ));
    }
}
END_NAMESPACE_2

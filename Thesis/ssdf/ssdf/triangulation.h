#pragma once
#include "stdafx.h"

BEGIN_NAMESPACE(Pups)
class Vertex;
typedef std::vector<Vertex*> Vertices;

class Triangulation{
public:
    Triangulation();
    void Create( Vertices& vs );

    //public variables
    CGAL::Dt			m_dt;
    CGAL::Vertices		m_cgvs;
    Vertices			m_vs;
};
END_NAMESPACE
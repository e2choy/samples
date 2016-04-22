#pragma once
#include "stdafx.h"
#include "triangulation.h"

BEGIN_NAMESPACE_2( Pups, PointFunction )
class Base;
END_NAMESPACE_2

BEGIN_NAMESPACE_2( Pups, LabelFunction )
class Base;
END_NAMESPACE_2


BEGIN_NAMESPACE(Pups)
class Vertex;
typedef std::vector<Vertex*> Vertices;

class Mesh{
public:
    Mesh();
    Mesh( PointFunction::Base& pf, LabelFunction::Base& lf );

    Vertices&		    GetVertices();
    const Vertices&		GetVertices() const;
    Triangulation&	GetTriangulation();
private:
    Vertices	  m_vs;
    Triangulation m_tri;
};
END_NAMESPACE

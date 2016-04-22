#pragma once
#include "stdafx.h"
BEGIN_NAMESPACE(Pups)
class Vertex;
class Triangle{
public:
    Triangle();
    Triangle( Vertex* v0, Vertex* v1, Vertex* v2 );
    Vertex* Get( int index );
private:
    std::vector<Vertex*> m_vs;
};
typedef std::vector<Triangle> Triangles;
END_NAMESPACE
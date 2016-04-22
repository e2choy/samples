#include "stdafx.h"
#include "triangle.h"

BEGIN_NAMESPACE(Pups)
Triangle::Triangle(){
}

Triangle::Triangle( Vertex* v0, Vertex* v1, Vertex* v2 ){
    m_vs = { v0, v1, v2 };
}

Vertex* Triangle::Get( int index ){
    return m_vs[index];
}
END_NAMESPACE

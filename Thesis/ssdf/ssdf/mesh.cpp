#include "stdafx.h"
#include "mesh.h"
#include "point_function.h"
#include "label_function.h"
#include "vertex.h"

BEGIN_NAMESPACE(Pups)
Mesh::Mesh(){
}

Mesh::Mesh( PointFunction::Base& pf, LabelFunction::Base& lf ){
    //generate points
    Points pts;
    pf.GeneratePoints( pts );

    //create the vertices
    m_vs.resize( pts.size() );
    for ( size_t i = 0; i < pts.size(); ++i ){
        Vertex* v = new Vertex( pts[i].x, pts[i].y );
        v->SetIndex( i );
        m_vs[i] = v;
    }

    //assign labels
    lf.AssignLabels( m_vs );

    //create the triangulation
    m_tri.Create( m_vs );
}

Vertices& Mesh::GetVertices(){
    return m_vs;
}

const Vertices& Mesh::GetVertices() const{
    return m_vs;
}

Triangulation&	Mesh::GetTriangulation(){
    return m_tri;
}
END_NAMESPACE

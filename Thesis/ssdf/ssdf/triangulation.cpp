#include "stdafx.h"
#include "triangulation.h"
#include "vertex.h"

using namespace CGAL;

BEGIN_NAMESPACE(Pups)
Triangulation::Triangulation(){
}

void Triangulation::Create( Vertices& vs ){
    m_vs = vs;
    m_dt.clear();
    m_cgvs.clear();

    //create the triangulation
    int i = 0;
    for ( Vertex* v : vs ){
        v->SetIndex( i );
        //add to cgal triangulation
        Dt::Point dtPt( v->X(), v->Y() );
        Dt::Vertex_handle h = m_dt.insert( dtPt );
        m_cgvs.push_back( h );

        h->info() = i;
        ++i;

        //clear the existing vertex neighbors
        v->ClearNeighbors();
    }

    //find the neighbors of the vertex
    for ( auto iter = m_dt.vertices_begin();
        iter != m_dt.vertices_end();
        ++iter ){
        auto circ = iter->incident_vertices();
        auto circ_end = circ;

        //iterate to assign neighbours
        do{
            if ( m_dt.is_infinite( circ ) )
                continue;
            m_vs[iter->info()]->AddNeighbor( m_vs[circ->info()] );
        } while ( ++circ != circ_end );
    }

}
END_NAMESPACE
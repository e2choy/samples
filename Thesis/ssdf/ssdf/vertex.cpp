#include "stdafx.h"
#include "vertex.h"

BEGIN_NAMESPACE(Pups)
Vertex::Vertex(){
}

Vertex::Vertex( float x, float y ) :m_pt( x, y ){

}

Vertex::Vertex( const Point& pt ) : m_pt( pt ){}

Vertex::~Vertex(){}


void Vertex::SetId( int id ){
    m_id = id;
}

int  Vertex::GetId() const{
    return m_id;
}

void Vertex::SetIndex( int index ){
    m_index = index;
}

int  Vertex::GetIndex() const{
    return m_index;
}

void Vertex::SetLabel( Label::Base* label ){
    m_label = label;
}

Label::Base* Vertex::GetLabel(){
    return m_label;
}

float Vertex::X() const{
    return m_pt.x;
}

float Vertex::Y() const{
    return m_pt.y;
}

void Vertex::SetX( float x ){
    m_pt.x = x;
}

void Vertex::SetY( float y ){
    m_pt.y = y;
}

Vertex::operator const Point&() const{ return m_pt; }

void Vertex::AddNeighbor( Vertex* v ){
    m_nghs.push_back( v );
}

void Vertex::ClearNeighbors(){
    m_nghs.clear();
}

const Vertices& Vertex::GetNeighbors() const{
    return m_nghs;
}

Vertices&		Vertex::GetNeighbors(){
    return m_nghs;
}

END_NAMESPACE
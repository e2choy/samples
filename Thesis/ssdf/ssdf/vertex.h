#ifndef VERTEX
#define VERTEX
#include "stdafx.h"

BEGIN_NAMESPACE_2(Pups,Label)
class Base;
END_NAMESPACE_2

BEGIN_NAMESPACE(Pups)
class Vertex;
typedef std::vector<Vertex*> Vertices;
typedef std::set<Vertex*> VertexSet;
typedef std::vector<Vertices> VerticesCollection;

class Vertex{
public:
    Vertex();
    Vertex( float x, float y );
    Vertex( const Point& pt );
    ~Vertex();

    //common
    void SetId( int id );
    int  GetId() const;

    void SetIndex( int index );
    int  GetIndex() const;

    void SetLabel( Label::Base* );
    Label::Base* GetLabel();

    //coordinates
    float X() const;
    float Y() const;
    void SetX( float x );
    void SetY( float y );
    operator const Point&() const;

    //neighbors
    void			AddNeighbor( Vertex* );
    void			ClearNeighbors();
    const Vertices& GetNeighbors() const;
    Vertices&		GetNeighbors();
private:
    int				m_id;		//id of label
    int				m_index;	//index of array
    Label::Base*	m_label;	//label
    Point			m_pt;		//location
    Vertices		m_nghs;		//neighbors
};

typedef std::pair<Vertex*, Vertex*> Edge;
typedef std::vector<Edge> Edges;
typedef std::set<Edge> EdgeSet;

END_NAMESPACE

#endif
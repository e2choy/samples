#pragma once
#include "stdafx.h"
#include "triangle.h"

BEGIN_NAMESPACE_2(Pups,Label)
class Base;
END_NAMESPACE_2

BEGIN_NAMESPACE(Pups)
class Vertex;
typedef std::vector<Vertex*> Vertices;
typedef std::pair<Vertex*, Vertex*> Edge;
typedef std::vector<Edge> Edges;
class Mesh;
class Meta;
class Skeleton;

class Region{
public:
    Region( const Vertices& vs, Mesh* mesh );
    
    void SetIndex( int index );
    int  GetIndex() const;

    void Fill( Meta& meta );

    //parameters of each region
    void SetBlendRadius( float r );
    void SetSupportK( float k );
private:
    void    ComputeBound();
    void    ComputeSignedArea( Triangles& triangles );
    void    CreateContours(
                Triangles& triangles,
                Skeleton& intContour,
                Skeleton& extContour );
    void    FindSupport(
        const Edges& edges,
        Floats& support );
    void    FindSupport(
        const Vertices& vs,
        Floats& support );

    void    ComputeDistances( 
                Skeleton& intContour, 
                Skeleton& extContour,
                const RectI& bound );
    void    ComputeWeights( const RectI& bound );
    float   ComputeWeight( int x, int y );

    Vertices	    m_vs;               //vertices the belong to the region
    Vertices        m_nghVs;            //vertices directly adjacent to region
    Mesh*		    m_mesh;             //mesh
    Rect            m_bound;            //bound in canonical coordinates
    int             m_id;               //same id of the underlying vertices
    int             m_index;            //index of the region
    Label::Base*    m_label;            //label

    Meta*           m_meta;             //global temporary variable
    float           m_blendRadius;      //blending radius
    float           m_supportK;         //support constant
};
typedef std::vector<Region*> Regions;
END_NAMESPACE
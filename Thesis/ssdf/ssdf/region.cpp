#include "stdafx.h"
#include "region.h"
#include "vertex.h"
#include "mesh.h"
#include "triangle.h"
#include "meta.h"
#include "graphics.h"
#include "skeleton.h"
#include "math.h"
#include "color.h"
#include "label.h"
using CGAL::Dt;

BEGIN_NAMESPACE( Pups )
Region::Region( const Vertices& vs, Mesh* mesh ) :
m_vs( vs ), 
m_mesh( mesh ), 
m_blendRadius(0.0f),
m_supportK(2.0f){
    //set the id and label of the underlying region
    assert( vs.size() );
    m_id = vs[0]->GetId();
    m_label = vs[0]->GetLabel();
}

void Region::SetIndex( int index ){
    m_index = index;
}

int  Region::GetIndex() const{
    return m_index;
}

void Region::Fill( Meta& meta ){
    //if no label skip
    if ( !m_label )
        return;

    m_meta = &meta;

    //data used in between steps
    Triangles triangles;

    ComputeBound();                     //find bounding box of region
    ComputeSignedArea( triangles );     //compute the signed area

    //define the contour
    Skeleton intContour;
    Skeleton extContour;
    CreateContours( triangles, intContour, extContour );        //create the signed contours

    //convert the bound into image coordinates
    float width = m_meta->m_width;
    float height = m_meta->m_height;
    RectI bound = RectI( 
        m_bound.x * width, 
        m_bound.y * height,
        m_bound.width * width, 
        m_bound.height * height );

    //use the contours to fill the bound
    ComputeDistances( intContour, extContour, bound );

    //compute the weights
    ComputeWeights( bound );
}

void Region::ComputeBound(){

    //all vertices and one ring
    Vertices vs;

    //iterate through the vertices and find neighbours
    for ( Vertex* v : m_vs ){
        //add itself
        if ( m_meta->m_vsIndices[v->GetIndex()] != m_index ){
            vs.push_back( v );
            m_meta->m_vsIndices[v->GetIndex()] = m_index;
        }

        //add neighbours
        for ( Vertex* ngh : v->GetNeighbors() ){
            if ( m_meta->m_vsIndices[ngh->GetIndex()] != m_index ){
                vs.push_back( ngh );
                m_meta->m_vsIndices[ngh->GetIndex()] = m_index;

                if ( ngh->GetId() != m_id )
                    m_nghVs.push_back( ngh );
            }
        }

    }

    //compute the bound
    {
        Rect bound;
        Point minPt = *(vs[0]);
        Point maxPt = *(vs[0]);
        for ( const Vertex* v : vs ){
            minPt.x = std::min( v->X(), minPt.x );
            minPt.y = std::min( v->Y(), minPt.y );
            maxPt.x = std::max( v->X(), maxPt.x );
            maxPt.y = std::max( v->Y(), maxPt.y );
        }
        minPt.x -= m_blendRadius;
        minPt.y -= m_blendRadius;
        maxPt.x += m_blendRadius;
        maxPt.y += m_blendRadius;
        minPt.x = Math::Clamp( minPt.x, 0, 1 );
        minPt.y = Math::Clamp( minPt.y, 0, 1 );
        maxPt.x = Math::Clamp( maxPt.x, 0, 1 );
        maxPt.y = Math::Clamp( maxPt.y, 0, 1 );
        bound = Rect( minPt, maxPt );
        m_bound = bound;
    }


}

void Region::ComputeSignedArea( Triangles& triangles ){
    //local compare function
    auto Compare = []( Vertex* v0, Vertex* v1 )->bool{
        if ( v0 >= v1 || v0->GetId() != v1->GetId() )
            return true;
        return false;
    };

    //find the triangles associated with the region
    {
        assert( m_mesh );
        Triangulation& tr = m_mesh->GetTriangulation();
        CGAL::Vertices& cgvs = tr.m_cgvs;
        CGAL::Dt& dt = tr.m_dt;
        Vertices& all = tr.m_vs;

        for ( Vertex* v : m_vs ){
            Dt::Vertex_handle vh = cgvs[v->GetIndex()];
            Dt::Face_circulator circ = dt.incident_faces( vh );
            Dt::Face_circulator end = circ;
            do{
                if ( dt.is_infinite( circ ) )
                    continue;
                //create the triangle
                Vertex* vs[3];
                for ( int i = 0; i < 3; ++i ){
                    int id = circ->vertex( i )->info();
                    vs[i] = all[id];
                }

                //check the triangle
                if ( Compare( v, vs[0] ) && Compare( v, vs[1] ) && Compare( v, vs[2] ) ){
                    triangles.push_back( Triangle( vs[0], vs[1], vs[2] ) );
                }
            } while ( end != ++circ );
        }
    }

    //compute the sign by iterating through the triangles
    for ( Triangle& tri : triangles ){
        Vertex* v0 = tri.Get( 0 );
        Vertex* v1 = tri.Get( 1 );
        Vertex* v2 = tri.Get( 2 );
        Points pts = { *v0, *v1, *v2 };
        //draw the sign field for the interior contour
        if ( v0->GetId() == m_id &&
            v1->GetId() == m_id &&
            v2->GetId() == m_id ){
            Graphics::FillConvexPoly( pts, m_index, m_meta->m_intMask );
        }

        //draw the sign field for the exterior contour
        Graphics::FillConvexPoly( pts, m_index, m_meta->m_extMask );

    }

    //for debugging
    //if ( m_index == 1 ){
    //    Graphics::DebugImage( "debug_intMask", m_index, m_meta->m_intMask );
    //    Graphics::DebugImage( "debug_extMask", m_index, m_meta->m_extMask );
    //}

}

void Region::CreateContours(
    Triangles& triangles,
    Skeleton& intContour,
    Skeleton& extContour ){

    //unmark all vertices
    for ( Vertex* v : m_vs ){
        m_meta->m_vsIndices[v->GetIndex()] = -1;
    }
    for ( Vertex* v : m_nghVs ){
        m_meta->m_vsIndices[v->GetIndex()] = -1;
    }

    Edges extEdges;
    EdgeSet intEdgeSet;
    Edges intEdges;

    //iterate through all faces
    for ( Triangle& triangle : triangles ){
        //count the number of vertices that match the index
        int count = 0;
        for ( int i = 0; i < 3; ++i ){
            if ( triangle.Get( i )->GetId() == m_id ){
                ++count;
            }
        }

        //figure out each edge
        for ( int i = 0; i < 3; ++i ){
            Vertex* v0 = triangle.Get( i );
            Vertex* v1 = triangle.Get( (i + 1) % 3 );

            //count==1 belong to exterior contour
            if ( count == 1 ){
                if ( v0->GetId() != m_id && v1->GetId() != m_id ){
                    //add to the exterior contour
                    extEdges.push_back( Edge( v0, v1 ) );

                    //mark vertices as covered
                    m_meta->m_vsIndices[v0->GetIndex()] = m_index;
                    m_meta->m_vsIndices[v1->GetIndex()] = m_index;
                }

            }

            //count==2 belong to the interior contour
            //can contain duplicate edges so skip
            if ( count == 2 ){
                //if edge belongs to the interior contour
                if ( v0->GetId() == m_id &&v1->GetId() == m_id ){
                    if ( v0 > v1 ){
                        std::swap( v0, v1 );
                    }
                    intEdgeSet.insert( Edge( v0, v1 ) );
                }
            }
        }//end for Triangle
    }//end for face

    //copy the interior edges
    intEdges.assign( intEdgeSet.begin(), intEdgeSet.end() );

    //iterate through the vertices to find orphan points
    Vertices intVs;
    Vertices extVs;
    Points intPoints;
    Points extPoints;

    if ( m_vs.size() == 1 ){
        intPoints.push_back( *m_vs[0] );
        intVs.push_back( m_vs[0] );
    }
    for ( Vertex* v : m_nghVs ){
        if ( m_meta->m_vsIndices[v->GetIndex()] == -1 ){
            extPoints.push_back( *v );
            extVs.push_back( v );
        }
    }

    //create lines from edges
    Lines extLines;
    Lines intLines;
    for ( auto& edge : extEdges ){
        extLines.push_back( Line( *edge.first, *edge.second ) );
    }
    for ( auto& edge : intEdges ){
        intLines.push_back( Line( *edge.first, *edge.second ) );
    }

    //find support
    Floats extEdgeSup;
    Floats extPtSup;
    Floats intEdgeSup;
    Floats intPtSup;
    FindSupport( extEdges, extEdgeSup );
    FindSupport( extVs, extPtSup );
    FindSupport( intEdges, intEdgeSup );
    FindSupport( intVs, intPtSup );

    //create the skeleton
    intContour.Create( intLines, intEdgeSup, intPoints, intPtSup );
    extContour.Create( extLines, extEdgeSup, extPoints ,extPtSup );
}

void Region::ComputeDistances( 
    Skeleton& intContour, 
    Skeleton& extContour,
    const RectI& bound ){

    //fill the interior and exterior distances
    intContour.Fill( m_meta->m_intContour, bound  );
    extContour.Fill( m_meta->m_extContour, bound );

    //compute the sign of the distances
    PointI tl = bound.tl();
    PointI br = bound.br();
    for ( int y = tl.y; y < br.y; ++y ){
        for ( int x = tl.x; x < br.x; ++x ){
            float   dI = m_meta->m_intContour.at<float>( y, x );
            float   dE = m_meta->m_extContour.at<float>( y, x );

            if ( m_meta->m_intMask.at<int>( y, x ) == m_index ){
                dI *= -1;
                m_meta->m_intContour.at<float>( y, x ) = dI;
            }
            else if ( m_meta->m_extMask.at<int>( y, x ) != m_index ){
                dE *= -1;
                m_meta->m_extContour.at<float>( y, x ) = dE;
            }
        }
    }

    //check the index
    //if ( m_index == 1 ){
    //    cv::imshow( "index1 intContour", m_meta->m_intContour );
    //    cv::imshow( "index1 extContour", m_meta->m_extContour );
    //}
}

void Region::ComputeWeights( const RectI& bound ){
    PointI tl = bound.tl();
    PointI br = bound.br();
    float width = m_meta->m_width;
    float height = m_meta->m_height;
    for ( int y = tl.y; y < br.y; ++y ){
        for ( int x = tl.x; x < br.x; ++x ){
            float weight = ComputeWeight( x, y );

            //add it to the color matrix
            if ( weight > 0 ){
                //compute color
                Color color;
                color = m_label->GetColor( x/width, y/height, weight );

                //add to total
                m_meta->m_totWeights.at<float>( y, x ) += weight;
                m_meta->m_totColors.at<Vec4f>( y, x ) += (color*weight).ToVec4f();
            }
        }
    }

    cv::imshow( "weights", m_meta->m_weights );
}

float  Region::ComputeWeight( int x, int y ){
    Mat&    intContour = m_meta->m_intContour;
    Mat&    extContour = m_meta->m_extContour;
    float   dI = intContour.at<float>( y, x );
    float   dE = extContour.at<float>( y, x );
    float   ratio = dI / (dI + dE);
    float   distE = -dE / 2.0f;
    float   distI = dI / 2.0f;
    //compute the distance to the midway contour
    float dist = Math::Smoothstep( ratio )*distI + Math::Smoothstep( 1 - ratio )*distE;
    m_meta->m_midContour.at<float>( y, x ) = dist;

    //compute it as a weight
    float blendRadius = m_blendRadius;
    float weight = 0.0f;
    if ( blendRadius > 0.0f ){
        float val = ((dist / blendRadius) + 1.0f) / 2.0f;
        weight = 1.0f - Math::Smoothstep( val );
        m_meta->m_weights.at<float>( y, x ) = weight;
    }
    else{
        weight = (dist > 0.0f) ? 0.0f : 1.0f;
        m_meta->m_weights.at<float>( y, x ) = weight;
    }
    return weight;
}

void Region::FindSupport(
    const Edges& edges,
    Floats& supports ){
    //find support for the set of edges
    supports.clear();
    for ( const Edge& e : edges ){
        float length = 0.0f;
        const Vertex* v0 = e.first;
        const Vertex* v1 = e.second;

        for ( const Vertex* ngh : v0->GetNeighbors() ){
            length = std::max( length, Math::SquareDistance( *v0, *ngh ) );
        }
        for ( const Vertex* ngh : v1->GetNeighbors() ){
            length = std::max( length, Math::SquareDistance( *v1, *ngh ) );
        }
        supports.push_back( std::sqrtf( length ) * m_supportK );
    }
}

void Region::FindSupport(
    const Vertices& vs,
    Floats& supports ){
    //find support for the set of points
    supports.clear();
    for ( const Vertex* v : vs ){
        float length = 0.0f;
        for ( const Vertex* ngh : v->GetNeighbors() ){
            length = std::max( length, Math::SquareDistance( *v, *ngh ) );
            supports.push_back( std::sqrtf( length )*m_supportK );
        }
    }
}

void Region::SetBlendRadius( float r ){
    m_blendRadius = r;
}

void Region::SetSupportK( float k ){
    m_supportK = k;
}
END_NAMESPACE
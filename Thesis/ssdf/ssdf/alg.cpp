#include "stdafx.h"
#include "alg.h"
#include "color.h"
#include "vertex.h"
#include "mesh.h"
#include "meta.h"
#include "region.h"
#include "graphics.h"

//boost graph
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

BEGIN_NAMESPACE(Pups)

Alg::Alg(){
}

Alg::Alg( int width, int height ):m_width(width),m_height(height){
}

void Alg::SetSize( int width, int height ){
    m_width = width;
    m_height = height;
}

int Alg::GetWidth() const{
    return m_width;
}

int Alg::GetHeight() const{
    return m_height;
}

void Alg::Run( Mesh& mesh ){
    //create the meta structure
    Meta meta( m_width, m_height, mesh.GetVertices().size() );

    //process the mesh
    CreateRegions( mesh );
    ProcessRegions( meta );
    ProcessImage( meta, mesh );
}

Mat& Alg::GetImage(){
    return m_img;
}

void Alg::CreateRegions( Mesh& mesh ){
    Vertices& vs = mesh.GetVertices();

    //build boost graph
    Graph g;
    for ( Vertex* v : mesh.GetVertices() ){
        //add self edge
        boost::add_edge( v->GetIndex(), v->GetIndex(), g );

        //add the neighbors
        for ( Vertex* ngh : v->GetNeighbors() ){
            if ( v->GetId() == ngh->GetId() &&
                v->GetIndex() > ngh->GetIndex() ){
                boost::add_edge( v->GetIndex(), ngh->GetIndex(), g );
            }
        }
    }


    //find the connected components
    VerticesCollection ccs;
    Ints comps( vs.size() );	//each index of comps is id of connected component
    int numComps = boost::connected_components( g, comps.data() );
    ccs.resize( numComps );
    for ( size_t i = 0; i < vs.size(); ++i ){
        ccs[comps[i]].push_back( vs[i] );
    }

    //create a region for each connected components
    m_regions.clear();
    for ( size_t i = 0; i < ccs.size(); ++i ){
        Region* r = new Region( ccs[i], &mesh );
        r->SetIndex( i );
        m_regions.push_back( r );

        //set the parameters of the region
        r->SetBlendRadius( m_blendRadius );
        r->SetSupportK( m_supportK );
    }

    //print number of regions created
    cout << m_regions.size() << " regions created." << endl;
}

void Alg::ProcessRegions( Meta& meta ){
    //iterate through each region
    for ( Region* region : m_regions ){
        region->Fill( meta );
    }
}

void Alg::ProcessImage( const Meta& meta, Mesh& mesh ){
    m_img = Mat( 400, 400, CV_8UC4, Color::CYAN );

    //iterate through the image
    for ( int y = 0; y < m_height; ++y ){
        for ( int x = 0; x < m_width; ++x ){
            Vec4f totCol = meta.m_totColors.at<Vec4f>( y, x );
            float totWeight = meta.m_totWeights.at<float>( y, x );

            Color c;
            if ( totWeight > 0.0f ){
                c = Color( totCol / totWeight );
            }
            else{
                c = Color::CYAN;
            }
            m_img.at<Vec4b>( y, x ) = c;
        }
    }
    //merge weights and colors to create texture
    //Graphics::DrawMesh( mesh, Color::BLUE, m_img );
}

void Alg::SetBlendRadius( float r ){
    m_blendRadius = r;
}

void Alg::SetSupportK( float K ){
    m_supportK = K;
}
END_NAMESPACE
#include "stdafx.h"
#include "graphics.h"
#include "vertex.h"
#include "mesh.h"
#include "color.h"
#include "label.h"

BEGIN_NAMESPACE_2( Pups, Graphics )

void DrawMesh( const Mesh& mesh, const Color& color, Mat& img ){
    const int THICKNESS = 1;
    const Vertices& vs = mesh.GetVertices();
    int width = img.cols;
    int height = img.rows;
    
    //draw the graph edges
    for ( Vertex* v : vs ){
        Label::Base* l = v->GetLabel();
        Point pt( v->X() * width, v->Y() * height );

        //draw the outgoing edges
        for ( Vertex* ngh : v->GetNeighbors() ){
            Point nghPt( ngh->X()*width, ngh->Y()*height );
            cv::line( img, pt, nghPt, color, THICKNESS );
        }
    }

    //draw the vertices
    const int RADIUS = 4;
    for ( Vertex* v : vs ){
        Point pt( v->X()* width, v->Y()*height );
        Label::Base* l = v->GetLabel();

        //draw the label color
        if ( l ){
            Color vtxCol = l->GetColor( v->X(), v->Y(), 1.0f );
            cv::circle( img, pt, RADIUS, vtxCol, -1 );
        }
       
        //draw the vertex outline
        cv::circle( img, pt, RADIUS, color, THICKNESS );

        //draw the vertex index
        cv::putText( img,
            std::to_string( v->GetIndex() ),
            pt,
            cv::FONT_HERSHEY_SIMPLEX,
            0.4f,
            Color::BLACK );
    }
}

void FillConvexPoly( const Points& _pts, int val, Mat& img ){
    PointsI pts;
    float width = img.cols;
    float height = img.rows;
    for ( const Point& _pt : _pts ){
        PointI pt( _pt.x * width, _pt.y * height );
        pts.push_back( pt );
    }
    cv::fillConvexPoly( img, pts, val );
}

void DebugImage( const string& str, int index, const Mat& img ){
    //assume img stores integers
    Mat debugImg( img.rows, img.cols, CV_8UC4, Color::BLACK );
    for ( int y = 0; y < img.rows; ++y ){
        for ( int x = 0; x < img.cols; ++x ){
            int val = img.at<int>( y, x );
            if ( val == index ){
                debugImg.at<Vec4b>( y, x ) = Color::RED;
            }
        }
    }

    //show the debug image
    cv::imshow( str + std::to_string( index ), debugImg );
}
END_NAMESPACE_2
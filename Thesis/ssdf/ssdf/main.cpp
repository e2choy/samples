#include "stdafx.h"
#include "mesh.h"
#include "alg.h"
#include "point_function.h"
#include "label_function.h"
#include "label.h"
#include "vertex.h"
#include "tests.h"

using namespace Pups;

int main( int argc, const char* argv[] ){
    //create a randomly labeled mesh
    Label::Simple redLabel( Color::RED );
    Label::Simple blueLabel( Color::BLUE );
    PointFunction::PoissonDisk pf( 0.15f );
    LabelFunction::RandomBinary lf( &redLabel, &blueLabel );
    //LabelFunction::Solid lf( &redLabel );
    Mesh mesh( pf, lf );

    //Vertices& vs = mesh.GetVertices();
    //int indices[] = { 16, 17, 18 };
    //for ( int i : indices ){
    //    vs[i]->SetId( 1 );
    //    vs[i]->SetLabel( &blueLabel );
    //}

    //run the algorithm
    Alg alg( 400, 400 );
    alg.SetBlendRadius( 0.05 );
    alg.Run( mesh );
    Mat img = alg.GetImage();
    cv::imshow( "img", img );
    cv::waitKey();
    return EXIT_SUCCESS;
}
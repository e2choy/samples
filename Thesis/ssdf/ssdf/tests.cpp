#include "stdafx.h"
#include "skeleton.h"

BEGIN_NAMESPACE(Pups)
void RunTestSkeleton(){
    Mat img( 400, 400, CV_32F, 0.0f );

    //create the upsidedown letter 'U'
    Lines lines;
    lines.push_back( Line( Point( 0.2, 0.2 ), Point( 0.8, 0.2 ) ) );
    lines.push_back( Line( Point( 0.8, 0.2 ), Point( 0.8, 0.8 ) ) );
    lines.push_back( Line( Point( 0.8, 0.8 ), Point( 0.6, 0.8 ) ) );
    lines.push_back( Line( Point( 0.6, 0.8 ), Point( 0.6, 0.4 ) ) );
    lines.push_back( Line( Point( 0.6, 0.4 ), Point( 0.4, 0.4 ) ) );
    lines.push_back( Line( Point( 0.4, 0.4 ), Point( 0.4, 0.8 ) ) );
    lines.push_back( Line( Point( 0.4, 0.8 ), Point( 0.2, 0.8 ) ) );
    lines.push_back( Line( Point( 0.2, 0.8 ), Point( 0.2, 0.2 ) ) );
    Points pts;

    //create the skeleton
    Skeleton s;

    //fill the bound
    s.Create( lines, pts );
    RectI roi( 0, 0, 400, 400 );
    s.Fill( img, roi );
   
    cv::imshow( "skeleton_test", img );
    cv::waitKey();
}
END_NAMESPACE

#include "stdafx.h"
#include "meta.h"

BEGIN_NAMESPACE(Pups)
Meta::Meta( int width, int height, int numVertices ) :
m_width( width ), m_height( height ), m_numVertices(numVertices){

    m_totWeights = Mat( m_height, m_width, CV_32F, 0.0f );  //total weight of all regions
    m_totColors = Mat( m_height, m_width, CV_32FC4,         //total colors of all regions
        cv::Scalar( 0.0f, 0.0f, 0.0f, 0.0f ) );

    //masks for the sign of the interior and exterior fields
    m_intMask = Mat( m_height, m_width, CV_32S, -1 );
    m_extMask = Mat( m_height, m_width, CV_32S, -1 );

    //field values
    m_intContour = 
        Mat( m_height, m_width, CV_32F, 0.0f );    //distance value from interior contour

    m_extContour 
        = Mat( m_height, m_width, CV_32F, 0.0f );  //distance value from exterior contour

    m_midContour 
        = Mat( m_height, m_width, CV_32F, 0.0f );  //distance value from midway contour

    m_weights = Mat( m_height, m_width, CV_32F, 0.0f );

    m_vsIndices = Ints( m_numVertices, -1 );
}
END_NAMESPACE
#pragma once
#include "stdafx.h"

BEGIN_NAMESPACE(Pups)

//meta data to calculate texture image
class Meta{
public:
    Meta( int width, int height, int numVertices );

    //global region data
    Mat					m_totWeights;		//weights of the texture
    Mat                 m_totColors;        //total colors

    //vertex mapping
    int                 m_numVertices;
    Ints                m_vsIndices;        //index mapping

    //mask to determine the sign of the field
    Mat                 m_intMask;          //mask for interior contour
    Mat                 m_extMask;          //mask for exterior contour

    //local for each region
    Mat m_intContour;		//distance to the interior contour
    Mat m_extContour;		//distance to the exterior contour
    Mat m_midContour;		//distance to the midway contour
    Mat m_weights;			//weights of each region
    int m_width;			//width of image
    int m_height;			//height of image
};
END_NAMESPACE
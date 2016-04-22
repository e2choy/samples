#pragma once
#include "stdafx.h"

BEGIN_NAMESPACE(Pups)
class Color;
class Mesh;
class Region;
class Meta;
typedef std::vector<Region*> Regions;

class Alg{
public:
    Alg();
    Alg( int width, int height );
    void SetSize( int width, int height );
    int GetWidth() const;
    int GetHeight() const;

    void Run( Mesh& mesh );
    Mat& GetImage();

    //set blending radius
    void SetBlendRadius( float r );

    //set the support of edge line segment of the region
    void SetSupportK( float K );
private:
    //the different steps of our algorithm
    void CreateRegions( Mesh& mesh );
    void ProcessRegions( Meta& meta );
    void ProcessImage( const Meta& meta, Mesh& mesh  );

    Mat					m_img;				//final texture image
    int					m_width;
    int					m_height;
    Regions				m_regions;

    //parameters for each region
    float               m_blendRadius;
    float               m_supportK;
};
END_NAMESPACE

#include "stdafx.h"
#include "graphics.h"
#include "matrix.h"

#include <random>

namespace Comp5900
{
	void ToOfImage	( const Image&	src,	ofImage& dest )
	{
		dest.allocate( src.Cols(), src.Rows(), OF_IMAGE_COLOR );
		for( int y = 0; y < src.Rows(); ++y )
		{
			for( int x = 0; x < src.Cols(); ++x )
			{
				for( int i = 0; i < RGB_BPP; ++i )
				{
					dest.getPixelsRef()[(y * dest.width + x) * 3 + i]	= static_cast<unsigned char>( src[y][x][i] );
				}
			}
		}
		dest.reloadTexture();
	}

	void FromOfImage( const ofImage& _src,	Image& dest )
	{
		ofImage& src = const_cast<ofImage&>( _src );
		dest.Resize( src.height, src.width );
		for( int y = 0; y < src.height; ++y )
		{
			for( int x = 0; x < src.width; ++x )
			{
				for( int i = 0; i < RGB_BPP; ++i )
				{
					dest[y][x][i] = static_cast<int>( src.getPixelsRef()[(y * src.width + x) * 3 + i] );
				}
			}
		}
	}

	void SaveImage( const ofImage& _image, const std::string& filename )
	{
		ofImage& image = const_cast<ofImage&>( _image );
		if( image.type  == OF_IMAGE_COLOR )
		{
			image.saveImage( filename, OF_IMAGE_QUALITY_BEST );
		}
		else if( image.type == OF_IMAGE_GRAYSCALE )
		{
			ofImage tmp;
			ToRgb( image, tmp );
			tmp.saveImage( filename );
		}
	}

	void ToRgb( const ofImage& _src, ofImage& dest )
	{
		ofImage& src = const_cast<ofImage&>( _src );
		ofImage temp;
		temp.allocate( src.width, src.height, OF_IMAGE_COLOR );
		for( int y = 0; y < src.height; ++y )
		{
			for( int x = 0; x < src.width; ++x )
			{
				for( int i = 0; i < RGB_BPP; ++i )
				{
					temp.getPixelsRef()[(y * temp.width + x) * 3 + i] = src.getPixelsRef()[(y * temp.width + x)];
				}
			}
		}
		temp.reloadTexture();
		dest = temp;
	}

	void SaveMatrix( const Matrix& matrix, const std::string& filename )
	{
		ofImage tmp;
		ToOfImage( matrix, tmp );
		SaveImage( tmp, filename );
	}

	//add low magnitude uniform noise
	void Noise( const MatrixF& src, float magnitude, MatrixF& dest )
	{
		dest.Resize( src.Rows(), src.Cols() );

		std::default_random_engine generator( rand() );
		std::uniform_real_distribution<float>			distribution( -1.0f * magnitude /2.0f, magnitude / 2.0f );

		for( int i = 0; i < src.Rows(); ++i )
		{
			for( int j = 0; j < src.Cols(); ++j )
			{
				dest[i][j] = src[i][j] + distribution( generator );
			}
		}

	}
}




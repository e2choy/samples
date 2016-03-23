#pragma once

#include "matrix.h"
#include "element.h"

namespace Comp5900
{
	//constants
	const int RGB_BPP	= 3;
	const int RGBA_BPP	= 4;

	//channel
	enum CHANNEL
	{
		RED,
		GREEN,
		BLUE,
		ALPHA,
	};

	//clamp function
	inline float Clamp( float v )
	{
		if( v < 0.0f ) return 0.0f;
		if( v > 255.0f ) return 255.0f;
		return v;
	}

	template<class T>
	void Clamp( _Matrix<T>& src )
	{
		for( int i = 0; i < src.Rows(); ++i )
		{
			for( int j = 0; j < src.Cols(); ++j )
			{
				src[i][j] = Clamp( src[i][j] );
			}
		}
	}

	//image
	typedef _Matrix<Element3>				Image;
	typedef _Matrix<Element3F>				ImageF;

	typedef std::vector<Image>				Images;
	typedef std::vector<ImageF>				ImagesF;

	//convert to image
	void ToOfImage	( const Image&	src,	ofImage& dest );
	void FromOfImage( const ofImage& src,	Image& dest );

	template<class T>
	void ToOfImage	( const _Matrix<T>&	src,	ofImage& dest )
	{
		dest.allocate( src.Cols(), src.Rows(), OF_IMAGE_GRAYSCALE );
		for( int i = 0; i < src.Rows(); ++i )
		{
			for( int j = 0; j < src.Cols(); ++j )
			{
				dest.getPixelsRef()[i * dest.width + j] = static_cast<unsigned char>( src[i][j] );
			}
		}
		dest.reloadTexture();
	}

	template<class T>
	void ToOfImage	( const _Matrix<T>&	r,const _Matrix<T>&	g,const _Matrix<T>&	b,	ofImage& dest )
	{
		assert( r.Rows() == g.Rows());
		assert( g.Rows() == b.Rows());

		assert( r.Cols() == g.Cols());
		assert( g.Cols() == b.Cols());

		dest.allocate( r.Cols(), r.Rows(), OF_IMAGE_COLOR );
		for( int y = 0; y < r.Rows(); ++y )
		{
			for( int x = 0; x < r.Cols(); ++x )
			{
				dest.getPixelsRef()[(y * dest.width + x) * 3 + 0]	= static_cast<unsigned char>( r[y][x] );
				dest.getPixelsRef()[(y * dest.width + x) * 3 + 1]	= static_cast<unsigned char>( g[y][x] );
				dest.getPixelsRef()[(y * dest.width + x) * 3 + 2]	= static_cast<unsigned char>( b[y][x] );
			}
		}
		dest.reloadTexture();
	}

	template<class T>
	void FromOfImage( const ofImage& _src,	_Matrix<T>& dest )
	{
		ofImage& src = const_cast<ofImage&>( _src );
		ofImage* grayImage = &src;
		ofImage temp;
		if( src.type != OF_IMAGE_GRAYSCALE )
		{
			temp = src;
			temp.setImageType( OF_IMAGE_GRAYSCALE );
			grayImage = &temp;
		}
		
		dest.Resize( src.height, src.width );
		for( int y = 0; y < src.height; ++y )
		{
			for( int x = 0; x < src.width; ++x )
			{
				dest[y][x]= static_cast<int>( grayImage->getPixelsRef()[(y * src.width + x)] );
			}
		}
	}

	template<class T>
	void FromOfImage( const ofImage& _src,	_Matrix<T>& r, _Matrix<T>& g, _Matrix<T>& b )
	{
		ofImage& src = const_cast<ofImage&>( _src );
		int bpp = 3;
		if( src.type  == OF_IMAGE_COLOR )
			bpp = 3;
		if( src.type  == OF_IMAGE_COLOR_ALPHA )
			bpp = 4;
		r.Resize( src.height, src.width );
		g.Resize( src.height, src.width );
		b.Resize( src.height, src.width );
		unsigned char* chars = src.getPixels();

		for( int y = 0; y < src.height; ++y )
		{
			for( int x = 0; x < src.width; ++x )
			{
				r[y][x] = (unsigned char) chars[(y * src.width + x)*bpp + 0];
				g[y][x] = (unsigned char) chars[(y * src.width + x)*bpp + 1];
				b[y][x] = (unsigned char) chars[(y * src.width + x)*bpp + 2];	
			}
		}
	}

	template<class T>
	void FromOfImageHSV( const ofImage& _src, _Matrix<T>& h, _Matrix<T>& s, _Matrix<T>& v )
	{
		ofImage& src = const_cast<ofImage&>( _src );
		int bpp = 3;
		if( src.type  == OF_IMAGE_COLOR )
			bpp = 3;
		if( src.type  == OF_IMAGE_COLOR_ALPHA )
			bpp = 4;
		h.Resize( src.height, src.width );
		s.Resize( src.height, src.width );
		v.Resize( src.height, src.width );
		unsigned char* chars = src.getPixels();

		for( int y = 0; y < src.height; ++y )
		{
			for( int x = 0; x < src.width; ++x )
			{
				unsigned char r = (unsigned char) chars[(y * src.width + x)*bpp + 0];
				unsigned char g = (unsigned char) chars[(y * src.width + x)*bpp + 1];
				unsigned char b = (unsigned char) chars[(y * src.width + x)*bpp + 2];	

				ofColor c(r,g,b);
				h[y][x] = c.getHue();
				s[y][x] = c.getSaturation();
				v[y][x] = c.getBrightness();
			}
		}
	}

	template<class T>
	void ToOfImageHSV	( const _Matrix<T>&	h,const _Matrix<T>&	s,const _Matrix<T>&	v,	ofImage& dest )
	{
		assert( h.Rows() == s.Rows());
		assert( s.Rows() == v.Rows());

		assert( h.Cols() == s.Cols());
		assert( s.Cols() == v.Cols());

		dest.allocate( h.Cols(), h.Rows(), OF_IMAGE_COLOR );
		for( int y = 0; y < h.Rows(); ++y )
		{
			for( int x = 0; x < h.Cols(); ++x )
			{
				ofColor c = ofColor::fromHsb( h[y][x], s[y][x], v[y][x] );
				dest.getPixelsRef()[(y * dest.width + x) * 3 + 0]	= c.r;
				dest.getPixelsRef()[(y * dest.width + x) * 3 + 1]	= c.g;
				dest.getPixelsRef()[(y * dest.width + x) * 3 + 2]	= c.b;
			}
		}
		dest.reloadTexture();
	}

	//save (fix for openframeworks bug)
	void SaveImage( const ofImage& image, const std::string& filename );
	void ToRgb( const ofImage& _src, ofImage& dest );

	//save matrix
	void SaveMatrix( const Matrix& matrix, const std::string& filename );

	//add low magnitude uniform noise
	void Noise( const MatrixF& src, float magnitude, MatrixF& dest );

};
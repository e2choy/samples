#include "stdafx.h"
#include "app.h"
#include "matrix.h"
#include "element.h"

#include <random>
#include <sstream>

#include "pyramid.h"
#include "local_laplacian.h"

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;


namespace Comp5900
{
	using namespace Pyramid;

	void App::setup()
	{
		m_image.loadImage("lenna.png");
		m_churchImage.loadImage("church.png");
		m_kittenImage.loadImage("kitten.png");
		m_flowerImage.loadImage("butterfly.png");
		m_current.loadImage("lenna.png");
	}

	void App::update()
	{
	}

	void App::draw()
	{
		if( m_current.bAllocated() )
		{
			int width = ofGetWidth();
			int height = ofGetHeight();
			m_current.draw(0,0,width,height);
		}
	}

	void App::keyPressed(int key)
	{
		switch(key)
		{
		case '0':
			//show the original grayscale image
			{
				m_current.loadImage("lenna.png");
				m_current.setImageType( OF_IMAGE_GRAYSCALE );
			}
			break;
		case '1':
			ShowStage1();
			break;
		case '2':
			ShowStage2();
			break;
		case '3':
			ShowStage3();
			break;
		case '4':
			ShowStage4Before();
			ShowStage4After();
			break;
		case '5':
			ShowStage5a();
			ShowStage5b();
			ShowStage5c();
			break;
		case '6':
			ShowStage6a();
			ShowStage6b();
			break;
		case '7':
			ShowStage7();
			break;
		default:
			break;
		};
	}

	void		App::ShowStage1()
	{
		//load the image
		MatrixF image;
		FromOfImage( m_image, image );

		//create the pyramid
		MatrixFArray pyramid;
		GaussianPyramid( image, pyramid );

		//save the pyramid
		for( int i = 0; i < pyramid.size(); ++i )
		{
			ofImage temp;
			ToOfImage( pyramid[i], temp );
			
			ostringstream oss;
			oss << "01-" << i << "gaussian_pyramid.png" ;
			SaveImage( temp, oss.str() );
		}

		ToOfImage( image, m_current );

		//save the laplacian pyramid
		MatrixFArray lpyramid;
		LaplacianPyramid( image, lpyramid, 0.40f );
		//save the pyramid
		for( int i = 0; i < lpyramid.size(); ++i )
		{
			ofImage temp;
			Absolute(lpyramid[i]);
			Clamp(lpyramid[i]);
			ToOfImage( lpyramid[i], temp );
			
			ostringstream oss;
			oss << "01-" << i << "laplacian_pyramid.png" ;
			SaveImage( temp, oss.str() );
		}
	}

	void		App::ShowStage2()
	{
		//load the image
		MatrixF image;
		FromOfImage( m_image, image );

		//create the pyramid
		MatrixFArray pyramid;
		LaplacianPyramid( image, pyramid, 0.40f, 4 );

		//now collapse the pyramid back to image
		CollapseLaplacianPyramid( pyramid, image );
		SaveMatrix( image, "02-collapsed_pyramid.png" );
		cout << "number of levels " << pyramid.size() << endl;

		ToOfImage( image, m_current );
	}


	void		App::ShowStage3()
	{
		//trivial sharpening
		MatrixF image;
		FromOfImage( m_kittenImage, image );
		SaveMatrix(image, "03-kitten.png");

		//create the pyramid
		MatrixFArray pyramid;
		LaplacianPyramid( image, pyramid );

		//double the values
		MatrixFArray pyramidtemp = pyramid;
		Multiply( pyramid[1], 2.0f, pyramidtemp[1] );

		//collapse the pyramid
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened.png");

		cout << "sharpened kitten finished " << endl;

		//X1.5
		for( int i = 0; i < pyramid.size() - 1; ++i )
			Multiply( pyramid[i], 1.50f, pyramidtemp[i] );
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened_1_5.png");

		//this one looks the best
		ToOfImage( image, m_current );

		//X2.0
		for( int i = 0; i < pyramid.size() - 1; ++i )
			Multiply( pyramid[i], 2.00f, pyramidtemp[i] );
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened_2_0.png");

		//X2.5
		for( int i = 0; i < pyramid.size() - 1; ++i )
			Multiply( pyramid[i], 2.50f, pyramidtemp[i] );
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened_2_5.png");

		//X3.0
		for( int i = 0; i < pyramid.size() - 1; ++i )
			Multiply( pyramid[i], 3.0f, pyramidtemp[i] );
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened_3_0.png");

		//X3.5
		for( int i = 0; i < pyramid.size() - 1; ++i )
			Multiply( pyramid[i], 3.50f, pyramidtemp[i] );
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened_3_5.png");

		//4.0
		for( int i = 0; i < pyramid.size() - 1; ++i )
			Multiply( pyramid[i], 4.00f, pyramidtemp[i] );
		CollapseLaplacianPyramid( pyramidtemp, image );
		Clamp( image );
		SaveMatrix( image, "03-kitten_sharpened_4_0.png");
	}

	void		App::ShowStage4Before()
	{
		//noise removal by coring
		MatrixF image;
		FromOfImage( m_image, image );

		//add low magnitude uniform noise
		Noise( image, 15.0f, image );
		Clamp( image );
		ToOfImage( image, m_current );
	}

	void		App::ShowStage4After()
	{
		//noise removal by coring
		MatrixF image;
		FromOfImage( m_image, image );

		//add low magnitude uniform noise
		Noise( image, 15.0f, image );
		Clamp( image );
		SaveMatrix( image, "04-noised_image.png" );

		//construct the pyramid
		MatrixFArray pyramid;
		LaplacianPyramid( image, pyramid, 0.40f );

		//we adaptively core the pyramid
		//set the threshold to 10% of the average absolute value
		for( int i = 0; i < pyramid.size() - 1; ++i )
		{
			MatrixF temp;
			
			//take absolute
			Absolute( pyramid[i], temp );

			//take sum
			float threshold = 5;

			//perform the coring
			for( int y = 0; y < pyramid[i].Rows(); ++y )
			{
				for( int x = 0; x < pyramid[i].Cols(); ++x )
				{
					if( abs(pyramid[i][y][x] ) < threshold )
						pyramid[i][y][x] = 0;
				}
			}
		}

		//collapse the pyramid
		CollapseLaplacianPyramid( pyramid, image );
		Clamp( image );
		SaveMatrix( image, "04-cored_image.png");

		//copy to show
		ToOfImage( image, m_current );
	}

	void App::ShowStage5a()
	{
		MatrixF image;
		FromOfImage( m_churchImage, image );
		SaveMatrix( image, "05-church_original.png");
		LocalLaplacian( m_churchImage, 0.3f, 0.25, "05-church_pt3_pt25.png" );
		LocalLaplacian( m_churchImage, 0.3f, 0.75, "05-church_pt3_pt75.png" );
		LocalLaplacian( m_churchImage, 0.3f, 2.00, "05-church_pt3_2.png" );
		LocalLaplacian( m_churchImage, 0.3f, 4.00, "05-church_pt3_4.png" );
	}

	void App::ShowStage5b()
	{
		MatrixF image;
		FromOfImage( m_kittenImage, image );
		SaveMatrix( image, "05-kitten_original.png");
		LocalLaplacian( m_kittenImage, 0.3f, 0.25, "05-kitten_pt3_pt25.png" );
		LocalLaplacian( m_kittenImage, 0.3f, 0.75, "05-kitten_pt3_pt75.png" );
		LocalLaplacian( m_kittenImage, 0.3f, 2.00, "05-kitten_pt3_2.png" );
		LocalLaplacian( m_kittenImage, 0.3f, 4.00, "05-kitten_pt3_4.png" );
	}

	void App::ShowStage5c()
	{
		MatrixF image;
		FromOfImage( m_flowerImage, image );
		SaveMatrix( image, "05-flower_original.png");
		LocalLaplacian( m_flowerImage, 0.3f, 0.25, "05-flower_pt3_pt25.png" );
		LocalLaplacian( m_flowerImage, 0.3f, 0.75, "05-flower_pt3_pt75.png" );
		LocalLaplacian( m_flowerImage, 0.3f, 2.00, "05-flower_pt3_2.png" );
		LocalLaplacian( m_flowerImage, 0.3f, 4.00, "05-flower_pt3_4.png" );
	}

	void		App::ShowStage6a()
	{
		LocalLaplacianColor( m_churchImage, 0.3f, 0.50, "06-church_pt3_pt5.png");
		LocalLaplacianColor( m_kittenImage, 0.3f, 0.50, "06-kitten_pt3_pt5.png");
	}

	void		App::ShowStage6b()
	{
		LocalLaplacianColor( m_churchImage, 0.3f, 2.00, "06-church_pt3_2.png");
		LocalLaplacianColor( m_kittenImage, 0.3f, 2.00, "06-kitten_pt3_2.png");
	}

	void		App::ShowStage7()
	{
		LocalLaplacianHSV( m_churchImage, 0.3f, 0.50, "07-church_pt3_pt5.png");
		LocalLaplacianHSV( m_churchImage, 0.3f, 4.00, "07-church_pt3_4.png");
	}

	void		App::LocalLaplacian( const ofImage& _image, float sigma, float alpha, const std::string& filename )
	{
		MatrixF image;
		FromOfImage( _image, image );

		Multiply( image, 1.0f/255.0f, image );
		LocalLaplacianFilter( image, sigma, alpha, image );
		Multiply( image, 255.0f, image );
		Clamp( image );

		SaveMatrix( image, filename );
	}

	void		App::LocalLaplacianColor( const ofImage& _image, float sigma, float alpha, const std::string& filename )
	{
		//some RGB color images
		MatrixF channels[3];
		FromOfImage( _image, channels[0], channels[1], channels[2] );
		
		for( int i = 0; i < 3; ++i )
		{
			Multiply( channels[i], 1.0f/255.0f, channels[i] );
			LocalLaplacianFilter( channels[i], sigma, alpha, channels[i] );
			Multiply( channels[i], 255.0f, channels[i] );
			Clamp( channels[i] );
		}
		
		ofImage img;
		ToOfImage( channels[0], channels[1], channels[2], img );
		SaveImage( img, filename );
	}

	void	App::LocalLaplacianHSV( const ofImage& _image, float sigma, float alpha, const std::string& filename )
	{
		//some HSV
		MatrixF channels[3];
		FromOfImageHSV( _image, channels[0], channels[1], channels[2] );

		//filter only the value
		int sel = 2;

		Multiply( channels[sel], 1.0f/255.0f, channels[sel] );
		LocalLaplacianFilter( channels[sel], sigma, alpha, channels[sel] );
		Multiply( channels[sel], 255.0f, channels[sel] );
		Clamp( channels[sel] );

		ofImage img;
		ToOfImageHSV( channels[0], channels[1], channels[2], img );
		SaveImage( img, filename );
	}
};
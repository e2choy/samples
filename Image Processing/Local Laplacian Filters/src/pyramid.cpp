#include "stdafx.h"
#include "pyramid.h"
#include "comp5900.h"

namespace Comp5900
{
	namespace Pyramid
	{
		//helper functions///////////////////////////////////////////////////////
		void	Decimate( const MatrixF& src, MatrixF& dest )
		{
			MatrixF temp;

			//the size of the decimated pyramid is
			//(width + 1) / 2 , (height + 1)/2
			temp.Resize( (src.Rows() + 1)/2, (src.Cols() + 1)/2 );
			for( int y = 0; y < src.Rows(); y += 2 )
			{
				for( int x = 0; x < src.Cols(); x += 2 )
				{
					temp[y/2][x/2] = src[y][x];
				}
			}
			dest = temp;
		}

		static int LevelsInPyramid( int width, int height )
		{
			int levels = 0;
			int minimum = MIN( width, height );
			while( minimum >  1 )
			{
				levels++;
				minimum = (minimum+1)/2;
			}
			return levels;
		}
		///////////////////////////////////////////////////////////////////////////

		void	GetKernel( float a, MatrixF& dest )
		{
			assert( a > 0.0f && a <= 0.50f );

			//1 x 5 matrix
			dest.Resize( 1, 5 );

			//c
			dest[0][0] = 0.25f - ( 0.5f * a );
			dest[0][4] = 0.25f - ( 0.5f * a );

			//b
			dest[0][1] = 0.25f;
			dest[0][3] = 0.25f;

			//a
			dest[0][2] = a;
		}

		void	Gaussian( const MatrixF& src, MatrixF& dest, float a )
		{
			//set the kernel
			MatrixF kernel, kerneltranspose; 
			GetKernel( a, kernel );
			Transpose( kernel, kerneltranspose );

			//perform the convolution
			MatrixF temp = src;
			Convolve( temp, kernel, temp );
			Convolve( temp, kerneltranspose, temp );
			dest = temp;
		}

		void	GaussianPyramid( const MatrixF& image, MatrixFArray& pyramid, float a, int levels )
		{
			//get the number of levels
			if( levels != 0 )
				levels = min( levels, LevelsInPyramid( image.Cols(), image.Rows() ));
			else
				levels = LevelsInPyramid( image.Cols(), image.Rows() );

			//the zeroth level is the original image
			MatrixF last = image;
			pyramid.push_back( last );

			//convolve and decimate
			for( int i = 1; i < levels; ++i )
			{
				Reduce( last, last, a );
				pyramid.push_back( last );
			}
		}

		void	Reduce( const MatrixF& src, MatrixF& dest, float a )
		{
			MatrixF temp = src;
			Gaussian( temp, temp, a );
			Decimate( temp, temp );
			dest = temp;
		}

		void	Expand( const MatrixF& src, MatrixF& dest, float a )
		{
			//temporary image zeroed out
			MatrixF temp;
			temp.Resize( 2*src.Rows(), 2*src.Cols() );

			//generate expanded image spaced apart
			for( int y = 0; y < src.Rows(); ++y )
			{
				for( int x = 0; x < src.Cols(); ++x )
				{
					temp[2*y][2*x] = src[y][x];
				}
			}

			//apply the gaussian
			Gaussian( temp, temp, a );

			//find weight
			MatrixF weight;
			weight.Resize( 2 * src.Rows(), 2 * src.Cols() );
			for( int y = 0; y < src.Rows(); ++y )
			{
				for( int x = 0; x < src.Cols(); ++x )
				{
					weight[2*y][2*x] = 1.0f;
				}
			}
			Gaussian( weight, weight, a );

			//divide weights
			Divide( temp, weight, temp );

			//copy final result
			dest = temp;

		}

		void	ExpandOdd( const MatrixF& src, MatrixF& dest, float a )
		{
			//temporary image zeroed out
			MatrixF temp;
			temp.Resize( (2*src.Rows())-1, (2*src.Cols())-1 );

			//generate expanded image spaced apart
			for( int y = 0; y < src.Rows(); ++y )
			{
				for( int x = 0; x < src.Cols(); ++x )
				{
					temp[2*y][2*x] = src[y][x];
				}
			}

			//apply the gaussian
			Gaussian( temp, temp, a );

			//find weight
			MatrixF weight;
			weight.Resize( (2*src.Rows())-1, (2*src.Cols())-1 );
			for( int y = 0; y < src.Rows(); ++y )
			{
				for( int x = 0; x < src.Cols(); ++x )
				{
					weight[2*y][2*x] = 1.0f;
				}
			}
			Gaussian( weight, weight, a );

			//divide weights
			Divide( temp, weight, temp );

			//copy final result
			dest = temp;

		}
		//create the laplacian pyramid
		void	LaplacianPyramid( const MatrixF& image, MatrixFArray& laplacianPyramid, float a, int levels )
		{
			MatrixFArray gaussianPyramid;

			//first create the gaussian pyramid
			//in our notation level0 is the largest image
			GaussianPyramid( image, gaussianPyramid, a, levels );

			//take the difference of gaussians
			for( int i = 0; i < gaussianPyramid.size() - 1; ++i )
			{
				MatrixF expandedGaussian;
				MatrixF laplacian;
				Expand( gaussianPyramid[i+1] , expandedGaussian, a );

				Subtract(gaussianPyramid[i], expandedGaussian, laplacian);
				laplacianPyramid.push_back(laplacian);

			}

			//the final output is the highest level gaussian
			laplacianPyramid.push_back(gaussianPyramid[gaussianPyramid.size() - 1]);
		}

		void	CollapseLaplacianPyramid( const MatrixFArray& laplacianPyramid, MatrixF& image, float a )
		{
			//start with the highest level pyramid
			MatrixF temp = laplacianPyramid[ laplacianPyramid.size() - 1 ];

			for( int i = laplacianPyramid.size() - 1; i > 0; --i )
			{
				Expand( temp, temp, a );
				Add( temp, laplacianPyramid[ i - 1 ], temp );
			}

			//temp now contains the reconstructed image
			image = temp;
		}
	}
};
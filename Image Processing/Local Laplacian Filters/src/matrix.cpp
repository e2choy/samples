#include "stdafx.h"
#include "matrix.h"
#include "point.h"

using std::for_each;

namespace Comp5900
{
	void	Threshold( const Matrix& src, int threshold, int minValue, int maxValue, Matrix& dest )
	{
		dest.Resize( src.Cols(), src.Rows() );
		for( int y = 0; y < src.Cols(); ++y )
			for( int x = 0; x < src.Rows(); ++x )
				dest[y][x] = (dest[y][x] > threshold) ? maxValue : minValue;
	}

	void	Power( const MatrixF& src, float power, MatrixF& dest )
	{
		dest.Resize( src.Cols(), src.Rows() );
		for( int y = 0; y < src.Cols(); ++y )
			for( int x = 0; x < src.Rows(); ++x )
				dest[y][x] = pow( src[y][x], power );
	}

	void	Gaussian( const _Matrix<float>& src, int deviation, _Matrix<float>& dest )
	{
		MatrixF kernelX;
		MatrixF kernelY;

		GaussianKernel( deviation, kernelX );
		Transpose( kernelX, kernelY );

		dest.Resize(src.Rows(),src.Cols());
		_Matrix<float> temp(src.Rows(),src.Cols());

		Convolve( src, kernelX, temp );
		Convolve( temp, kernelY, dest );
	}

	void Gradient( const MatrixF& matrix, MatrixF& grad )
	{
		float xSobel[] = 
		{
			-1.0f, 0.0f, 1.0f,
			-2.0f, 0.0f, 2.0f,
			-1.0f, 0.0f, 2.0f
		};
		MatrixF xSobelMatrix( 3, 3, xSobel );

		float ySobel[] = 
		{
			-1.0f, 0.0f, 1.0f,
			-2.0f, 0.0f, 2.0f,
			-1.0f, 0.0f, 2.0f
		};
		MatrixF ySobelMatrix( 3, 3, ySobel );

		MatrixF xGrad;
		Convolve( matrix, xSobelMatrix, xGrad );
		Power(xGrad,2,xGrad);

		MatrixF yGrad;
		Convolve( matrix, xSobelMatrix, yGrad );
		Power(yGrad,2,yGrad);

		Add( xGrad, yGrad, grad );
		Power( grad, 0.5, grad );
	}

	void	GaussianKernel( int deviation, MatrixF& dest )
	{
		dest.Resize( 1, (deviation * 3) * 2 + 1 );
		int center = deviation * 3;

		for( int i = 0; i <= dest.Cols() / 2; ++i )
		{
			float value = pow(2.718281f, -1.0f * pow(i*i*1.0f,2.0f)/(2.0f*deviation*deviation));
			dest[0][center + i] = value;
			dest[0][center - i] = value;
		}
		
		float sum = Sum( dest );
		Multiply( dest, 1.0f / sum, dest );
	}
};
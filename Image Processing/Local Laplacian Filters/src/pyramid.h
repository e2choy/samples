#pragma once

#include "comp5900.h"


namespace Comp5900
{
	namespace Pyramid
	{
		void	GaussianPyramid( 
			const MatrixF& image, 
			MatrixFArray& pyramid, 
			float a = 0.4f, 
			int levels = 0 );	//0 levels is max levels

		void	LaplacianPyramid( 
			const MatrixF& image, 
			MatrixFArray& pyramid, 
			float a = 0.4f, 
			int levels = 0 );

		void	CollapseLaplacianPyramid( 
			const MatrixFArray& pyramid, 
			MatrixF& image,
			float a = 0.4f);

		void	Expand( const MatrixF& src, MatrixF& dest, float a );
		void	ExpandOdd( const MatrixF& src, MatrixF& dest, float a );
		void	Reduce( const MatrixF& src, MatrixF& dest, float a );

		void	Decimate( const MatrixF& src, MatrixF& dest );

		void	GetKernel( float a, MatrixF& dest );
		void	Gaussian( const MatrixF& src, MatrixF& dest, float a  );
	};

};
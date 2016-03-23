#include "local_laplacian.h"
#include "comp5900.h"
#include "pyramid.h"
#include "remapping.h"

namespace Comp5900
{
	namespace Pyramid
	{

		void LocalLaplacianFilter( 
			const MatrixF& src,
			float sigma,
			float alpha,
			MatrixF& dest)
		{
			//our remapping function
			Remapping remapFunction(sigma,alpha);

			//compute input gaussian pyramid
			MatrixFArray gpyramid, lpyramid;
			GaussianPyramid( src, gpyramid );

			//resize the laplacian pyramid
			lpyramid.resize( gpyramid.size() - 1 );
			for( int i = 0; i < lpyramid.size(); ++i )
				lpyramid[i].Resize( gpyramid[i].Rows(), gpyramid[i].Cols() );

			//no optimization
			//take the whole subregion
			cout << "Beginning" << endl;
			for( int i = 0; i < gpyramid.size() - 1; ++i )
			{
				for( int y = 0; y < gpyramid[i].Rows(); ++y )
				{
					for( int x = 0; x < gpyramid[i].Cols(); ++x )
					{		
						lpyramid[i][y][x] = FindLaplacianValue(
							src, remapFunction, gpyramid[i][y][x], i, y, x);
					}
				}
			}
			cout << "Ending" << endl;
			lpyramid.push_back( gpyramid[gpyramid.size()-1] );

			//collapse the pyramid
			CollapseLaplacianPyramid( lpyramid, dest );
		}

		void RemapImage( 
			const MatrixF& src, 
			float g0, 
			const Remapping& remapping,
			MatrixF& dest  )
		{
			dest.Resize( src.Rows(), src.Cols() );
			for( int y = 0; y < src.Rows(); ++y )
			{
				for( int x = 0; x < src.Cols(); ++x )
				{
					dest[y][x] = remapping( g0, src[y][x] );
				}
			}
		}

		float FindLaplacianValue(
			const MatrixF& img,
			const Remapping& remapping,
			float gvalue,
			int level,
			int row,
			int col )
		{
			//find the total size we need
			int size = 3 * ( pow(2.0f,level+2) - 1 );
			int halfSize = size / 2;

			//find the corresponding point from that level to original image
			int rowTemp = row*pow(2.0f,level);
			int colTemp = col*pow(2.0f,level);

			//copy the whole block (replicating when necessary)
			MatrixF mat[2];
			MatrixF* dest0 = &(mat[0]);
			MatrixF* dest1 = &(mat[1]);

			Point topLeft( colTemp - halfSize, rowTemp - halfSize );
			Point bottomRight( colTemp + halfSize, rowTemp + halfSize );
			Copy( img, topLeft, bottomRight, *dest0 );
			RemapImage(*dest0, gvalue, remapping, *dest0);

			Pyramid::Gaussian( *dest0, *dest1, 0.4f );
			Copy( *dest1, Point(2,2), Point(dest1->Cols()-3,dest1->Rows()-3), *dest1);
			Decimate(*dest1,*dest1);

			for(int i = 0; i < level; ++i)
			{
				std::swap( dest0, dest1 );
				Pyramid::Gaussian( *dest0, *dest1, 0.4f );
				Copy( *dest1, Point(2,2), Point(dest1->Cols()-3,dest1->Rows()-3), *dest1);
				Decimate(*dest1,*dest1);
			}

			//expand
			assert( dest1->Rows() == 3 );
			assert( dest1->Cols() == 3 );
			ExpandOdd( *dest1, *dest1, 0.4f ); //now 5X5

			//we need to also reduce dest0
			Copy( *dest0, Point(2,2), Point(dest0->Cols()-3,dest0->Rows()-3), *dest0);
			MatrixF laplacian;
			Subtract(*dest0, *dest1, laplacian);
			return laplacian[laplacian.Rows()/2][laplacian.Cols()/2];
		}
	};
};
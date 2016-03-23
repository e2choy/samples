#pragma once

#include "stdafx.h"
#include "ofMain.h"
#include "operations.h"
#include "point.h"

namespace Comp5900
{
	template<class T>
	class _Matrix
	{
	public:
		_Matrix():m_rows(0), m_cols(0)					{}

		_Matrix( const _Matrix& rhs )					{ operator = ( rhs ); }

		_Matrix& operator = ( const _Matrix& rhs )
		{
			if( this != &rhs )
			{
				m_rows		=	rhs.m_rows;
				m_cols		=	rhs.m_cols;
				m_buffer	=	rhs.m_buffer;
			}
			return *this;
		}

		template< class S >
		_Matrix( const _Matrix<S>& rhs )
		{
			operator = ( rhs );
		}

		template<class S>
		_Matrix& operator = ( const _Matrix<S>& rhs )
		{
			Resize( rhs.Rows(), rhs.Cols() );

			for( int y = 0; y < Rows(); ++y )
			{
				for( int x = 0; x < Cols(); ++x )
				{
					(*this)[y][x] = static_cast<T>( rhs[y][x] );
				}
			}
			return *this;
		}

		_Matrix( int rows, int cols ): m_rows( rows ), m_cols( cols )				{ Resize( rows, cols ); }

		_Matrix( int rows, int cols, T* values )									{ Resize( rows, cols ); std::copy( values, values + Size(), m_buffer.begin()); }

		~_Matrix(){}

		T*			operator[]( int row )					{ return & m_buffer[ m_cols * row ]; }
		const T*	operator[]( int row ) const				{ return & m_buffer[ m_cols * row ]; }
	
		int Rows()		const														{ return m_rows; }
		int Cols()		const														{ return m_cols; }
		int Height()	const														{ return m_rows; }
		int Width()		const														{ return m_cols; }

		int		Size()		const													{ return m_rows * m_cols; }
		void	Resize( int rows, int cols )										{ m_rows = rows; m_cols = cols; m_buffer.resize( Size() ); }

	private:
		int						m_rows;
		int						m_cols;
		std::vector<T>			m_buffer;
	};

	typedef _Matrix<int>				Matrix;
	typedef std::vector<Matrix>			MatrixArray;

	typedef _Matrix<float>				MatrixF;
	typedef std::vector<MatrixF>		MatrixFArray;

	typedef _Matrix<double>				MatrixD;
	typedef std::vector<MatrixD>		MatrixDArray;

	template<class T,class U>
	void Transform( _Matrix<T>& matrix, U& function )
	{
		for( int row = 0; row < matrix.Rows(); ++row )
			for( int col = 0; col < matrix.Cols(); ++col )
				matrix[row][col] = function( matrix[row][col] );
	}

	template<class T,class U>
	void Transform( const _Matrix<T>& src1, const _Matrix<T>& src2, _Matrix<T>& dest, U& function )
	{
		dest.Resize( src1.Rows(), src1.Cols() );
		for( int row = 0; row < src1.Rows(); ++row )
			for( int col = 0; col < src1.Cols(); ++col )
				dest[row][col] = function( src1[row][col], src2[row][col] );
	}

	template<class T>
	void Set(_Matrix<T>& matrix, T newvalue )													
	{ 
		Transform( matrix, [&newvalue](T value) { return newvalue; } ); 
	}

	template<class T>
	void Absolute( _Matrix<T>& matrix )
	{
		Transform( matrix, [](T value) { return abs(value); } );
	}

	template<class T>
	void Absolute( _Matrix<T>& src, _Matrix<T>& dest )
	{		
		dest.Resize( src.Rows(), src.Cols());
		for( int row = 0; row < src.Rows(); ++row )
			for( int col = 0; col < src.Cols(); ++col )
				dest[row][col] = abs( src[row][col] );
	}

	template<class T>
	void Add( const _Matrix<T>& src1, const _Matrix<T>& src2, _Matrix<T>& dest )
	{
		dest.Resize( src1.Rows(), src1.Cols());
		Transform( src1, src2, dest, [](T input1, T input2){ return input1 + input2; } );
	}

	template<class T>
	void Subtract( const _Matrix<T>& src1, const _Matrix<T>& src2, _Matrix<T>& dest )
	{
		dest.Resize( src1.Rows(), src1.Cols());
		Transform( src1, src2, dest, [](T input1, T input2){ return input1 - input2; } );
	}

	template<class T>
	void Multiply( const _Matrix<T>& src1, const _Matrix<T>& src2, _Matrix<T>& dest )
	{
		dest.Resize( src1.Rows(), src1.Cols());
		Transform( dest, [](T input1, T input2){ return input1 * input2; } );
	}

	template<class T,class S>
	void Multiply( const _Matrix<T>& src1, const S& src2, _Matrix<T>& dest )
	{
		dest.Resize( src1.Rows(), src1.Cols());
		for( int row = 0; row < src1.Rows(); ++row )
			for( int col = 0; col < src1.Cols(); ++col )
				dest[row][col] = src1[row][col] * src2;
	}

	template<class T1, class T2>
	void ConvolveSeparable( const _Matrix<T1>& src, const _Matrix<T2>& kernel, _Matrix<T1>& dest )
	{
		_Matrix<T2> transpose = kernel;
		Transpose( transpose, transpose );
		
		_Matrix<T1> temp( src.Rows(), src.Cols() );
		Convolve( src, kernel, temp );
		Convolve( temp, transpose, temp );
		dest = temp;
	}

	template<class T1, class T2>
	void Convolve( const _Matrix<T1>& src, const _Matrix<T2>& kernel, _Matrix<T1>& dest )
	{
		_Matrix<T1> temp( src.Rows(), src.Cols() );
		for( int y = 0; y < src.Rows(); ++y )
		{
			for( int x = 0; x < src.Cols(); ++x )
			{
				temp[y][x] = PointConvolve( y, x, src, kernel );
			}
		}
		dest = temp; 
	}

	template<class T1, class T2>
	T1 PointConvolve( int y, int x, const _Matrix<T1>& matrix, const _Matrix<T2>& kernel )
	{
		int xIndex = 0;
		int yIndex = 0;
		int kernelX = kernel.Cols() / 2;
		int kernelY = kernel.Rows() / 2;
		T1 sum = T1();
		for( int row = 0; row < kernel.Rows(); ++row )
		{
			for( int col = 0; col < kernel.Cols(); ++col )
			{
				xIndex = x + col - kernelX;
				yIndex = y + row - kernelY;

				if( xIndex >= matrix.Cols())
				{
					xIndex = Mod( 2*matrix.Cols()-xIndex-1,matrix.Cols());
				}
				else if( xIndex < 0 )
				{
					xIndex = Mod(-1*(xIndex+1),matrix.Cols());
				}

				if( yIndex >= matrix.Rows())
				{
					yIndex = Mod( 2*matrix.Rows()-yIndex-1,matrix.Rows());
				}
				else if( yIndex < 0 )
				{
					yIndex = Mod(-1*(yIndex+1),matrix.Rows());
				}


				sum += (matrix[yIndex][xIndex] * kernel[row][col]);
			}
		}
		return sum;
	}

	template<class T>
	T Sum( const _Matrix<T>& matrix )
	{
		T sum = static_cast<T>(0);
		for( int y = 0; y < matrix.Cols(); ++y )
			for( int x = 0; x < matrix.Rows(); ++x )
				sum += matrix[y][x];
		return sum;
	}

	template<class T>
	T Max( const _Matrix<T>& matrix )
	{
		if( ! matrix.Size() ) return static_cast<T>(0);
		T max = matrix(0,0);
		for( int y = 0; y < src.Cols(); ++y )
			for( int x = 0; x < src.Rows(); ++x )
				max = MAX( max, matrix[y][x]);
		return max;
	}

	template<class T>
	T Min( const _Matrix<T>& matrix )
	{
		if( ! matrix.Size() ) return static_cast<T>(0);
		T min = matrix(0,0);
		for( int y = 0; y < src.Cols(); ++y )
			for( int x = 0; x < src.Rows(); ++x )
				min = MIN( min, matrix[y][x]);
		return min;
	}

	template<class T>
	void	Transpose( const _Matrix<T>& src, _Matrix<T>& dest )
	{
		dest.Resize( src.Cols(), src.Rows() );
		for( int y = 0; y < src.Cols(); ++y )
			for( int x = 0; x < src.Rows(); ++x )
				dest[y][x] = src[x][y];
	}

	template<class T>
	void	Divide( const _Matrix<T>& src1, const _Matrix<T>& src2, _Matrix<T>& dest )
	{
		assert( src1.Cols() == src2.Cols() );
		assert( src1.Rows() == src2.Rows() );
		dest.Resize( src1.Rows(), src1.Cols() );

		for( int y = 0; y < src1.Cols(); ++y )
			for( int x = 0; x < src1.Rows(); ++x )
				dest[y][x] = src1[y][x] / src2[y][x];
	}

	template<class T>
	void	Copy( const _Matrix<T>& src,
		const Point& topleft,
		const Point& bottomright,
		_Matrix<T>& dest )
	{

		int xsize = bottomright.X - topleft.X + 1;
		int ysize = bottomright.Y - topleft.Y + 1;
		_Matrix<T> temp; temp.Resize(ysize,xsize);

		for(int y = 0; y < ysize; ++y )
		{
			for(int x = 0; x < xsize; ++x)
			{
				int xIndex = topleft.X + x;
				int yIndex = topleft.Y + y;
				if( xIndex >= src.Cols())
				{
					xIndex = Mod( 2*src.Cols()-xIndex-1,src.Cols());
				}
				else if( xIndex < 0 )
				{
					xIndex = Mod(-1*(xIndex+1),src.Cols());
				}

				if( yIndex >= src.Rows())
				{
					yIndex = Mod( 2*src.Rows()-yIndex-1,src.Rows());
				}
				else if( yIndex < 0 )
				{
					yIndex = Mod(-1*(yIndex+1),src.Rows());
				}
				temp[y][x] = src[yIndex][xIndex];
			}
		}
		dest = temp;
	}

	void	Threshold( const Matrix& src, int threshold, int minValue, int maxValue, Matrix& dest );

	void	Power( const MatrixF& src, float power, MatrixF& dest );

	void	Gradient( const MatrixF&, MatrixF& );

	void	GaussianKernel( int deviation, MatrixF& dest );

	void	Gaussian( const _Matrix<float>& src, int deviation, _Matrix<float>& dest );

	template<class T>
	std::ostream& operator<< (ostream &out, const _Matrix<T>& matrix)
	{
		for( int i = 0; i < matrix.Rows(); ++i )
		{
			for( int j = 0; j < matrix.Cols(); ++j )
			{
				out << matrix[i][j] << " ";
			}
			out << std::endl;
		}
		return out;
	}
};
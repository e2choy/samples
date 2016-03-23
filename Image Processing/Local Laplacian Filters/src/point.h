#pragma once
#include "stdafx.h"

namespace Comp5900
{
	template<class T>
	class _Point
	{
	public:
		_Point():
		  X(0), Y(0)
		{
		}

		_Point( const T& x, const T& y ):
		X(x), Y(y)
		{
		}

		T X, Y;
	};

	typedef _Point<int>			Point;
	typedef _Point<int>			Size;
	typedef _Point<float>		PointF;
	typedef _Point<float>		SizeF;

	typedef std::vector<Point>	Points;

	template<class T>
	T Distance( _Point<T> pt1, _Point<T> pt2 )
	{
		T distance = static_cast<T>(
			sqrt( pow((pt1.X - pt2.X),2) + pow(( pt1.Y - pt2.Y ),2)  )
			);
		return distance;
	}
};
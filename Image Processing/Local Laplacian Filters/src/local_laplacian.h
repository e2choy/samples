#pragma once

#include "comp5900.h"

namespace Comp5900
{
	namespace Pyramid
	{

		class Remapping;

		void LocalLaplacianFilter( 
			const MatrixF& src,
			float sigma,
			float alpha,
			MatrixF& dest);

		void RemapImage( 
			const MatrixF& src, 
			float local, 
			const Remapping& remapping,
			MatrixF& dest  );

		float FindLaplacianValue(
			const MatrixF& img,
			const Remapping& remapping,
			float gvalue,
			int level,
			int row,
			int col );
	};
};
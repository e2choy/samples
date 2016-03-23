#pragma once

#include "comp5900.h"

namespace Comp5900
{
	namespace Pyramid
	{
		class Remapping
		{
		public:
			Remapping(float sigma, float alpha);
			virtual float operator()( float local, float value ) const;
			virtual ~Remapping(){};
		private:
			float m_sigma;
			float m_alpha;

			float Rd(float g0, float value) const;
			float Re(float g0, float value) const;

			float Fd(float f) const;
			float Fe(float f) const;
		};
	};
};
#include "remapping.h"

namespace Comp5900
{
	namespace Pyramid
	{

		Remapping::Remapping( float sigma,		//what is edge vs detail
			float alpha	):						//detail remapping parameter for Fd
			m_sigma(sigma),
			m_alpha(alpha)
		{
		}


		float Remapping::operator()(
			float g0,			//local gaussian value in pyramid g0
			float value			//pixel of original full scale image
			) const
		{
			float diff = abs( value - g0 );

			if( diff <= m_sigma )
			{
				return Rd(g0,value);
			}
			else
			{
				return Re(g0,value);
			}
		}

		float Remapping::Rd(float g0, float value) const
		{
			float diff = value - g0;
			float scale = abs(diff) / m_sigma;
			float ret = g0 + Sign(diff)*m_sigma*Fd(scale);

			return ret;
		}

		float Remapping::Re(float g0, float value) const
		{
			float diff = value - g0;
			float ret = g0 + Sign(diff)*(Fe(abs(diff) - m_sigma) + m_sigma);

			return ret;
		}

		float Remapping::Fd(float f) const
		{
			float ret = pow(f,m_alpha);
			if( m_alpha < 1.0f )
			{
				float t = SmoothStep( 0.01,0.02,f*m_sigma);
				ret = t*ret + (1.0f-t)*f;
			}
			return ret;
		}

		float Remapping::Fe(float f) const
		{
			//do nothing fe(a) = a
			return f;
		}


	};
};
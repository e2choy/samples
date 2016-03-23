#pragma once

namespace Comp5900
{
	template<class T, int depth>
	struct _Element
	{
	public:
		_Element()
		{
			for( int i = 0; i < depth; ++i )
			{
				m_elements[i] = static_cast<T>( 0 );
			}
		}

		_Element( const _Element& rhs )
		{
			operator = ( rhs );
		}

		_Element& operator = ( const _Element& rhs )
		{
			if( this != &rhs )
			{
				for( int i = 0; i < depth; ++i )
				{
					m_elements[i] = ( rhs.m_elements[i] );
				}
			}
			return *this;
		}

		template< class S >
		_Element( const _Element<S,depth>& rhs )
		{
			operator = ( rhs );
		}

		template< class S >
		_Element& operator = ( const _Element<S,depth>& rhs )
		{
			for( int i = 0; i < depth; ++i )
			{
				m_elements[i] = static_cast<T>( rhs[i] );
			}
			return * this;
		}

		template< class S >
		_Element& operator += ( const _Element<S,depth>& rhs )
		{
			for( int i = 0; i < depth; ++i )
			{
				m_elements[i] += rhs[i];
			}
			return * this;
		}


		int			Depth(){ return depth; }

		T&			operator[]( int index )			{ return m_elements[index]; }
		const T&	operator[]( int index )	const	{ return m_elements[index]; }

	private:
		T m_elements[depth];
	};

	template<class T, int depth>
	_Element<T,depth> operator * ( const _Element<T,depth>& rhs, float f )
	{
		_Element<T,depth> e;
		for( int i = 0; i < e.Depth(); ++i )
		{
			e[i] = rhs[i] * f;
		}
		return e;
	}

	typedef _Element<int,3>		Element3;
	typedef _Element<float,3>	Element3F;

};
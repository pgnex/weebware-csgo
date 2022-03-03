#pragma once
#include <cmath>
#define CHECK_VALID( _v ) 0
template <typename T>
struct _VECTOR2D
{
	T x;
	T y;

	_VECTOR2D( ) : x( 0 ), y( 0 ) {}
	_VECTOR2D( T x_, T y_ ) : x( x_ ), y( y_ ) {}
};

template <typename T>
struct _VECTOR : public _VECTOR2D<T>
{
	T z;

	_VECTOR( ) : _VECTOR2D<T>( ), z( 0 ) {}
	_VECTOR( T x_, T y_, T z_ ) : _VECTOR2D<T>( x_, y_ ), z( z_ ) {}

#pragma region Vector Operators

	inline float length( void ) const
	{
		CHECK_VALID( *this );

		float root = 0.0f;

		float sqsr = this->x * this->x + this->y * this->y + this->z * this->z;

		root = sqrt( sqsr );

		return root;
	}

	inline float dist_to( const _VECTOR<T>& rhs ) {
		return _VECTOR<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z).length();
	}

	inline _VECTOR<T> operator+( const _VECTOR<T>& rhs ) const
	{
		return _VECTOR<T>( this->x + rhs.x, this->y + rhs.y, this->z + rhs.z );
	}

	inline _VECTOR<T> operator-( const _VECTOR<T>& rhs ) const
	{
		return _VECTOR<T>( this->x - rhs.x, this->y - rhs.y, this->z - rhs.z );
	}

	inline T operator*( const _VECTOR<T>& rhs ) const
	{
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	}

	inline _VECTOR<T> operator+=( const _VECTOR<T>& rhs )
	{
		return *this = *this + rhs;
	}

	inline _VECTOR<T> operator-=( const _VECTOR<T>& rhs )
	{
		return *this = *this - rhs;
	}

	inline T operator*=( const _VECTOR<T>& rhs )
	{
		return *this = *this * rhs;
	}

#pragma endregion

#pragma region T Operators

	inline _VECTOR<T> operator+( const T& rhs ) const
	{
		return _VECTOR<T>( this->x + rhs, this->y + rhs, this->z + rhs );
	}

	inline _VECTOR<T> operator-( const T& rhs ) const
	{
		return _VECTOR<T>( this->x - rhs, this->y - rhs, this->z - rhs );
	}

	inline _VECTOR<T> operator*( const T& rhs ) const
	{
		return _VECTOR<T>( this->x * rhs, this->y * rhs, this->z * rhs );
	}

	inline _VECTOR<T> operator/( const T& rhs ) const
	{
		return _VECTOR<T>( this->x / rhs, this->y / rhs, this->z / rhs );
	}

	inline _VECTOR<T> operator+=( const T& rhs )
	{
		return *this = *this + rhs;
	}

	inline _VECTOR<T> operator-=( const T& rhs )
	{
		return *this = *this - rhs;
	}

	inline _VECTOR<T> operator*=( const T& rhs )
	{
		return *this = *this * rhs;
	}

	inline _VECTOR<T> operator/=( const T& rhs )
	{
		return *this = *this / rhs;
	}

#pragma endregion

#pragma region Other Operations

	inline T size_sqr( )
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	inline T size( )
	{
		return std::sqrt( size_sqr( ) );
	}

#pragma endregion

};

typedef _VECTOR<int> c_vectori;
typedef _VECTOR<float> Vector;
typedef _VECTOR2D<float> c_vector_2d;
typedef _VECTOR<float> QAngle;
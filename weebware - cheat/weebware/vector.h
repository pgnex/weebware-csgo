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

		float sqsr = x * x + y * y + z * z;

		root = sqrt( sqsr );

		return root;
	}

	float dist_to( const _VECTOR<T>& rhs ) const {
		Vector delta;

		delta.x = x - rhs.x;
		delta.y = y - rhs.y;
		delta.z = z - rhs.z;

		return delta.length( );
	}

	inline _VECTOR<T> operator+( const _VECTOR<T>& rhs ) const
	{
		return _VECTOR<T>( x + rhs.x, y + rhs.y, z + rhs.z );
	}

	inline _VECTOR<T> operator-( const _VECTOR<T>& rhs ) const
	{
		return _VECTOR<T>( x - rhs.x, y - rhs.y, z - rhs.z );
	}

	inline T operator*( const _VECTOR<T>& rhs ) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
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
		return _VECTOR<T>( x + rhs, y + rhs, z + rhs );
	}

	inline _VECTOR<T> operator-( const T& rhs ) const
	{
		return _VECTOR<T>( x - rhs, y - rhs, z - rhs );
	}

	inline _VECTOR<T> operator*( const T& rhs ) const
	{
		return _VECTOR<T>( x * rhs, y * rhs, z * rhs );
	}

	inline _VECTOR<T> operator/( const T& rhs ) const
	{
		return _VECTOR<T>( x / rhs, y / rhs, z / rhs );
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
		return x * x + y * y + z * z;
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

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	VectorAligned() = default;

	explicit VectorAligned(const Vector& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z; this->w = 0.f;
	}

	constexpr VectorAligned& operator=(const Vector& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z; this->w = 0.f;
		return *this;
	}

public:
	float w;
};
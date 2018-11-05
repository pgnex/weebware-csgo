#pragma once
#include "vector.h"
#include <chrono>
#include <ctime>
#include <cmath>

typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];

class c_maths
{
public:
#define Assert( _exp ) ((void)0)
bool s_bMathlibInitialized = false;
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )

	float dot_product(Vector a,  Vector b);

	void VectorTransform2(Vector& in1, float in2[3][4], Vector& out);

	matrix3x4* w2smatrix;
	void w2s_fixed_calcations();
	bool screen_transform(Vector& point, Vector& screen);
	bool world_to_screen(Vector& origin, Vector& screen);

	void vector_qangles(Vector forward, QAngle& angles);
	void qangle_vector(const QAngle& angles, Vector& forward);
	void qangle_vector(const QAngle& qAngles, Vector& vecForward, Vector& vecRight, Vector& vecUp);
	void vector_qangles3d(Vector& vecForward, Vector& vecAngles);
	void calc_angle(Vector src, Vector dst, Vector& angles);

	float get_fov(QAngle& viewAngle, QAngle& aimAngle, bool distance_scaling = 0, float dst = 0);
	void normalize_angle(QAngle& angle);
	void clamp_angle(QAngle& angle);
	void sin_cos(float value, float &sr, float &cr);

};

extern c_maths g_maths;




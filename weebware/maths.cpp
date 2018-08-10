#include "maths.h"
#include "shared.h"



c_maths g_maths;

float c_maths::dot_product(Vector a, Vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void c_maths::VectorTransform2(Vector& in1, float in2[3][4], Vector& out)
{
	out.x = dot_product(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out.y = dot_product(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out.z = dot_product(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}

void c_maths::w2s_fixed_calcations()
{
	w2smatrix = &g_weebware.g_engine->w2s_matrix();
}

bool c_maths::screen_transform(Vector& point, Vector& screen)
{
	float w;
	matrix3x4& worldToScreen = g_weebware.g_engine->w2s_matrix();
	screen.x = worldToScreen[0][0] * point.x + worldToScreen[0][1] * point.y + worldToScreen[0][2] * point.z + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point.x + worldToScreen[1][1] * point.y + worldToScreen[1][2] * point.z + worldToScreen[1][3];
	w = worldToScreen[3][0] * point.x + worldToScreen[3][1] * point.y + worldToScreen[3][2] * point.z + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool c_maths::world_to_screen(Vector& origin, Vector& screen)
{
	if (!screen_transform(origin, screen))
	{
		int ScreenWidth, ScreenHeight;
		g_weebware.g_engine->get_screen_dimensions(ScreenWidth, ScreenHeight);
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5 * screen.x * ScreenWidth + 0.5;
		y -= 0.5 * screen.y * ScreenHeight + 0.5;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

void c_maths::vector_qangles(Vector forward, QAngle& angles)
{
	float tmp, yaw, pitch;

	if (forward.z == 0 && forward.x == 0)
	{
		yaw = 0;

		if (forward.z > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward.y, forward.x) * 180 / 3.14159265358979323846f);

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2(-forward.z, tmp) * 180 / 3.14159265358979323846f);

		if (pitch < 0)
			pitch += 360;
	}

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
}

void c_maths::qangle_vector(const QAngle& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles.y));
	cy = cos(DEG2RAD(angles.y));

	sp = sin(DEG2RAD(angles.x));
	cp = cos(DEG2RAD(angles.x));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void c_maths::sin_cos(float value, float &sr, float &cr)
{
	sr = sin(value);
	cr = cos(value);
}

void c_maths::qangle_vector(const QAngle& qAngles, Vector& vecForward, Vector& vecRight, Vector& vecUp)
{
	float sp, sy, sr, cp, cy, cr;
	sin_cos((float)(qAngles.z * (PI / 180.f)), sr, cr);
	sin_cos((float)(qAngles.y * (PI / 180.f)), sy, cy);
	sin_cos((float)(qAngles.x * (PI / 180.f)), sp, cp);

	vecForward.x = cp * cy;
	vecForward.y = cp * sy;
	vecForward.z = -sp;

	vecRight.x = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
	vecRight.y = -1.0f * sr * sp * sy + -1.0f * cr * cy;
	vecRight.z = -1.0f * sr * cp;

	vecUp.x = cr * sp * cy + -sr * -sy;
	vecUp.y = cr * sp * sy + -sr * cy;
	vecUp.z = cr * cp;
}

void c_maths::normalize_angle(QAngle& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

void c_maths::clamp_angle(QAngle& angle)
{
	if (angle.y > 180.0f)
		angle.y = 180.0f;
	else if (angle.y < -180.0f)
		angle.y = -180.0f;

	if (angle.x > 89.0f)
		angle.x = 89.0f;
	else if (angle.x < -89.0f)
		angle.x = -89.0f;

	angle.z = 0;
}

float c_maths::get_fov(QAngle& viewAngle, QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	normalize_angle(delta);
	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}


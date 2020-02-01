#include "maths.h"
#include "shared.h"

c_maths g_maths;

float c_maths::dot_product(Vector a, Vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void c_maths::VectorTransform2(Vector& in1, const matrix3x4_t& in2, Vector& out)
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

void c_maths::calc_angle(Vector src, Vector dst, Vector& angles)
{
	Vector delta = src - dst;
	double hyp = delta.size();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	if (delta.x >= 0.0) angles.y += 180.0f;
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

void c_maths::vector_qangles3d(Vector& vecForward, Vector& vecAngles)
{
	Vector vecView;
	if (vecForward.y == 0.f && vecForward.x == 0.f)
	{
		vecView.x = 0.f;
		vecView.y = 0.f;
	}
	else
	{
		vecView.y = atan2(vecForward.y, vecForward.x) * 180.f / 3.14159265358979323846f;

		if (vecView.y < 0.f)
			vecView.y += 360;

		vecView.z = sqrt(vecForward.x * vecForward.x + vecForward.y * vecForward.y);

		vecView.x = atan2(vecForward.z, vecView.z) * 180.f / 3.14159265358979323846f;
	}

	vecAngles.x = -vecView.x;
	vecAngles.y = vecView.y;
	vecAngles.z = 0.f;
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

Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void c_maths::VectorAngles(const Vector& forward, Vector& up, QAngle& angles)
{
	Vector left = CrossProduct(up, forward);

	float forwardDist = forward.length();

	if (forwardDist > 0.001f)
	{
		angles.x = atan2f(-forward.z, forwardDist) * 180 / PI_F;
		angles.y = atan2f(forward.y, forward.x) * 180 / PI_F;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f(left.z, upZ) * 180 / PI_F;
	}
	else
	{
		angles.x = atan2f(-forward.z, forwardDist) * 180 / PI_F;
		angles.y = atan2f(-left.x, left.y) * 180 / PI_F;
		angles.z = 0;
	}
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

float c_maths::get_fov(QAngle& viewAngle, QAngle& aimAngle, bool distance_scaling, float dst)
{
	if (distance_scaling) {
		Vector delta = aimAngle - viewAngle;
		normalize_angle(delta);
		// delta.y = min(90, max(-90, delta.x));
		float dx1 = std::sin(DEG2RAD(delta.y)) * dst;
		float dx2 = std::sin(DEG2RAD(delta.x)) * dst;
		return std::sqrt(dx1 * dx1 + dx2 * dx2);
	}
	else {
		QAngle delta = aimAngle - viewAngle;
		normalize_angle(delta);
		return sqrtf(delta.x * delta.x + delta.y * delta.y);
	}
	return 0.f; // this should never happen
}



const matrix3x4& c_maths::GetWorldToScreen()
{
	const matrix3x4& pMatrix = *reinterpret_cast<matrix3x4*>(0x210 * 2 + *(unsigned long*)((unsigned long)g_weebware.g_render + 0xDC) - 0x44);
	return pMatrix;
}

bool c_maths::ClipTransform(Vector& in, Vector& out)
{
	const matrix3x4& worldToScreen = GetWorldToScreen();

	std::cout << "w2s: " << worldToScreen << std::endl;

	float w;
	out.x = worldToScreen[0][0] * in.x + worldToScreen[0][1] * in.y + worldToScreen[0][2] * in.z + worldToScreen[0][3];
	out.y = worldToScreen[1][0] * in.x + worldToScreen[1][1] * in.y + worldToScreen[1][2] * in.z + worldToScreen[1][3];
	w = worldToScreen[3][0] * in.x + worldToScreen[3][1] * in.y + worldToScreen[3][2] * in.z + worldToScreen[3][3];
	out.z = 0.0f;


	std::cout << "w: " << w << std::endl;

	bool behind;
	if (w < 0.001f)
	{
		behind = true;
		out.x *= 100000;
		out.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		out.x *= invw;
		out.y *= invw;
	}
	return behind;
}

bool c_maths::CRWorldToScreen(Vector& in, Vector& out)
{
	if (!ClipTransform(in, out))
	{
		int ScreenWidth, ScreenHeight;
		g_weebware.g_engine->get_screen_dimensions(ScreenWidth, ScreenHeight);
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5 * out.x * ScreenWidth + 0.5;
		y -= 0.5 * out.y * ScreenHeight + 0.5;
		out.x = x;
		out.y = y;
		return true;
	}

	return false;
}
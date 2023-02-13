#include <include.h>

float math::deg2rad(const float v)
{
	return (v * static_cast<float>(M_PI)) / 180.0f;
}

float math::rad2deg(const float v)
{
	return (v * 180.0f) / static_cast<float>(M_PI);
}

void math::sin_cos(float radians, float* sine, float* cosine)
{
	*sine = sinf(radians);
	*cosine = cosf(radians);
}

void math::VectorAngles(const math::vec3_t& forward, math::ang3_t& angles)
{
	float temp, yaw, pitch;

	if (forward.y == 0.0f && forward.x == 0.0f)
	{
		yaw = 0.0f;

		if (forward.z > 0.0f)
		{
			pitch = 270.0f;
		}
		else
		{
			pitch = 90.0f;
		}
	}
	else
	{
		yaw = atan2f(forward.y, forward.x) * 180.0f / static_cast<float>(M_PI);

		if (yaw < 0.0f)
		{
			yaw += 360.0f;
		}

		temp = sqrtf(forward.x * forward.x + forward.y * forward.y);
		pitch = atan2f(-forward.z, temp) * 180.0f / static_cast<float>(M_PI);

		if (pitch < 0.0f)
		{
			pitch += 360.0f;
		}

		angles.x = pitch;
		angles.y = yaw;
		angles.z = 0.0f;
	}
}

void math::AngleVectors(const math::ang3_t& in, math::vec3_t& out)
{
	float sp, sy, cp, cy;

	sin_cos(deg2rad(in.y), &sy, &cy);
	sin_cos(deg2rad(in.x), &sp, &cp);

	out.x = cp * cy;
	out.y = cp * sy;
	out.z = -sp;
}

void math::ClampAngle(vec3_t& angle)
{
	angle.x = std::clamp(angle.x, -89.0f, +89.0f);
	angle.y = std::clamp(angle.y, -180.0f, +180.0f);
	angle.z = std::clamp(angle.z, 0.0f, 0.0f);
}

void math::ClampAngle(ang3_t& angle)
{
	angle.x = std::clamp(angle.x, -89.0f, +89.0f);
	angle.y = std::clamp(angle.y, -180.0f, +180.0f);
	angle.z = std::clamp(angle.z, 0.0f, 0.0f);
}

float math::DotProduct(const math::vec3_t& first, const math::vec3_t& second)
{
	return ((first.x * second.x) + (first.y * second.y) + (first.z * second.z));
}

float math::dot_product(const float* first, const float* second)
{
	return ((first[0] * second[0]) + (first[1] * second[1]) + (first[2] * second[2]));
}

void math::NormalizeVector(math::vec3_t& in)
{
	if (in.x > 89.0f)
	{
		in.x -= 360.0f;
	}

	if (in.x < -89.0f)
	{
		in.x += 360.0f;
	}

	while (in.y > 180.0f)
	{
		in.y -= 360.0f;
	}

	while (in.y < -180.0f)
	{
		in.y += 360.0f;
	}
}

void math::NormalizeAngle(math::ang3_t& angle)
{
	if (angle.x > 89.0f)
	{
		angle.x -= 360.0f;
	}

	if (angle.x < -89.0f)
	{
		angle.x += 360.0f;
	}

	while (angle.y > 180.0f)
	{
		angle.y -= 360.0f;
	}

	while (angle.y < -180.0f)
	{
		angle.y += 360.0f;
	}
}

void math::fix_movement(float& forward_move, float& side_move, math::ang3_t view_angle, math::ang3_t previous_angle,
	const float old_forward_move, const float old_side_move) 
{
	float delta, first, second;

	if (previous_angle.y < 0.0f)
	{
		first = 360.0f + previous_angle.y;
	}
	else
	{
		first = previous_angle.y;
	}

	if (view_angle.y < 0.0f)
	{
		second = 360.0f + view_angle.y;
	}
	else
	{
		second = view_angle.y;
	}

	if (first > second)
	{
		delta = std::abs(first - second);
	}
	else
	{
		delta = 360.0f - std::abs(second - first);
	}

	delta = 360.0f - delta;

	forward_move = std::cos(math::deg2rad(delta)) * old_forward_move + std::cos(
		math::deg2rad(delta + 90.0f)) * old_side_move;
	side_move = std::sin(math::deg2rad(delta)) * old_forward_move + std::sin(
		math::deg2rad(delta + 90.0f)) * old_side_move;
}

float math::SE::get_fov_difference(ang3_t angle, vec3_t start, vec3_t end)
{
	ang3_t aim_angle{};
	vec3_t view_position{}, aim_position{};

	const vec3_t delta = end - start;

	VectorAngles(delta, aim_angle);

	AngleVectors(angle, view_position);
	AngleVectors(aim_angle, aim_position);

	const float aimplsq = aim_position.Length2d();
	const auto dotp = static_cast<float>(DotProduct(aim_position, view_position) / aimplsq);

	const float vcos = acos(dotp);
	const float drad = math::rad2deg(vcos);

	if (!isnan(vcos))
		return drad;

	return 0.0f;
}

FLOAT math::SE::VectorNormalize(vec3_t& vec) {
	FLOAT radius = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	FLOAT iradius = 1.f / (radius + FLT_EPSILON);

	vec.x *= iradius;
	vec.y *= iradius;
	vec.z *= iradius;

	return radius;
}

FLOAT math::SE::AngleNormalize(FLOAT angle)
{
	angle = std::fmodf(angle, 360.0f);

	if (angle > 180)
		angle -= 360;

	if (angle < -180)
		angle += 360;

	return angle;
}

FLOAT math::SE::AngleNormalizePositive(FLOAT angle)
{
	angle = std::fmodf(angle, 360.0f);

	if (angle < 0.0f)
		angle += 360.0f;

	return angle;
}

VOID math::SE::AngleNormalize(ang3_t& vec) {
	vec.x = std::clamp(vec.x, -89.f, +89.f);
	vec.y = remainderf(vec.y, 360.f);
	vec.z = 0.f;
}

void math::vector_transform(vec3_t& in, const matrix3x4& matrix, vec3_t& out)
{
	const auto in_array = in.get_base();

	out.x = dot_product(in_array, matrix[0]) + matrix[0][3];
	out.y = dot_product(in_array, matrix[1]) + matrix[1][3];
	out.z = dot_product(in_array, matrix[2]) + matrix[2][3];
}

math::vec3_t math::vec3_t::Normalized()
{
	auto temp = *this;

	NormalizeVector(temp);

	return temp;
}

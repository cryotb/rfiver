#pragma once

#pragma warning(disable : 4312 4311 4302)

namespace math
{
	template < typename _t >
	struct floating_point_holder2_t {
		_t x;
		_t y;
	};

	template < typename _t >
	struct floating_point_holder3_t {
		_t x;
		_t y;
		_t z;
	};

	template < typename _t >
	struct floating_point_holder4_t {
		_t x;
		_t y;
		_t z;
		_t h;
	};

	class vec3_t
	{
	public:
		float* get_base()
		{
			return reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this));
		}

		void init(float xx, float xy, float xz) { x = xx; y = xy; z = xz; }

		bool operator==(const vec3_t& other) const
		{
			return (other.x == x && other.y == y && other.z == z);
		}

		vec3_t operator-(const vec3_t& other) const
		{
			return vec3_t{ this->x - other.x, this->y - other.y, this->z - other.z };
		}

		vec3_t operator+(const vec3_t& other) const
		{
			return vec3_t{ this->x + other.x, this->y + other.y, this->z + other.z };
		}

		vec3_t& operator*=(const float multiplier)
		{
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;

			return *this;
		}

		vec3_t operator*(const vec3_t& other) const
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		vec3_t operator*(const float& other) const
		{
			return { x * other, y * other, z * other };
		}

		[[nodiscard]] FLOAT Length() CONST {
			return (x + y + z);
		}

		[[nodiscard]] FLOAT Length2d() CONST {
			return sqrtf(x * x + y * y + z * z);
		}

		[[nodiscard]] FLOAT LengthSquare() CONST {
			return (x * x + y * y + z * z);
		}

		[[nodiscard]] BOOL Empty() CONST {
			return (Length() <= 0.0f);
		}

		[[nodiscard]] FLOAT DistanceTo(vec3_t& vOther) CONST
		{
			CONST AUTO delta = (vOther - *this);

			return fabsf(delta.LengthSquare());
		}

		[[nodiscard]] FLOAT DistanceTo(CONST vec3_t& vOther) CONST
		{
			CONST AUTO delta = (vOther - *this);

			return fabsf(delta.LengthSquare());
		}

		[[nodiscard]] floating_point_holder3_t<FLOAT> Data() {
			return { x, y, z };
		}

		vec3_t Normalized();
	public:
		float x, y, z;
	protected:
	private:
	};

	class __declspec(align(16)) VectorAligned3 : public vec3_t
	{
	public:
		inline VectorAligned3(void) {};
		inline VectorAligned3(float X, float Y, float Z)
		{
			init(X, Y, Z);
		}

		void Zero()
		{
			init(0, 0, 0);
		}

	public:
		explicit VectorAligned3(const vec3_t& vOther)
		{
			init(vOther.x, vOther.y, vOther.z);
		}

		VectorAligned3& operator=(const vec3_t& vOther)
		{
			init(vOther.x, vOther.y, vOther.z);
			return *this;
		}

		VectorAligned3& operator=(const VectorAligned3& vOther)
		{
			init(vOther.x, vOther.y, vOther.z);
			return *this;
		}

		float w;
	};

	class ang3_t {
	public:
		bool operator==(const ang3_t& other) const
		{
			return (other.x == x && other.y == y && other.z == z);
		}

		ang3_t operator-(const ang3_t& other) const
		{
			return ang3_t{ this->x - other.x, this->y - other.y, this->z - other.z };
		}

		ang3_t operator+(const ang3_t& other) const
		{
			return ang3_t{ this->x + other.x, this->y + other.y, this->z + other.z };
		}

		ang3_t& operator*=(const float multiplier)
		{
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;

			return *this;
		}

		ang3_t operator*(const ang3_t& other) const
		{
			return { x * other.x, y * other.y, z * other.z };
		}

		ang3_t operator*(const float& other) const
		{
			return { x * other, y * other, z * other };
		}

		[[nodiscard]] floating_point_holder3_t<FLOAT> Data() {
			return { x, y, z };
		}

		[[nodiscard]] FLOAT Length() CONST {
			return (x + y + z);
		}

		[[nodiscard]] FLOAT Length2d() CONST {
			return sqrtf(x * x + y * y + z * z);
		}

		[[nodiscard]] FLOAT LengthSquare() CONST {
			return (x * x + y * y + z * z);
		}

		[[nodiscard]] BOOL Empty() CONST {
			return (Length() <= 0.0f);
		}

		[[nodiscard]] FLOAT DistanceTo(ang3_t& vOther) CONST
		{
			CONST AUTO delta = (vOther - *this);

			return fabsf(delta.LengthSquare());
		}

		[[nodiscard]] FLOAT DistanceTo(CONST ang3_t& vOther) CONST
		{
			CONST AUTO delta = (vOther - *this);

			return fabsf(delta.LengthSquare());
		}
	public:
		float x, y, z;
	protected:
	private:
	};

	class Vector4 : public vec3_t
	{
	public:
	public:
		void init(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		Vector4() { }

		Vector4(float _x, float _y, float _z, float _w)
		{
			init(_x, _y, _z, _w);
		}

		float w;
	protected:
	private:
	};

	struct matrix3x4
	{
		float* operator[](int i)
		{
			return m_value[i];
		}

		const float* operator[](int i) const
		{
			return m_value[i];
		}

		float* Base()
		{
			return &m_value[0][0];
		}

		const float* Base() const
		{
			return &m_value[0][0];
		}

		float m_value[3][4];
	};

	struct matrix4x4
	{
		float* operator[](int i)
		{
			return m_value[i];
		}

		const float* operator[](int i) const
		{
			return m_value[i];
		}

		float* Base()
		{
			return &m_value[0][0];
		}

		const float* Base() const
		{
			return &m_value[0][0];
		}

		float m_value[4][4];
	};

	struct __declspec(align(16)) matrix3x4a : matrix3x4
	{
	};

	extern float deg2rad(const float v);
	extern float rad2deg(const float v);
	extern void sin_cos(float radians, float* sine, float* cosine);

	extern void VectorAngles(const math::vec3_t& forward, math::ang3_t& angles);
	extern void AngleVectors(const math::ang3_t& in, math::vec3_t& out);
	extern void ClampAngle(vec3_t& angle);
	extern void ClampAngle(ang3_t& angle);

	extern void NormalizeVector(math::vec3_t& in);
	extern void NormalizeAngle(math::ang3_t& angle);

	extern float DotProduct(const math::vec3_t& first, const math::vec3_t& second);
	extern float dot_product(const float* first, const float* second);
	extern void vector_transform(vec3_t& in, const matrix3x4& matrix, vec3_t& out);

	namespace SE {
		extern FLOAT VectorNormalize(vec3_t& vec);
		extern FLOAT AngleNormalize(FLOAT angle);
		extern FLOAT AngleNormalizePositive(FLOAT angle);
		extern VOID AngleNormalize(ang3_t & vec);

		extern float get_fov_difference(ang3_t angle, vec3_t start, vec3_t end);
	}

	__inline_block Vector4 ToVector4(vec3_t vec, float w)
	{
		return Vector4(vec.x, vec.y, vec.z, w);
	}

	extern void fix_movement(float& forward_move, float& side_move, math::ang3_t view_angle, math::ang3_t previous_angle,
		const float old_forward_move, const float old_side_move);
}



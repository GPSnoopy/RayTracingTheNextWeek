#pragma once

#define VEC3_AVX
#ifdef VEC3_AVX
#include <immintrin.h>
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include <algorithm>
#include <iostream>

class Vec3 final
{
public:

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3(Vec3&&) = default;
	~Vec3() = default;
	Vec3& operator = (const Vec3&) = default;
	Vec3& operator = (Vec3&&) = default;

#ifdef VEC3_AVX

	explicit Vec3(__m128 v) : v_(v) { }
	explicit Vec3(const float a) : v_(_mm_set1_ps(a)) { }
	Vec3(const float x, const float y, const float z) : v_(_mm_setr_ps(x, y, z, 0)) { }

	__m128 v() const { return v_; }
	
	float x() const { return _mm_cvtss_f32(_mm_shuffle_ps(v_, v_, _MM_SHUFFLE(0, 0, 0, 0))); }
	float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(v_, v_, _MM_SHUFFLE(0, 0, 0, 1))); }
	float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(v_, v_, _MM_SHUFFLE(0, 0, 0, 2))); }

	float r() const { return x(); }
	float g() const { return y(); }
	float b() const { return x(); }

	Vec3 const& operator + () const { return *this; }
	Vec3 operator - () const { return Vec3(_mm_sub_ps(_mm_setzero_ps(), v_)); }

	Vec3& operator += (const Vec3& v) { v_ = _mm_add_ps(v_, v.v_); return *this; }
	Vec3& operator -= (const Vec3& v) { v_ = _mm_sub_ps(v_, v.v_); return *this; }
	Vec3& operator *= (const Vec3& v) { v_ = _mm_mul_ps(v_, v.v_); return *this; }
	Vec3& operator /= (const Vec3& v) { v_ = _mm_div_ps(v_, v.v_); return *this; }

	Vec3& operator += (const float s) { *this += Vec3(s); return *this; }
	Vec3& operator -= (const float s) { *this -= Vec3(s); return *this; }
	Vec3& operator *= (const float s) { *this *= Vec3(s); return *this; }
	Vec3& operator /= (const float s) { *this /= Vec3(s); return *this; }

#else

	explicit Vec3(const float a) : v_{ a, a, a } { }
	Vec3(const float x, const float y, const float z) : v_{ x, y, z } { }

	float x() const { return v_[0]; }
	float y() const { return v_[1]; }
	float z() const { return v_[2]; }

	float r() const { return v_[0]; }
	float g() const { return v_[1]; }
	float b() const { return v_[2]; }

	//float operator [] (const int i) const { return v_[i]; }

	Vec3 const& operator + () const { return *this; }
	Vec3 operator - () const { return { -v_[0], -v_[1], -v_[2] }; }

	Vec3& operator += (const Vec3& v) { v_[0] += v.v_[0]; v_[1] += v.v_[1]; v_[2] += v.v_[2]; return *this; }
	Vec3& operator -= (const Vec3& v) { v_[0] -= v.v_[0]; v_[1] -= v.v_[1]; v_[2] -= v.v_[2]; return *this; }
	Vec3& operator *= (const Vec3& v) { v_[0] *= v.v_[0]; v_[1] *= v.v_[1]; v_[2] *= v.v_[2]; return *this; }
	Vec3& operator /= (const Vec3& v) { v_[0] /= v.v_[0]; v_[1] /= v.v_[1]; v_[2] /= v.v_[2]; return *this; }

	Vec3& operator += (const float s) { v_[0] += s; v_[1] += s; v_[2] += s; return *this; }
	Vec3& operator -= (const float s) { v_[0] -= s; v_[1] -= s; v_[2] -= s; return *this; }
	Vec3& operator *= (const float s) { v_[0] *= s; v_[1] *= s; v_[2] *= s; return *this; }
	Vec3& operator /= (const float s) { v_[0] /= s; v_[1] /= s; v_[2] /= s; return *this; }

#endif

private:

#ifdef VEC3_AVX
	__m128 v_;
#else
	float v_[3];
#endif
};

inline Vec3 operator + (const Vec3& left, const Vec3& right) { return Vec3(left) += right; }
inline Vec3 operator - (const Vec3& left, const Vec3& right) { return Vec3(left) -= right; }
inline Vec3 operator * (const Vec3& left, const Vec3& right) { return Vec3(left) *= right; }
inline Vec3 operator / (const Vec3& left, const Vec3& right) { return Vec3(left) /= right; }

inline Vec3 operator + (const Vec3& left, const float right) { return Vec3(left) += right; }
inline Vec3 operator - (const Vec3& left, const float right) { return Vec3(left) -= right; }
inline Vec3 operator * (const Vec3& left, const float right) { return Vec3(left) *= right; }
inline Vec3 operator / (const Vec3& left, const float right) { return Vec3(left) *= (1.0f / right); }

inline Vec3 operator + (const float left, const Vec3& right) { return Vec3(left) + right; }
inline Vec3 operator - (const float left, const Vec3& right) { return Vec3(left) - right; }
inline Vec3 operator * (const float left, const Vec3& right) { return Vec3(left) * right; }
inline Vec3 operator / (const float left, const Vec3& right) { return Vec3(left) / right; }

inline float Min(const Vec3& v)
{
	return std::min(v.x(), std::min(v.y(), v.z()));
}

inline float Max(const Vec3& v)
{
	return std::max(v.x(), std::max(v.y(), v.z()));
}

inline Vec3 Min(const Vec3& l, const Vec3& r)
{
#ifdef VEC3_AVX
	return Vec3(_mm_min_ps(l.v(), r.v()));
#else
	return { std::min(l.x(), r.x()), std::min(l.y(), r.y()), std::min(l.z(), r.z()) };
#endif
}

inline Vec3 Max(const Vec3& l, const Vec3& r)
{
#ifdef VEC3_AVX
	return Vec3(_mm_max_ps(l.v(), r.v()));
#else
	return { std::max(l.x(), r.x()), std::max(l.y(), r.y()), std::max(l.z(), r.z()) };
#endif
}

inline float Dot(const Vec3& l, const Vec3& r)
{
#ifdef VEC3_AVX
	return _mm_cvtss_f32(_mm_dp_ps(l.v(), r.v(), 0x77));
#else
	return l.x()*r.x() + l.y()*r.y() + l.z()*r.z();
#endif
}

inline Vec3 Cross(const Vec3& l, const Vec3& r)
{
#ifdef VEC3_AVX
	const __m128 vec0 = l.v();
	const __m128 vec1 = r.v();
	const __m128 tmp0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
	const __m128 tmp1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
	const __m128 tmp2 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 1, 0, 2));
	const __m128 tmp3 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 0, 2, 1));
	return Vec3(_mm_fmsub_ps(tmp0, tmp1, _mm_mul_ps(tmp2, tmp3)));
#else
	return Vec3
	{
		l.y()*r.z() - l.z()*r.y(),
		l.z()*r.x() - l.x()*r.z(),
		l.x()*r.y() - l.y()*r.x()
	};
#endif
}

inline Vec3 Clamp(const Vec3& v, const Vec3& min, const Vec3 max)
{
#ifdef VEC3_AVX
	return Vec3(_mm_min_ps(_mm_max_ps(v.v(), min.v()), max.v()));
#else
	return { std::clamp(v.x(), min.x(), max.x()), std::clamp(v.y(), min.y(), max.y()), std::clamp(v.z(), min.z(), max.z()) };
#endif
}

inline Vec3 Floor(const Vec3& v)
{
#ifdef VEC3_AVX
	return Vec3(_mm_floor_ps(v.v()));
#else
	return { std::floor(v.x()), std::floor(v.y()), std::floor(v.z()) };
#endif
}

inline float SquaredLength(const Vec3& v)
{
	return Dot(v, v);
}

inline float Length(const Vec3& v)
{
	return std::sqrt(SquaredLength(v));
}

inline Vec3 Sqrt(const Vec3& v)
{
#ifdef VEC3_AVX
	return Vec3(_mm_sqrt_ps(v.v()));
#else
	return { std::sqrt(v.x()), std::sqrt(v.y()), std::sqrt(v.z()) };
#endif
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / Length(v);
}

inline std::istream& operator >> (std::istream& in, Vec3& v)
{
	float x, y, z;
	in >> x >> y >> z;
	v = Vec3(x, y, z);
	return in;
}

inline std::ostream& operator << (std::ostream& out, const Vec3& v)
{
	return out << v.x() << " " << v.y() << " " << v.z();
}

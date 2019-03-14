#pragma once

#include "Ray.h"

// Axis Aligned Bounding Box
class Aabb final
{
public:

	Aabb(const Aabb&) = default;
	Aabb(Aabb&&) = default;
	~Aabb() = default;
	Aabb& operator = (const Aabb&) = default;
	Aabb& operator = (Aabb&&) = default;

	Aabb() :
		Min(std::numeric_limits<float>::max()),
		Max(-std::numeric_limits<float>::max())
	{
	}

	Aabb(const Vec3& min, const Vec3& max) : 
		Min(min), Max(max)
	{
	}

	Aabb(const Aabb& left, const Aabb& right) : 
		Min(::Min(left.Min, right.Min)), 
		Max(::Max(left.Max, right.Max))
	{
		
	}

	Vec3 Min;
	Vec3 Max;

	bool Hit(const Ray& ray, float tMin, float tMax) const
	{
		const Vec3 invD = 1.0f / ray.Direction;
		const Vec3 t0 = (Min - ray.Origin) * invD;
		const Vec3 t1 = (Max - ray.Origin) * invD;

		tMin = std::max(tMin, ::Max(::Min(t0, t1)));
		tMax = std::min(tMax, ::Min(::Max(t0, t1)));

		return tMax > tMin;
	}
};

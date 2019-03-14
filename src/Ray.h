#pragma once

#include "Vec3.h"

class Ray final
{
public:

	Ray(const Ray&) = default;
	Ray(Ray&&) = default;
	~Ray() = default;
	Ray& operator = (const Ray&) = delete;
	Ray& operator = (Ray&&) = delete;

	Ray(const Vec3& origin, const Vec3& direction, const float time = 0.0f)
		: Origin(origin), Direction(direction), Time(time)
	{
	}

	const Vec3 Origin;
	const Vec3 Direction;
	const float Time;

	Vec3 PointAtParameter(const float t) const { return Origin + t * Direction; }
};
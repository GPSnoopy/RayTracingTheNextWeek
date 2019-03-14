#pragma once

#include "Hittable.h"
#include "Materials/Material.h"
#include <stdexcept>

class RectangleXY final : public Hittable
{
public:

	RectangleXY(const float x0, const float x1, const float y0, const float y1, const float z, const Material* const material) :
		RectangleXY(x0, x1, y0, y1, z, MaterialPtr(material))
	{
	}

	RectangleXY(const float x0, const float x1, const float y0, const float y1, const float z, MaterialPtr material) :
		x0(x0), x1(x1), y0(y0), y1(y1), z(z), Material(std::move(material))
	{
		if (x0 > x1) throw std::invalid_argument("x0 > x1");
		if (y0 > y1) throw std::invalid_argument("y0 > y1");
	}

	const float x0;
	const float x1;
	const float y0;
	const float y1;
	const float z;
	const MaterialPtr Material;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random&) const override
	{
		const float t = (z - ray.Origin.z()) / ray.Direction.z();
		
		if (t < tMin || t > tMax)
		{
			return std::nullopt;
		}

		const float x = ray.Origin.x() + t * ray.Direction.x();
		const float y = ray.Origin.y() + t * ray.Direction.y();

		if (x < x0 || x > x1 || y < y0 || y > y1)
		{
			return std::nullopt;
		}

		return HitRecord(
			t,
			(x - x0) / (x1 - x0),
			(y - y0) / (y1 - y0),
			ray.PointAtParameter(t),
			Vec3(0, 0, 1),
			Material.get()
		);
	}

	Aabb BoundingBox(float, float) const override
	{
		return Aabb(Vec3(x0, y0, z - 0.0001f), Vec3(x1, y1, z + 0.0001f));
	}
};

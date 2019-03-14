#pragma once

#include "Hittable.h"
#include "Materials/Material.h"
#include <stdexcept>

class RectangleYZ final : public Hittable
{
public:

	RectangleYZ(const float y0, const float y1, const float z0, const float z1, const float x, const Material* const material) :
		RectangleYZ(y0, y1, z0, z1, x, MaterialPtr(material))
	{
	}

	RectangleYZ(const float y0, const float y1, const float z0, const float z1, const float x, const MaterialPtr& material) :
		y0(y0), y1(y1), z0(z0), z1(z1), x(x), Material(material)
	{
		if (y0 > y1) throw std::invalid_argument("y0 > y1");
		if (z0 > z1) throw std::invalid_argument("z0 > z1");
	}

	const float y0;
	const float y1;
	const float z0;
	const float z1;
	const float x;
	const MaterialPtr Material;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random&) const override
	{
		const float t = (x - ray.Origin.x()) / ray.Direction.x();

		if (t < tMin || t > tMax)
		{
			return std::nullopt;
		}

		const float y = ray.Origin.y() + t * ray.Direction.y();
		const float z = ray.Origin.z() + t * ray.Direction.z();

		if (y < y0 || y > y1 || z < z0 || z > z1)
		{
			return std::nullopt;
		}

		return HitRecord(
			t,
			(y - y0) / (y1 - y0),
			(z - z0) / (z1 - z0),
			ray.PointAtParameter(t),
			Vec3(1, 0, 0),
			Material.get()
		);
	}

	Aabb BoundingBox(float, float) const override
	{
		return Aabb(Vec3(x - 0.0001f, y1, z0), Vec3(x + 0.0001f, y1, z1));
	}
};

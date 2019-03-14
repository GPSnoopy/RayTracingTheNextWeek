#pragma once

#include "Hittable.h"
#include "Materials/Material.h"
#include <stdexcept>

class RectangleXZ final : public Hittable
{
public:

	RectangleXZ(const float x0, const float x1, const float z0, const float z1, const float y, const Material* const material) :
		RectangleXZ(x0, x1, z0, z1, y, MaterialPtr(material))
	{
	}

	RectangleXZ(const float x0, const float x1, const float z0, const float z1, const float y, const MaterialPtr& material) :
		x0(x0), x1(x1), z0(z0), z1(z1), y(y), Material(material)
	{
		if (x0 > x1) throw std::invalid_argument("x0 > x1");
		if (z0 > z1) throw std::invalid_argument("z0 > z1");
	}

	const float x0;
	const float x1;
	const float z0;
	const float z1;
	const float y;
	const MaterialPtr Material;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random&) const override
	{
		const float t = (y - ray.Origin.y()) / ray.Direction.y();

		if (t < tMin || t > tMax)
		{
			return std::nullopt;
		}

		const float x = ray.Origin.x() + t * ray.Direction.x();
		const float z = ray.Origin.z() + t * ray.Direction.z();

		if (x < x0 || x > x1 || z < z0 || z > z1)
		{
			return std::nullopt;
		}

		return HitRecord(
			t,
			(x - x0) / (x1 - x0),
			(z - z0) / (z1 - z0),
			ray.PointAtParameter(t),
			Vec3(0, 1, 0),
			Material.get()
		);
	}

	Aabb BoundingBox(float, float) const override
	{
		return Aabb(Vec3(x0, y - 0.0001f, z0), Vec3(x1, y + 0.0001f, z1));
	}
};

#pragma once

#include "Hittable.h"
#include "Materials/Material.h"

class SphereMoving final : public Hittable
{
public:

	SphereMoving(
		const Vec3& center0, const Vec3& center1,
		const float time0, const float time1,
		const float radius, const Material* const material) :
		Center0(center0),
		Center1(center1),
		Time0(time0),
		Time1(time1),
		Radius(radius), 
		Material(material)
	{
	}

	const Vec3 Center0;
	const Vec3 Center1;
	const float Time0;
	const float Time1;
	const float Radius;
	const MaterialPtr Material;

	Vec3 Center(const float time) const
	{
		return Center0 + ((time - Time0) / (Time1 - Time0))*(Center1 - Center0);
	}

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random&) const override
	{
		// https://en.wikipedia.org/wiki/Quadratic_formula

		const Vec3 center = Center(ray.Time);
		const Vec3 oc = ray.Origin - center;
		const float a = Dot(ray.Direction, ray.Direction);
		const float b = Dot(oc, ray.Direction);
		const float c = Dot(oc, oc) - Radius * Radius;
		const float discriminant = b * b - a * c;

		if (discriminant >= 0)
		{
			const float t1 = (-b - std::sqrt(discriminant)) / a;
			const float t2 = (-b + std::sqrt(discriminant)) / a;

			if ((tMin <= t1 && t1 < tMax) || (tMin <= t2 && t2 < tMax))
			{
				const float t = (tMin <= t1 && t1 < tMax) ? t1 : t2;
				const Vec3 point = ray.PointAtParameter(t);
				const Vec3 normal = (point - center) / Radius;
				const Sphere::uv texCoord = Sphere::GetSphereUv(normal);

				return HitRecord(t, texCoord.u, texCoord.v, point, normal, Material.get());
			}
		}

		return std::nullopt;
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		const Aabb aabb0(Center(time0) - Radius, Center(time1) + Radius);
		const Aabb aabb1(Center(time1) - Radius, Center(time1) + Radius);

		return Aabb(aabb0, aabb1);
	}
};

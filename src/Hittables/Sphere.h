#pragma once

#include "Hittable.h"
#include "Materials/Material.h"

class Sphere final : public Hittable
{
public:

	Sphere(const Vec3& center, const float radius, const Material* const material) :
		Sphere(center, radius, MaterialPtr(material))
	{
	}

	Sphere(const Vec3& center, const float radius, MaterialPtr material) :
		Center(center), Radius(radius), Material(std::move(material))
	{
	}

	const Vec3 Center;
	const float Radius;
	const MaterialPtr Material;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random&) const override
	{
		// https://en.wikipedia.org/wiki/Quadratic_formula

		const Vec3 oc = ray.Origin - Center;
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
				const Vec3 normal = (point - Center) / Radius;
				const uv texCoord = GetSphereUv(normal);

				return HitRecord(t, texCoord.u, texCoord.v, point, normal, Material.get());
			}
		}

		return std::nullopt;
	}


	Aabb BoundingBox(const float, const float) const override
	{
		return Aabb(Center - Radius, Center + Radius);
	}

private:

	friend class SphereMoving;
	struct uv { float u; float v; };

	static uv GetSphereUv(const Vec3& point)
	{
		const float phi = std::atan2(point.z(), point.x());
		const float theta = std::asin(point.y());
		const float pi = M_PI;

		return
		{
			1 - (phi + pi) / (2* pi),
			(theta + pi /2) / pi
		};
	}
};

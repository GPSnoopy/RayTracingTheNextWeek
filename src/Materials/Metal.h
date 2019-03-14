#pragma once

#include "Material.h"

class Metal final : public Material
{
public:

	Metal(const Vec3& albedo, const float fuzz)
		: Albedo(albedo), Fuzz(std::clamp(fuzz, 0.0f, 1.0f))
	{
	}

	const Vec3 Albedo;
	const float Fuzz;

	std::optional<Scattered> Scatter(const Ray& ray, const HitRecord& record, Random& random) const override
	{
		const Vec3 reflected = Reflect(UnitVector(ray.Direction), record.Normal);

		return Dot(reflected, record.Normal) > 0
			? Scattered(Ray(record.Point, reflected + Fuzz*RandomInUnitSphere(random), ray.Time), Albedo)
			: std::optional<Scattered>();
	}
};

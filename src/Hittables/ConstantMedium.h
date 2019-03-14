#pragma once

#include "Hittable.h"
#include "Materials/Isotropic.h"
#include "Materials/Material.h"
#include "Textures/Texture.h"

class ConstantMedium final : public Hittable
{
public:

	ConstantMedium(HittablePtr boundary, const float density, const Texture* const texture) :
		Boundary(std::move(boundary)), Density(density), PhaseFunction(new Isotropic(texture)) { }

	const HittablePtr Boundary;
	const float Density;
	const MaterialPtr PhaseFunction;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random& random) const override
	{
		const auto rec1 = Boundary->Hit(ray, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), random);
		if (rec1)
		{
			const auto rec2 = Boundary->Hit(ray, rec1->t + 0.0001, std::numeric_limits<float>::max(), random);
			if (rec2)
			{
				float t1 = std::max(rec1->t, tMin);
				const float t2 = std::min(rec2->t, tMax);

				if (t1 >= t2)
				{
					return std::nullopt;
				}

				t1 = std::max(t1, 0.0f);

				const float distanceInsideBoundary = (t2 - t1) * Length(ray.Direction);
				const float hitDistance = -(1 / Density)*std::log(Uniform(random));

				if (hitDistance < distanceInsideBoundary)
				{
					const float t = t1 + hitDistance / Length(ray.Direction);
					const Vec3 point = ray.PointAtParameter(t);
					const Vec3 normal = Vec3(1, 0, 0); // arbitrary

					return HitRecord(t, 0, 0, point, normal, PhaseFunction.get());
				}
			}
		}

		return std::nullopt;
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		return Boundary->BoundingBox(time0, time1);
	}
};

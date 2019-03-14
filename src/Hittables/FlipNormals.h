#pragma once

#include "Hittable.h"
#include "Materials/Material.h"

class FlipNormals final : public Hittable
{
public:

	explicit FlipNormals(const Hittable* const hittable) :
		Hittable(hittable)
	{
	}

	const HittablePtr Hittable;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random& random) const override
	{
		const auto record = Hittable->Hit(ray, tMin, tMax, random);
		return record
			? HitRecord(record->t, record->u, record->v, record->Point, -record->Normal, record->Material)
			: std::optional<HitRecord>();
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		return Hittable->BoundingBox(time0, time1);
	}
};

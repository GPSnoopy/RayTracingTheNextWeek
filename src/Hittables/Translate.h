#pragma once

#include "Hittable.h"

class Translate final : public Hittable
{
public:

	Translate(const Hittable* const hittable, const Vec3& offset) :
		Translate(HittablePtr(hittable), offset) { }

	Translate(HittablePtr hittable, const Vec3& offset) :
		Hittable(std::move(hittable)), Offset(offset) { }

	const HittablePtr Hittable;
	const Vec3 Offset;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random& random) const override
	{
		const auto record = Hittable->Hit(Ray(ray.Origin - Offset, ray.Direction, ray.Time), tMin, tMax, random);
		return record
			? HitRecord(record->t, record->u, record->v, record->Point + Offset, record->Normal, record->Material)
			: std::optional<HitRecord>();
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		const auto aabb = Hittable->BoundingBox(time0, time1);
		return Aabb(aabb.Min + Offset, aabb.Max + Offset);
	}
};

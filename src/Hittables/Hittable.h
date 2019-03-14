#pragma once

#include "Aabb.h"
#include "HitRecord.h"
#include "Ray.h"
#include <memory>
#include <optional>
#include <utility>

class Hittable
{
public:

	Hittable() = default;
	Hittable(Hittable&&) = default;
	virtual ~Hittable() = default;

	Hittable(const Hittable&) = delete;
	Hittable& operator = (const Hittable&) = delete;
	Hittable& operator = (Hittable&&) = delete;

	virtual std::optional<HitRecord> Hit(const Ray& ray, float tMin, float tMax, Random& random) const = 0;
	virtual Aabb BoundingBox(float time0, float time1) const = 0;
};

typedef std::shared_ptr<const Hittable> HittablePtr;

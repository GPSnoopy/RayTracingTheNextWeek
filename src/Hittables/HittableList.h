#pragma once

#include "Hittable.h"
#include <memory>
#include <vector>

class HittableList final : public Hittable
{
public:

	HittableList() = default;

	explicit HittableList(std::vector<HittablePtr>&& list) :
		list_(std::move(list))
	{
	}

	void Add(const Hittable* hittable)
	{
		Add(HittablePtr(hittable));
	}

	void Add(HittablePtr hittable)
	{
		list_.push_back(std::move(hittable));
	}

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, float tMax, Random& random) const override
	{
		std::optional<HitRecord> record;

		for (const auto& hittable : list_)
		{
			if (const auto rec = hittable->Hit(ray, tMin, tMax, random))
			{
				tMax = rec->t;
				record = rec;
			}
		}

		return record;
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		Aabb aabb;

		for (const auto& hittable : list_)
		{
			aabb = Aabb(aabb, hittable->BoundingBox(time0, time1));
		}

		return aabb;
	}

private:

	std::vector<HittablePtr> list_;
};
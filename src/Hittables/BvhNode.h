#pragma once

#include "Hittable.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

// Bounding Volume Hierarchy
class BvhNode final : public Hittable
{
public:

	template <class Iterator>
	static HittablePtr Create(
		Iterator begin, Iterator end,
		const float time0, const float time1, const int depth = 0)
	{
		const auto distance = std::distance(begin, end);

		if (distance == 0)
		{
			throw std::invalid_argument("empty iterator range");
		}

		if (distance == 1)
		{
			return std::move(*begin);
		}

		if (distance == 2)
		{
			return HittablePtr(new BvhNode(*begin, *(begin + 1), time0, time1));
		}

		const int axis = depth % 3;//std::uniform_int_distribution(0, 2)(random);
		std::sort(begin, end, [=](const HittablePtr& left, const HittablePtr& right) { return Ordering(*left, *right, time0, time1, axis); });

		return HittablePtr(new BvhNode(
			Create(begin, begin + distance / 2, time0, time1, depth + 1), 
			Create(begin + distance / 2, end, time0, time1, depth + 1),
			time0, time1));
	}

	const HittablePtr Left;
	const HittablePtr Right;
	const float Time0;
	const float Time1;
	const Aabb Box;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random& random) const override
	{
		if (Box.Hit(ray, tMin, tMax))
		{
			const auto left = Left->Hit(ray, tMin, tMax, random);
			const auto right = Right->Hit(ray, tMin, tMax, random);

			if (left && right)
			{
				return left->t < right->t ? left : right;
			}

			return left ? left : right;
		}

		return std::nullopt;
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		if (time0 != Time0) throw std::invalid_argument("invalid time0");
		if (time1 != Time1) throw std::invalid_argument("invalid time1");

		return Box;
	}

private:

	BvhNode(HittablePtr left, HittablePtr right, const float time0, const float time1) :
		Left(std::move(left)), Right(std::move(right)), 
		Time0(time0), Time1(time1), 
		Box(Left->BoundingBox(time0, time1), Right->BoundingBox(time0, time1))
	{
	}

	static bool Ordering(const Hittable& left, const Hittable& right, const float time0, const float time1, const int axis)
	{
		switch (axis)
		{
		case 0: return left.BoundingBox(time0, time1).Min.x() < right.BoundingBox(time0, time1).Min.x();
		case 1: return left.BoundingBox(time0, time1).Min.y() < right.BoundingBox(time0, time1).Min.y();
		case 2: return left.BoundingBox(time0, time1).Min.z() < right.BoundingBox(time0, time1).Min.z();
		default: throw std::out_of_range("axis");
		}
	}

};

#pragma once

#include "Hittable.h"

class RotateY final : public Hittable
{
public:

	RotateY(const Hittable* const hittable, const float angle) :
		RotateY(HittablePtr(hittable), angle) { }

	RotateY(HittablePtr hittable, const float angle) :
		Hittable(std::move(hittable)), 
		Angle(angle),
		sinTheta_(std::sin(float(M_PI / 180) * angle)),
		cosTheta_(std::cos(float(M_PI / 180) * angle))
	{
	}

	const HittablePtr Hittable;
	const float Angle;

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random& random) const override
	{
		const Ray rotated(
			Vec3(
				cosTheta_ * ray.Origin.x() - sinTheta_ * ray.Origin.z(),
				ray.Origin.y(),
				sinTheta_*ray.Origin.x() + cosTheta_ * ray.Origin.z()),
			Vec3(
				cosTheta_ * ray.Direction.x() - sinTheta_ * ray.Direction.z(),
				ray.Direction.y(),
				sinTheta_*ray.Direction.x() + cosTheta_ * ray.Direction.z()),
			ray.Time
		);

		const auto record = Hittable->Hit(rotated, tMin, tMax, random);
		if (record)
		{
			const Vec3 point(
				cosTheta_ * record->Point.x() + sinTheta_ * record->Point.z(),
				record->Point.y(),
				-sinTheta_ * record->Point.x() + cosTheta_ * record->Point.z()
			);

			const Vec3 normal(
				cosTheta_ * record->Normal.x() + sinTheta_ * record->Normal.z(),
				record->Normal.y(),
				-sinTheta_ * record->Normal.x() + cosTheta_ * record->Normal.z()
			);

			return HitRecord(record->t, record->u, record->v, point, normal, record->Material);
		}

		return std::nullopt;
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		const auto box = Hittable->BoundingBox(time0, time1);

		Vec3 min(std::numeric_limits<float>::max());
		Vec3 max(-std::numeric_limits<float>::max());

		for (int i = 0; i != 2; ++i)
		{
			for (int j = 0; j != 2; ++j)
			{
				for (int k = 0; k != 2; ++k)
				{
					const Vec3 selector(i, j, k);
					const Vec3 point = selector * box.Max + (1 - selector)*box.Min;
					const float x = cosTheta_ * point.x() + sinTheta_ * point.z();
					const float z = -sinTheta_ * point.x() + cosTheta_ * point.z();
					const Vec3 rotatedPoint(x, point.y(), z);

					min = Min(min, rotatedPoint);
					max = Max(max, rotatedPoint);
				}
			}
		}

		return Aabb(min, max);
	}

private:

	const float sinTheta_;
	const float cosTheta_;
};

#pragma once

#include "HittableList.h"
#include "FlipNormals.h"
#include "RectangleXY.h"
#include "RectangleXZ.h"
#include "RectangleYZ.h"

class Box final : public Hittable
{
public:

	Box(const Vec3& p0, const Vec3& p1, const Material* const material) :
		Box(p0, p1, MaterialPtr(material)) { }

	Box(const Vec3& p0, const Vec3& p1, const MaterialPtr& material) :
		rectangles_(CreateRectangles(p0, p1, material)) { }

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, const float tMax, Random& random) const override
	{
		return rectangles_.Hit(ray, tMin, tMax, random);
	}

	Aabb BoundingBox(const float time0, const float time1) const override
	{
		return rectangles_.BoundingBox(time0, time1);
	}

private:

	static HittableList CreateRectangles(const Vec3& p0, const Vec3& p1, const MaterialPtr& material)
	{
		HittableList list;

		list.Add(                new RectangleXY(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
		list.Add(new FlipNormals(new RectangleXY(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material)));
		list.Add(                new RectangleXZ(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
		list.Add(new FlipNormals(new RectangleXZ(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material)));
		list.Add(                new RectangleYZ(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
		list.Add(new FlipNormals(new RectangleYZ(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material)));

		return list;
	}

	const HittableList rectangles_;
};
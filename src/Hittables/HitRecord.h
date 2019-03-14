#pragma once

#include "Vec3.h"

class Material;

struct HitRecord final
{
	HitRecord(const float t, const float u, const float v, const Vec3& point, const Vec3& normal, const Material* material) :
		t(t), u(u), v(v), Point(point), Normal(normal), Material(material)
	{
	}

	float t; // t parameter
	float u; // u texture coordinate
	float v; // v texture coordinate
	Vec3 Point; // coordinate of hit point
	Vec3 Normal; // normal vector
	const class Material* Material;
};

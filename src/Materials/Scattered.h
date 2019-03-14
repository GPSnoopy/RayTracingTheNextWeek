#pragma once

#include "Ray.h"
#include "Vec3.h"

struct Scattered
{
	Scattered(const Ray& scattered, const Vec3& attenuation) :
		Ray(scattered), Attenuation(attenuation)
	{
	}

	const class Ray Ray;
	const Vec3 Attenuation;
};

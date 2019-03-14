#pragma once

#include "PerlinNoise.h"

class NoiseTexture final : public Texture
{
public:

	NoiseTexture(Random& random, const float scale, const float turbulence) :
		Scale(scale), Turbulence(turbulence), perlin_(random) { }

	Vec3 Value(const float, const float, const Vec3& point) const override
	{
		return Vec3(1)*0.5f*(1 + std::sin(Scale*point.x() + Turbulence*perlin_.Turbulence(Scale*point)));
	}

	const float Scale;
	const float Turbulence;

private:

	const PerlinNoise perlin_;
};
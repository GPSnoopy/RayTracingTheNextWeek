#pragma once

#include "Texture.h"

class CheckerTexture final : public Texture
{
public:

	CheckerTexture(const Texture* const odd, const Texture* const even) :
		Odd(odd), Even(even) { }

	const TexturePtr Odd;
	const TexturePtr Even;

	Vec3 Value(const float u, const float v, const Vec3& point) const override
	{
		return std::sin(10 * point.x())*std::sin(10 * point.y())*std::sin(10 * point.z()) < 0
			? Odd->Value(u, v, point)
			: Even->Value(u, v, point);
	}
};
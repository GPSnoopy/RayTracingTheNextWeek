#pragma once

#include "Texture.h"

class ConstantTexture final : public Texture
{
public:

	ConstantTexture(const float r, const float g, const float b) :
		ConstantTexture(Vec3(r, g, b)) { }
	
	explicit ConstantTexture(const Vec3& color) : 
		Color(color) { }

	const Vec3 Color;

	Vec3 Value(const float, const float, const Vec3&) const override
	{
		return Color;
	}
};
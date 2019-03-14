#pragma once

#include "Vec3.h"

class Texture
{
public:

	Texture() = default;
	Texture(Texture&&) = default;
	virtual ~Texture() = default;

	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;
	Texture& operator = (Texture&&) = delete;

	virtual Vec3 Value(float u, float v, const Vec3& point) const = 0;
};

typedef std::shared_ptr<const Texture> TexturePtr;

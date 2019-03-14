#pragma once

#include "Material.h"
#include "Textures/Texture.h"

class DiffuseLight final : public Material
{
public:

	explicit DiffuseLight(const Texture* const texture) : 
		Texture(texture) {	}

	const TexturePtr Texture;

	std::optional<Scattered> Scatter(const Ray&, const HitRecord&, Random&) const override
	{
		return std::nullopt;
	}

	Vec3 Emitted(const float u, const float v, const Vec3& point) const override
	{
		return Texture->Value(u, v, point);
	}
};

#pragma once

#include "Material.h"
#include "Textures/Texture.h"

class Isotropic final : public Material
{
public:

	explicit Isotropic(const Texture* const texture) : Texture(texture) {	}
	explicit Isotropic(TexturePtr texture) : Texture(std::move(texture)) { }

	const TexturePtr Texture;

	std::optional<Scattered> Scatter(const Ray& ray, const HitRecord& record, Random& random) const override
	{
		return Scattered(Ray(record.Point, RandomInUnitSphere(random), ray.Time), Texture->Value(record.u, record.v, record.Point));
	}
};

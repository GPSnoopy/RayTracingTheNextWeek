#pragma once

#include "Material.h"
#include "Textures/Texture.h"

class Lambertian final : public Material
{
public:

	explicit Lambertian(const Texture* const texture) : Texture(texture) {	}
	explicit Lambertian(TexturePtr texture) : Texture(std::move(texture)) { }
	
	const TexturePtr Texture;

	std::optional<Scattered> Scatter(const Ray& ray, const HitRecord& record, Random& random) const override
	{
		const Vec3 target = record.Point + record.Normal + RandomInUnitSphere(random);
		return Scattered(Ray(record.Point, target - record.Point, ray.Time), Texture->Value(record.u, record.v, record.Point));
	}
};

#pragma once

#include "Hittables/HitRecord.h"
#include "Scattered.h"
#include "Random.h"
#include <optional>
#include <utility>

class Material
{
public:

	Material() = default;
	Material(Material&&) = default;
	virtual ~Material() = default;

	Material(const Material&) = delete;
	Material& operator = (const Material&) = delete;
	Material& operator = (Material&&) = delete;

	virtual std::optional<Scattered> Scatter(const Ray& ray, const HitRecord& record, Random& random) const = 0;

	virtual Vec3 Emitted(const float u, const float v, const Vec3& point) const 
	{ 
		(void)u;
		(void)v;
		(void)point;
		return Vec3(0); 
	}

protected:

	static Vec3 Reflect(const Vec3& v, const Vec3& normal)
	{
		return v - 2 * Dot(v, normal)*normal;
	}

	static std::optional<Vec3> Refract(const Vec3& v, const Vec3& normal, const float niOverNt)
	{
		const Vec3 uv = UnitVector(v);
		const float dt = Dot(uv, normal);
		const float discriminant = 1 - niOverNt * niOverNt*(1 - dt * dt);

		return discriminant > 0
			? niOverNt * (uv - normal * dt) - normal * std::sqrt(discriminant)
			: std::optional<Vec3>();
	}
};

typedef std::shared_ptr<const Material> MaterialPtr;

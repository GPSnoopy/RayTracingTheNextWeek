#pragma once

#include "Ray.h"
#include "Random.h"


class Camera final
{
public:

	static Camera LookAt(
		const Vec3& origin, const Vec3& target, 
		const Vec3& up, 
		const float verticalFov, const float aspectRatio,
		const float aperture, const float focusDistance,
		const float shutterStart, const float shutterEnd)
	{
		const float theta = verticalFov * float(M_PI / 180);
		const float halfHeight = std::tan(theta / 2);
		const float halfWidth = aspectRatio * halfHeight;
		const float lensRadius = aperture / 2;

		const Vec3 w = UnitVector(origin - target);
		const Vec3 u = UnitVector(Cross(up, w));
		const Vec3 v = Cross(w, u);

		const Vec3 lowerLeftCorner = origin - focusDistance * (halfWidth * u + halfHeight * v + w);
		const Vec3 horizontal = 2 * halfWidth * focusDistance * u;
		const Vec3 vertical = 2 * halfHeight * focusDistance * v;

		return Camera(origin, lowerLeftCorner, horizontal, vertical, u, v, w, lensRadius, shutterStart, shutterEnd);
	}

	Ray GetRay(Random& random, const float s, const float t) const
	{
		const Vec3 rd = LensRadius * RandomInUnitDisk(random);
		const Vec3 offset = u * rd.x() + v * rd.y();
		const float time = ShutterStart + Uniform(random) * (ShutterEnd - ShutterStart);

		return Ray(Origin + offset, LowerLeftCorner + s * Horizontal + t * Vertical - Origin - offset, time);
	}

	const Vec3 Origin;
	const Vec3 LowerLeftCorner;
	const Vec3 Horizontal;
	const Vec3 Vertical;

	const Vec3 u, v, w;
	const float LensRadius;
	const float ShutterStart;
	const float ShutterEnd;

private:

	Camera(
		const Vec3& origin, 
		const Vec3& lowerLeftCorner, 
		const Vec3& horizontal, 
		const Vec3& vertical,
		const Vec3& u, const Vec3& v, const Vec3& w,
		const float lensRadius,
		const float shutterStart,
		const float shutterEnd) :
		Origin(origin),
		LowerLeftCorner(lowerLeftCorner),
		Horizontal(horizontal),
		Vertical(vertical),
		u(u), v(v), w(w),
		LensRadius(lensRadius),
		ShutterStart(shutterStart),
		ShutterEnd(shutterEnd)
	{
	}
};

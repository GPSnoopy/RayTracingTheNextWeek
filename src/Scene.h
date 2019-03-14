#pragma once

#include "Camera.h"
#include "Hittables.h"
#include "Materials.h"
#include "Textures.h"
#include "Textures/ImageTexture.h"

class Scene final
{
public:

	inline static const int QualityReduction = 1;
	inline static const int Width = 3840 / QualityReduction;
	inline static const int Height = 2160 / QualityReduction;
	inline static const float AspectRatio = float(Width) / float(Height);
	inline static const int Samples = 8*8*1024 / QualityReduction;
	inline static const int Bounces = 32;
	inline static const float ShutterStart = 0.0f;
	inline static const float ShutterEnd = 1.0f;

	const class Camera Camera;
	const HittablePtr World;
	const bool HasSky;

	// Random scene with many balls.
	static Scene RandomSpheres()
	{
		Random random(42);
		std::vector<HittablePtr> world;

		world.emplace_back(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(new CheckerTexture(
			new ConstantTexture(0.2f, 0.3f, 0.1f),
			new ConstantTexture(0.9f, 0.9f, 0.9f)))));

		for (int a = -11; a < 11; ++a)
		{
			for (int b = -11; b < 11; ++b)
			{
				const float chooseMat = Uniform(random);
				const Vec3 center(a + 0.9f*Uniform(random), 0.2f, b + 0.9f*Uniform(random));

				if (Length(center - Vec3(4, 0.2f, 0)) > 0.9)
				{
					if (chooseMat < 0.8f) // Diffuse
					{
						world.emplace_back(new SphereMoving(
							center, center + Vec3(0, 0.5f*Uniform(random), 0),
							ShutterStart, ShutterEnd,
							0.2f, new Lambertian(new ConstantTexture(
								Uniform(random)*Uniform(random),
								Uniform(random)*Uniform(random),
								Uniform(random)*Uniform(random)))));
					}
					else if (chooseMat < 0.95f) // Metal
					{
						world.emplace_back(new Sphere(center, 0.2f, new Metal(
							Vec3(0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random))),
							0.5f*Uniform(random))));
					}
					else // Glass
					{
						world.emplace_back(new Sphere(center, 0.2f, new Dielectric(1.5f)));
					}
				}
			}
		}

		world.emplace_back(new Sphere(Vec3(0, 1, 0), 1.0f, new Dielectric(1.5f)));
		world.emplace_back(new Sphere(Vec3(-4, 1, 0), 1.0f, new Lambertian(new ConstantTexture(0.4f, 0.2f, 0.1f))));
		world.emplace_back(new Sphere(Vec3(4, 1, 0), 1.0f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f)));

		const Vec3 lookFrom(13, 2, 3);
		const Vec3 lookAt(0, 0, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 20;
		const float aperture = 0.1f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);
		auto bvh = BvhNode::Create(world.begin(), world.end(), ShutterStart, ShutterEnd);

		return Scene(camera, std::move(bvh), true);
	}

	// Two checker spheres.
	static Scene TwoCheckerSpheres()
	{
		HittableList world;

		const TexturePtr checker(new CheckerTexture(
			new ConstantTexture(0.2f, 0.3f, 0.1f),
			new ConstantTexture(0.9f, 0.9f, 0.9f)
		));

		world.Add(new Sphere(Vec3(0, -10, 0), 10, new Lambertian(checker)));
		world.Add(new Sphere(Vec3(0,  10, 0), 10, new Lambertian(checker)));

		const Vec3 lookFrom(13, 2, 3);
		const Vec3 lookAt(0, 0, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 20;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), true);
	}

	// Two perlin spheres.
	static Scene TwoPerlinSpheres()
	{
		Random random(42);
		HittableList world;

		const TexturePtr perlinTexture(new NoiseTexture(random, 4.0f, 10));

		world.Add(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(perlinTexture)));
		world.Add(new Sphere(Vec3(0, 2, 0), 2, new Lambertian(perlinTexture)));

		const Vec3 lookFrom(13, 2, 3);
		const Vec3 lookAt(0, 0, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 20;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), true);
	}

	static Scene Earth()
	{
		HittableList world;

		world.Add(new Sphere(Vec3(0, 0, 0), 2, new Lambertian(ImageTexture::Load("data/land_ocean_ice_cloud_8192.png"))));

		const Vec3 lookFrom(13, 2, 3);
		const Vec3 lookAt(0, 0, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 20;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), true);
	}

	static Scene SimpleLight()
	{
		Random random(42);
		HittableList world;

		const TexturePtr perlinTexture(new NoiseTexture(random, 4.0f, 10));

		world.Add(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(perlinTexture)));
		world.Add(new Sphere(Vec3(0, 2, 0), 2, new Lambertian(perlinTexture)));
		world.Add(new Sphere(Vec3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(4, 4, 4))));
		world.Add(new RectangleXY(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(4, 4, 4))));

		const Vec3 lookFrom(13, 2, 3);
		const Vec3 lookAt(0, 2, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 40;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), false);
	}

	static Scene CornellBox()
	{
		HittableList world;

		MaterialPtr red(new Lambertian(new ConstantTexture(0.65f, 0.05f, 0.05f)));
		MaterialPtr green(new Lambertian(new ConstantTexture(0.12f, 0.45f, 0.15f)));
		MaterialPtr white(new Lambertian(new ConstantTexture(0.73f, 0.73f, 0.73f)));
		MaterialPtr light(new DiffuseLight(new ConstantTexture(Vec3(15))));

		world.Add(new FlipNormals(new RectangleYZ(0, 555, 0, 555, 555, green)));
		world.Add(new RectangleYZ(0, 555, 0, 555, 0, red));
		world.Add(new RectangleXZ(213, 343, 227, 332, 554, light));
		world.Add(new FlipNormals(new RectangleXZ(0, 555, 0, 555, 555, white)));
		world.Add(new RectangleXZ(0, 555, 0, 555, 0, white));
		world.Add(new FlipNormals(new RectangleXY(0, 555, 0, 555, 555, white)));

		world.Add(
			new Translate(
				new RotateY(
					new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65)));

		world.Add(
			new Translate(
				new RotateY(
					new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295)));

		const Vec3 lookFrom(278, 278, -800);
		const Vec3 lookAt(278, 278, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 40;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), false);
	}

	static Scene CornellBoxSmoke()
	{
		HittableList world;

		MaterialPtr red(new Lambertian(new ConstantTexture(0.65f, 0.05f, 0.05f)));
		MaterialPtr green(new Lambertian(new ConstantTexture(0.12f, 0.45f, 0.15f)));
		MaterialPtr white(new Lambertian(new ConstantTexture(0.73f, 0.73f, 0.73f)));
		MaterialPtr light(new DiffuseLight(new ConstantTexture(Vec3(7))));

		world.Add(new FlipNormals(new RectangleYZ(0, 555, 0, 555, 555, green)));
		world.Add(new RectangleYZ(0, 555, 0, 555, 0, red));
		world.Add(new RectangleXZ(113, 443, 127, 432, 554, light));
		world.Add(new FlipNormals(new RectangleXZ(0, 555, 0, 555, 555, white)));
		world.Add(new RectangleXZ(0, 555, 0, 555, 0, white));
		world.Add(new FlipNormals(new RectangleXY(0, 555, 0, 555, 555, white)));

		HittablePtr box1(
			new Translate(
				new RotateY(
					new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65)));

		HittablePtr box2(
			new Translate(
				new RotateY(
					new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295)));

		world.Add(new ConstantMedium(box1, 0.01, new ConstantTexture(1.0f, 1.0f, 1.0f)));
		world.Add(new ConstantMedium(box2, 0.01, new ConstantTexture(0.0f, 0.0f, 0.0f)));

		const Vec3 lookFrom(278, 278, -800);
		const Vec3 lookAt(278, 278, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 40;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), false);
	}

	static Scene Final()
	{
		Random random(41);
		HittableList world;

		MaterialPtr white(new Lambertian(new ConstantTexture(0.73f, 0.73f, 0.73f)));
		MaterialPtr ground(new Lambertian(new ConstantTexture(0.48f, 0.83f, 0.53f)));

		std::vector<HittablePtr> boxes1;

		for (int i = 0; i != 20; ++i)
		{
			for (int j = 0; j != 20; ++j)
			{
				const float w = 100;
				const float x0 = -1000 + i * w;
				const float y0 = 0;
				const float z0 = -1000 + j * w;
				const float x1 = x0 + w;
				const float y1 = 100 * (Uniform(random) + 0.01);
				const float z1 = z0 + w;

				boxes1.emplace_back(new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground));
			}
		}

		world.Add(BvhNode::Create(boxes1.begin(), boxes1.end(), ShutterStart, ShutterEnd));

		MaterialPtr light(new DiffuseLight(new ConstantTexture(7, 7, 7)));

		world.Add(new RectangleXZ(123, 423, 147, 412, 554, light));

		const Vec3 center(400, 400, 200);
		world.Add(new SphereMoving(center, center + Vec3(30, 0, 0), ShutterStart, ShutterEnd, 50, new Lambertian(new ConstantTexture(0.7f, 0.3f, 0.1))));
		world.Add(new Sphere(Vec3(260, 150, 45), 50, new Dielectric(1.5)));
		world.Add(new Sphere(Vec3(-60, 115, 25), 50, new Dielectric(1.5)));
		world.Add(new Sphere(Vec3(0, 150, 145), 50, new Metal(Vec3(0.8, 0.8, 0.9), 10.0f)));

		const HittablePtr boundary0(new Sphere(Vec3(360, 150, 145), 70, new Dielectric(1.5f)));
		world.Add(boundary0);
		world.Add(new ConstantMedium(boundary0, 0.2f, new ConstantTexture(0.2f, 0.4f, 0.9f)));

		const HittablePtr boundary1(new Sphere(Vec3(0, 0, 0), 5000, new Dielectric(1.5f)));
		world.Add(new ConstantMedium(boundary1, 0.0001f, new ConstantTexture(1.0f, 1.0f, 1.0f)));

		world.Add(new Sphere(Vec3(400, 200, 400), 100, new Lambertian(ImageTexture::Load("data/land_ocean_ice_cloud_2048.png"))));
		world.Add(new Sphere(Vec3(220, 280, 300), 80, new Lambertian(new NoiseTexture(random, 0.1f, 7))));

		std::vector<HittablePtr> boxes2;

		for (int i = 0; i < 1000; ++i)
		{
			boxes2.emplace_back(new Sphere(Vec3(165 * Uniform(random), 165 * Uniform(random), 165 * Uniform(random)), 10, white));
		}

		world.Add(
			new Translate(
				new RotateY(BvhNode::Create(boxes2.begin(), boxes2.end(), ShutterStart, ShutterEnd), 15.0f),
				Vec3(-100, 270, 395)));

		const Vec3 lookFrom(478, 278, -600);
		const Vec3 lookAt(278, 278, 0);
		const Vec3 up(0, 1, 0);
		const float fov = 40;
		const float aperture = 0.0f;
		const float focusDistance = 10.0f;

		auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, AspectRatio, aperture, focusDistance, ShutterStart, ShutterEnd);

		return Scene(camera, HittablePtr(new HittableList(std::move(world))), false);
	}

private:

	Scene(::Camera camera, HittablePtr&& world, const bool hasSky) :
		Camera(std::move(camera)), World(std::move(world)), HasSky(hasSky) { }
};

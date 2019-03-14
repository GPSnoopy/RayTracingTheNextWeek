
#include "Scene.h"

#include <limits>
#include <thread>

Vec3 Color(const Scene& scene, const Ray& ray, const int depth, const int maxDepth, Random& random)
{
	const auto record = scene.World->Hit(ray, 0.001f, std::numeric_limits<float>::max(), random);

	if (record)
	{
		const Vec3 emitted = record->Material->Emitted(record->u, record->v, record->Point);
		const auto scattered = record->Material->Scatter(ray, *record, random);

		return depth < maxDepth && scattered 
			? emitted + scattered->Attenuation * Color(scene, scattered->Ray, depth + 1, maxDepth, random)
			: emitted;
	}

	if (scene.HasSky)
	{
		const Vec3 direction = UnitVector(ray.Direction);
		const float t = 0.5f*(direction.y() + 1);
		return (1 - t)*Vec3(1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
	}

	return Vec3(0);
}

void ThreadMainLoop(std::vector<Vec3>& buffer, const Scene& scene, const int numThreads, const int threadId)
{
	Random random(threadId + 1);

	for (int j = Scene::Height - threadId - 1; j >= 0; j -= numThreads)
	{
		for (int i = 0; i < Scene::Width; ++i)
		{
			Vec3 color(0);

			for (int s = 0; s < Scene::Samples; ++s)
			{
				const float u = float(i + Uniform(random)) / float(Scene::Width);
				const float v = float(j + Uniform(random)) / float(Scene::Height);

				color += Color(scene, scene.Camera.GetRay(random, u, v), 0, Scene::Bounces, random);
			}

			buffer[j*Scene::Width + i] = Clamp(Sqrt(color / float(Scene::Samples)) * 255.99f, Vec3(0.0f), Vec3(255.0f));
		}
	}
}

void MultiThreadedRayTracing(std::vector<Vec3>& buffer, const Scene& scene)
{
	const int numThreads = std::max(1, static_cast<int>(std::thread::hardware_concurrency()) - 2);
	std::vector<std::thread> threads;

	std::cerr << "Ray-tracing using " << numThreads << " threads" << std::endl;

	for (int t = 0; t != numThreads; ++t)
	{
		threads.emplace_back([=, &buffer, &scene]()
		{
			ThreadMainLoop(buffer, scene, numThreads, t);
		});
	}	

	for (auto& thread : threads)
	{
		thread.join();
	}
}

void OutputFramebuffer(const std::vector<Vec3>& buffer, const int w, const int h)
{
	std::cout << "P3\n" << w << " " << h << "\n255\n";

	for (int j = h; j--;)
	{
		for (int i = 0; i < w; ++i)
		{
			const Vec3& color = buffer[j*w + i];
			std::cout << static_cast<int>(color.x()) << " " << static_cast<int>(color.y()) << " " << static_cast<int>(color.z()) << "\n";
		}
	}
}

void Application()
{
	const auto scene = Scene::Final();

	std::vector<Vec3> buffer(Scene::Width * Scene::Height);

	MultiThreadedRayTracing(buffer, scene);
	OutputFramebuffer(buffer, Scene::Width, Scene::Height);
}

int main()
{
	try
	{
		Application();

		return EXIT_SUCCESS;
	}

	catch (const std::exception& exception)
	{
		std::cerr << "ERROR: " << exception.what() << std::endl;
	}

	catch (...)
	{
		std::cerr << "ERROR: unhandled exception" << std::endl;
	}

	return EXIT_FAILURE;
}

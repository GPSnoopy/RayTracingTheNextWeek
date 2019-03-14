#pragma once

#include "Random.h"
#include "Vec3.h"
#include <vector>
#include <numeric>

class PerlinNoise final
{
public:

	explicit PerlinNoise(Random& random) :
		randomVectors_(CreateRandomVectors(random)),
		permutationsX_(CreatePermutations(random)),
		permutationsY_(CreatePermutations(random)),
		permutationsZ_(CreatePermutations(random))
	{
	}

	float Turbulence(const Vec3& point, const int depth = 7) const
	{
		Vec3 tempPoint = point;
		float accum = 0;
		float weight = 1;

		for (int i = 0; i != depth; ++i)
		{
			accum += weight * Noise(tempPoint);
			weight *= 0.5f;
			tempPoint *= 2;
		}

		return std::abs(accum);
	}

	float Noise(const Vec3& point) const
	{
		const Vec3 floor = Floor(point);
		const Vec3 coord = point - floor;
		const int i = int(floor.x());
		const int j = int(floor.y());
		const int k = int(floor.z());

		Vec3 c[2][2][2];

		for (int di = 0; di != 2; ++di)
			for (int dj = 0; dj != 2; ++dj)
				for (int dk = 0; dk != 2; ++dk)
					c[di][dj][dk] = randomVectors_[
						permutationsX_[(i + di) & 255] ^ 
						permutationsY_[(j + dj) & 255] ^ 
						permutationsZ_[(k + dk) & 255]];

		return PerlinInterpolate(c, coord);
	}

private:

	static std::vector<Vec3> CreateRandomVectors(Random& random)
	{
		std::vector<Vec3> floats(256);
		std::generate(floats.begin(), floats.end(), [&random]()
		{
			return UnitVector(Vec3(-1 + 2 * Uniform(random), -1 + 2 * Uniform(random), -1 + 2 * Uniform(random)));
		});

		return floats;
	}

	static std::vector<int> CreatePermutations(Random& random)
	{
		std::vector<int> permutations(256);
		std::iota(permutations.begin(), permutations.end(), 0);
		std::shuffle(permutations.begin(), permutations.end(), random);

		return permutations;
	}

	static float PerlinInterpolate(const Vec3 c[2][2][2], const Vec3 coord)
	{
		const Vec3 t = coord * coord*(3 - 2 * coord);
		float accum = 0;

		for (int i = 0; i != 2; ++i) 
			for (int j = 0; j != 2; ++j)
				for (int k = 0; k != 2; ++k) 
				{
					const Vec3 weight = t - Vec3(float(i), float(j), float(k));

					accum +=
						(i*t.x() + (1 - i)*(1 - t.x()))*
						(j*t.y() + (1 - j)*(1 - t.y()))*
						(k*t.z() + (1 - k)*(1 - t.z()))*
						Dot(c[i][j][k], weight);
				}

		return accum;
	}

	const std::vector<Vec3> randomVectors_;
	const std::vector<int> permutationsX_;
	const std::vector<int> permutationsY_;
	const std::vector<int> permutationsZ_;
};

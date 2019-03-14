#pragma once

#include "Texture.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "ThirdParty/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#undef STBI_ONLY_PNG

class ImageTexture final : public Texture
{
public:

	static TexturePtr Load(const std::string& filename)
	{
		int width, height, comp;
		std::unique_ptr<uint8_t, void (*) (void*)> data(
			stbi_load(filename.c_str(), &width, &height, &comp, 3),
			stbi_image_free);

		if (!data)
		{
			throw std::runtime_error("failed to load image '" + filename + "' (" + stbi_failure_reason() + ")");
		}

		return std::make_unique<const ImageTexture>(std::vector<uint8_t>(data.get(), data.get() + (width*height*comp)), width, height);
	}

	ImageTexture(std::vector<uint8_t> pixels, const int width, const int height) :
		Pixels(std::move(pixels)), Width(width), Height(height) { }

	const std::vector<uint8_t> Pixels;
	const int Width;
	const int Height;

	Vec3 Value(const float u, const float v, const Vec3&) const override
	{
		const int i = std::clamp(int(u*Width), 0, Width - 1);
		const int j = std::clamp(int((1 - v)*Height), 0, Height - 1);

		return
		{
			Pixels[3 * i + 3 * Width*j + 0] / 255.0f,
			Pixels[3 * i + 3 * Width*j + 1] / 255.0f,
			Pixels[3 * i + 3 * Width*j + 2] / 255.0f,
		};
	}
};

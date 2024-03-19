#pragma once

#include <string>

#include "Fengshui/Core/Core.h"
#include "Fengshui/Renderer/Shader.h"

namespace Fengshui
{
	class TextureBase
	{
	public:
		virtual ~TextureBase() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind() const = 0;
	};

	class Texture : public TextureBase
	{
	public:
		virtual ~Texture() = default;
		static Ref<Texture> Create(const std::string& filePath, Ref<Shader> shader);
	};

	class Texture2D : public TextureBase
	{
	public:
		virtual ~Texture2D() = default;
		static Ref<Texture2D> Create(const std::string& filePath, Ref<Shader> shader);
	};
}
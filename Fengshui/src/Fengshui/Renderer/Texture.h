#pragma once

#include <string>

#include "Fengshui/Core/Core.h"

namespace Fengshui
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;
		static Ref<Texture2D> Create(const std::string& filePath);
	};
}
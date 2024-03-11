#pragma once

#include "Fengshui/Renderer/Texture.h"

namespace Fengshui
{
	class OpenGLTexture
	{
		class OpenGLTexture2D : public Texture2D
		{
		public:
			OpenGLTexture2D(const std::string& filePath);
			virtual ~OpenGLTexture2D();

			virtual uint32_t GetWidth() const { return m_Width; };
			virtual uint32_t GetHeight() const { return m_Height; };

			virtual void Bind() const override;

		private:
			uint32_t m_Width;
			uint32_t m_Height;
			uint32_t m_RendererID;
		};
	};
}


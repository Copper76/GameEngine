#pragma once

#include "Fengshui/Renderer/Texture.h"

#include <glad/glad.h>

namespace Fengshui
{

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(uint32_t width, uint32_t height);
		OpenGLTexture(const std::string& filePath);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const { return m_Width; };
		virtual uint32_t GetHeight() const { return m_Height; };

		virtual uint32_t GetRendererID() const { return m_RendererID; };

		virtual void SetData(void* data, uint32_t size);

		virtual void Bind(int slot) const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTexture&)other).m_RendererID; };

	private:
		std::string m_FilePath;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_Format;
	};

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const { return m_Width; };
		virtual uint32_t GetHeight() const { return m_Height; };

		virtual uint32_t GetRendererID() const { return m_RendererID; };

		virtual void SetData(void* data, uint32_t size);

		virtual void Bind(int slot) const override;

		virtual bool operator==(const Texture2D& other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; };

	private:

		std::string m_FilePath;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_Format;
	};
}


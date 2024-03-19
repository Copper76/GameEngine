#include "fspch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Fengshui
{
	OpenGLTexture::OpenGLTexture(const std::string& filePath, Ref<Shader> shader)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);//stb loads top down but opengl reads bottom up
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		FS_ENGINE_ASSERT(data, "Failed to load image");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		FS_ENGINE_ASSERT(internalFormat & dataFormat, "The format is not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Bind() const
	{
		glBindTextureUnit(m_Slot, m_RendererID);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, Ref<Shader> shader) : m_FilePath(filePath), m_Slot(shader->GetNextSlot())
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);//stb loads top down but opengl reads bottom up
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		FS_ENGINE_ASSERT(data, "Failed to load image");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		FS_ENGINE_ASSERT(internalFormat & dataFormat, "The format is not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTextureUnit(m_Slot, m_RendererID);
	}
}
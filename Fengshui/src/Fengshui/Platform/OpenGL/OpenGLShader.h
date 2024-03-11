#pragma once

#include "Fengshui/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Fengshui
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;//for debugging mostly

		void UploadUniformInt(const std::string name, int value);
		void UploadUniformFloat(const std::string name, float value);
		void UploadUniformVec2(const std::string name, const glm::vec2& value);
		void UploadUniformVec3(const std::string name, const glm::vec3& value);
		void UploadUniformVec4(const std::string name, const glm::vec4& value);
		void UploadUniformMat3(const std::string name, const glm::mat3& value);
		void UploadUniformMat4(const std::string name, const glm::mat4& value);

	private:
		uint32_t m_RendererID;
	};
}


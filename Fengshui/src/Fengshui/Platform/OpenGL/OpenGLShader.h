#pragma once

#include "Fengshui/Renderer/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Fengshui
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& name);
		OpenGLShader(const std::string& shaderFilePath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;//for debugging mostly

		virtual const std::string GetName() const override { return m_Name; }

		virtual uint32_t GetNextSlot() override;

		virtual void SetInt(const std::string name, int value) override;
		virtual void SetFloat(const std::string name, float value) override;
		virtual void SetVec2(const std::string name, const glm::vec2& value) override;
		virtual void SetVec3(const std::string name, const glm::vec3& value) override;
		virtual void SetVec4(const std::string name, const glm::vec4& value) override;
		virtual void SetMat3(const std::string name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string name, const glm::mat4& value) override;

	private:
		void UploadUniformInt(const std::string name, int value);
		void UploadUniformFloat(const std::string name, float value);
		void UploadUniformVec2(const std::string name, const glm::vec2& value);
		void UploadUniformVec3(const std::string name, const glm::vec3& value);
		void UploadUniformVec4(const std::string name, const glm::vec4& value);
		void UploadUniformMat3(const std::string name, const glm::mat3& value);
		void UploadUniformMat4(const std::string name, const glm::mat4& value);

		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string> shaderSources);
	private:
		uint32_t m_RendererID;
		uint32_t m_NextSlot = 0;
		std::string m_Name;
	};
}


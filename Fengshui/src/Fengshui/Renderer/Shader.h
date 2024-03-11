#pragma once

#include <string>

namespace Fengshui
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;//for debugging mostly

		//virtual void UploadVSRendererUniformBuffer() = 0;
		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}
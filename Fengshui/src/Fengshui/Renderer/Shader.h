#pragma once

#include <string>

namespace Fengshui
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;//for debugging mostly
	private:
		uint32_t m_RendererID;
	};
}
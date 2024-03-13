#pragma once

#include "Fengshui/Renderer/Buffer.h"

namespace Fengshui
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

	/**
	//TODO: Move this to RenderComponent
	class VertexArrayLibrary
	{
	public:
		void Add(const Ref<VertexArray> vertexArray, const std::string& name = "vertexArray");

		Ref<VertexArray> Get(const std::string& name);

	private:
		bool Exist(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<VertexArray>> m_VertexArrays;
	};
	**/
}
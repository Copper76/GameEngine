#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/Renderer2D.h"

#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

namespace Fengshui
{
	//Renderer::SceneData* Renderer::m_SceneData = new  Renderer::SceneData;

	//ShaderLibrary* Renderer::m_ShaderLib = new ShaderLibrary;

	//VertexArrayLibrary* Renderer::m_VertexArrLib = new VertexArrayLibrary;

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TexCoord;
		float TextureIndex;
		float TilingFactor;
	};

	struct RendererConfig
	{
		static const uint32_t MaxIndices = 50000;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t ShapeIndexCount = 0;

		Ref<VertexArray> shapeVertexArray;
		Ref<VertexBuffer> shapeVertexBuffer;
		Ref<Shader> StandardShader;

		QuadVertex* ShapeVertexBufferBase = 0;
		QuadVertex* ShapeVertexBufferPtr = 0;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};

	static RendererConfig s_Data;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(CameraComponent* camera)
	{
		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetMat4("u_ViewProjectionMatrix", camera->ViewProjectionMatrix);

		PrepareNextBatch();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::PrepareNextBatch()
	{
	}
}
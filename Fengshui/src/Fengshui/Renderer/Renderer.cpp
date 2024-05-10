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
		float textureIndex;
		float tilingFactor;
	};

	struct RendererConfig
	{
		static const uint32_t maxIndices = 50000;
		static const uint32_t maxTextureSlots = 32;

		uint32_t shapeIndexCount = 0;

		Ref<VertexArray> shapeVertexArray;
		Ref<VertexBuffer> shapeVertexBuffer;
		Ref<Shader> standardShader;

		QuadVertex* ShapeVertexBufferBase = 0;
		QuadVertex* ShapeVertexBufferPtr = 0;

		std::array<Ref<Texture2D>, maxTextureSlots> TextureSlots;
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

	void Renderer::BeginScene(CameraComponent* scene)
	{
		s_Data.standardShader->Bind();
		s_Data.standardShader->SetMat4("u_ViewProjectionMatrix", scene->ViewProjectionMatrix);

		PrepareNextBatch();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::PrepareNextBatch()
	{
	}
}
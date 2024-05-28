#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/Renderer2D.h"

#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

namespace Fengshui
{
	//Renderer::SceneData* Renderer::m_SceneData = new  Renderer::SceneData;

	//ShaderLibrary* Renderer::m_ShaderLib = new ShaderLibrary;

	//VertexArrayLibrary* Renderer::m_VertexArrLib = new VertexArrayLibrary;

	struct ShapeVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TexCoord;
		float TextureIndex;
		float TilingFactor;
	};

	struct RendererConfig
	{
		static const uint32_t MaxCubes = 10000;
		static const uint32_t MaxVertices = MaxCubes * 8;
		static const uint32_t MaxIndices = MaxCubes * 36;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t ShapeIndexCount = 0;
		uint32_t ShapeVertexCount = 0;

		Ref<VertexArray> ShapeVertexArray;
		Ref<VertexBuffer> ShapeVertexBuffer;
		Ref<IndexBuffer> ShapeIndexBuffer;
		Ref<Shader> StandardShader;

		ShapeVertex* ShapeVertexBufferBase = 0;
		ShapeVertex* ShapeVertexBufferPtr = 0;
		uint32_t* ShapeIndexBufferPtr = 0;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		std::array<std::array<glm::vec4, 4>, 6> CubeVertexPositions;
	};

	static RendererConfig s_Data;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();

		s_Data.ShapeVertexArray = VertexArray::Create();

		s_Data.ShapeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(ShapeVertex));

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				{ShaderDataType::Vec4, "a_Colour"},
				{ShaderDataType::Vec2, "a_TexCoord"},
				{ShaderDataType::Float, "a_TexIndex"},
				{ShaderDataType::Float, "a_TilingFactor"},
				});

			s_Data.ShapeVertexBuffer->SetLayout(layout);
		}
		s_Data.ShapeVertexArray->AddVertexBuffer(s_Data.ShapeVertexBuffer);

		s_Data.ShapeVertexBufferBase = new ShapeVertex[s_Data.MaxVertices];

		//uint32_t* shapeIndices = new uint32_t[s_Data.MaxIndices];
		s_Data.ShapeIndexBufferPtr = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)//Optimised for cubes, shoud probably revert to tries for flexibility
		{
			s_Data.ShapeIndexBufferPtr[i + 0] = offset + 0;
			s_Data.ShapeIndexBufferPtr[i + 1] = offset + 1;
			s_Data.ShapeIndexBufferPtr[i + 2] = offset + 2;

			s_Data.ShapeIndexBufferPtr[i + 3] = offset + 2;
			s_Data.ShapeIndexBufferPtr[i + 4] = offset + 3;
			s_Data.ShapeIndexBufferPtr[i + 5] = offset + 0;

			offset += 4;
		}
		s_Data.ShapeIndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);
		s_Data.ShapeVertexArray->SetIndexBuffer(s_Data.ShapeIndexBuffer);

		//delete[] shapeIndices;

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.StandardShader = Shader::Create("Assets/Shaders/StandardShader.glsl");
		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		Ref<Texture> defaultTexture = Texture::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		defaultTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = defaultTexture; //0 is the default texture

		////Front face
		//s_Data.CubeVertexPositions[0] = { -0.5f, -0.5f, -0.5f, 1.0f };
		//s_Data.CubeVertexPositions[1] = { 0.5f, -0.5f, -0.5f, 1.0f };
		//s_Data.CubeVertexPositions[2] = { 0.5f, 0.5f, -0.5f, 1.0f };
		//s_Data.CubeVertexPositions[3] = { -0.5f, 0.5f, -0.5f, 1.0f };

		////Back face
		//s_Data.CubeVertexPositions[4] = { -0.5f, -0.5f, 0.5f, 1.0f };
		//s_Data.CubeVertexPositions[5] = { 0.5f, -0.5f, 0.5f, 1.0f };
		//s_Data.CubeVertexPositions[6] = { 0.5f, 0.5f, 0.5f, 1.0f };
		//s_Data.CubeVertexPositions[7] = { -0.5f, 0.5f, 0.5f, 1.0f };

		s_Data.CubeVertexPositions[0] = { glm::vec4{ -0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f } };
		s_Data.CubeVertexPositions[1] = { glm::vec4{ 0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f } };
		s_Data.CubeVertexPositions[2] = { glm::vec4{ 0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, 0.5f, 1.0f} , { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } };
		s_Data.CubeVertexPositions[3] = { glm::vec4{ -0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f } };
		s_Data.CubeVertexPositions[4] = { glm::vec4{ -0.5f, 0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f } };
		s_Data.CubeVertexPositions[5] = { glm::vec4{ -0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f } };
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
		Flush();
	}

	void Renderer::Flush()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.ShapeVertexBufferPtr - (uint8_t*)s_Data.ShapeVertexBufferBase);
		if (dataSize > 0)
		{
			s_Data.ShapeVertexBuffer->SetData(s_Data.ShapeVertexBufferBase, dataSize);
			s_Data.ShapeIndexBuffer->SetData(s_Data.ShapeIndexBufferPtr, s_Data.ShapeIndexCount);
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			RenderCommand::DrawIndexed(s_Data.ShapeVertexArray, s_Data.ShapeIndexCount);
		}
	}

	void Renderer::PrepareNextBatch()
	{
		s_Data.ShapeIndexCount = 0;
		s_Data.ShapeVertexCount = 0;
		s_Data.ShapeVertexBufferPtr = s_Data.ShapeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const float tilingFactor, const Ref<Texture>& texture, const glm::vec2* texCoords, const glm::vec4& colour)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
				glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
				glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawCube(transform, tilingFactor, texture, texCoords, colour);
	}

	void Renderer::DrawCube(const glm::mat4 transform, const float tilingFactor, const Ref<Texture>& texture, const glm::vec2* texCoords, const glm::vec4& colour)
	{

		if (s_Data.ShapeIndexCount >= RendererConfig::MaxIndices)
		{
			Flush();
			PrepareNextBatch();
		}

		float textureIndex = 0.0f;

		if (texture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				if (s_Data.TextureSlotIndex >= RendererConfig::MaxTextureSlots)
				{
					Flush();
					PrepareNextBatch();
				}
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		uint32_t i = s_Data.ShapeIndexCount;
		for (uint32_t face = 0; face < 6; face++)
		{
			uint32_t offset = s_Data.ShapeVertexCount;

			s_Data.ShapeIndexBufferPtr[i + 0] = offset + 0;
			s_Data.ShapeIndexBufferPtr[i + 1] = offset + 1;
			s_Data.ShapeIndexBufferPtr[i + 2] = offset + 2;

			s_Data.ShapeIndexBufferPtr[i + 3] = offset + 2;
			s_Data.ShapeIndexBufferPtr[i + 4] = offset + 3;
			s_Data.ShapeIndexBufferPtr[i + 5] = offset + 0;

			for (uint32_t i = 0; i < 4; i++)
			{
				s_Data.ShapeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[face][i];
				s_Data.ShapeVertexBufferPtr->Colour = colour;
				//s_Data.ShapeVertexBufferPtr->Colour = { i / 8.0f,i / 8.0f,i / 8.0f, 1.0f };
				s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr++;
			}

			s_Data.ShapeVertexCount += 4;
			s_Data.ShapeIndexCount += 6;
		}
	}
}
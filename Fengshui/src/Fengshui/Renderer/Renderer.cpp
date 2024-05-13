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

		Ref<VertexArray> ShapeVertexArray;
		Ref<VertexBuffer> ShapeVertexBuffer;
		Ref<Shader> StandardShader;

		ShapeVertex* ShapeVertexBufferBase = 0;
		ShapeVertex* ShapeVertexBufferPtr = 0;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 ShapeVertexPositions[8];
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

		uint32_t* shapeIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 36)//Optimised for cubes, shoud probably revert to tries for flexibility
		{
			shapeIndices[i + 0] = offset + 0;
			shapeIndices[i + 1] = offset + 1;
			shapeIndices[i + 2] = offset + 2;

			shapeIndices[i + 3] = offset + 2;
			shapeIndices[i + 4] = offset + 3;
			shapeIndices[i + 5] = offset + 0;

			shapeIndices[i + 6] = offset + 1;
			shapeIndices[i + 7] = offset + 5;
			shapeIndices[i + 8] = offset + 6;

			shapeIndices[i + 9]  = offset + 6;
			shapeIndices[i + 10] = offset + 2;
			shapeIndices[i + 11] = offset + 1;

			shapeIndices[i + 12] = offset + 5;
			shapeIndices[i + 13] = offset + 4;
			shapeIndices[i + 14] = offset + 7;

			shapeIndices[i + 15] = offset + 7;
			shapeIndices[i + 16] = offset + 6;
			shapeIndices[i + 17] = offset + 5;

			shapeIndices[i + 18] = offset + 4;
			shapeIndices[i + 19] = offset + 0;
			shapeIndices[i + 20] = offset + 3;

			shapeIndices[i + 21] = offset + 3;
			shapeIndices[i + 22] = offset + 7;
			shapeIndices[i + 23] = offset + 4;

			shapeIndices[i + 24] = offset + 3;
			shapeIndices[i + 25] = offset + 2;
			shapeIndices[i + 26] = offset + 6;

			shapeIndices[i + 27] = offset + 6;
			shapeIndices[i + 28] = offset + 7;
			shapeIndices[i + 29] = offset + 3;

			shapeIndices[i + 30] = offset + 4;
			shapeIndices[i + 31] = offset + 5;
			shapeIndices[i + 32] = offset + 1;

			shapeIndices[i + 33] = offset + 1;
			shapeIndices[i + 34] = offset + 0;
			shapeIndices[i + 35] = offset + 4;

			offset += 8;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(s_Data.MaxIndices, shapeIndices);
		s_Data.ShapeVertexArray->SetIndexBuffer(indexBuffer);

		delete[] shapeIndices;

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

		//Front face
		s_Data.ShapeVertexPositions[0] = { -0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.ShapeVertexPositions[1] = { 0.5f, -0.5f, -0.5f, 1.0f };
		s_Data.ShapeVertexPositions[2] = { 0.5f, 0.5f, -0.5f, 1.0f };
		s_Data.ShapeVertexPositions[3] = { -0.5f, 0.5f, -0.5f, 1.0f };

		//Back face
		s_Data.ShapeVertexPositions[4] = { -0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.ShapeVertexPositions[5] = { 0.5f, -0.5f, 0.5f, 1.0f };
		s_Data.ShapeVertexPositions[6] = { 0.5f, 0.5f, 0.5f, 1.0f };
		s_Data.ShapeVertexPositions[7] = { -0.5f, 0.5f, 0.5f, 1.0f };
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
		uint32_t dataSize = (uint8_t*)s_Data.ShapeVertexBufferPtr - (uint8_t*)s_Data.ShapeVertexBufferBase;
		s_Data.ShapeVertexBuffer->SetData(s_Data.ShapeVertexBufferBase, dataSize);
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.ShapeVertexArray, s_Data.ShapeIndexCount);
	}

	void Renderer::PrepareNextBatch()
	{
		s_Data.ShapeIndexCount = 0;
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

		for (uint32_t i = 0; i < 8; i++)
		{
			s_Data.ShapeVertexBufferPtr->Position = transform * s_Data.ShapeVertexPositions[i];
			//s_Data.ShapeVertexBufferPtr->Colour = colour;
			s_Data.ShapeVertexBufferPtr->Colour = {i / 8.0f,i / 8.0f,i / 8.0f, 1.0f };
			//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.ShapeVertexBufferPtr->TexCoord = {0,0};
			s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.ShapeVertexBufferPtr++;
		}

		s_Data.ShapeIndexCount += 36;
	}
}
#include "fspch.h"
#include "Renderer2D.h"
#include "Fengshui/Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Fengshui
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TexCoord;
		float TextureIndex;
		float TilingFactor;
	};

	struct Renderer2DConfig
	{
		static const uint32_t MaxQuads = 10000;//Maximum number of quads that can be drawn with a single draw call
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t QuadIndexCount = 0;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		Ref<Shader> StandardShader;

		QuadVertex* QuadVertexBufferBase = 0;
		QuadVertex* QuadVertexBufferPtr = 0;
		uint32_t* QuadIndicesPtr = 0;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DConfig s_Data;

	void Renderer2D::Init()
	{

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				{ShaderDataType::Vec4, "a_Colour"},
				{ShaderDataType::Vec2, "a_TexCoord"},
				{ShaderDataType::Float, "a_TexIndex"},
				{ShaderDataType::Float, "a_TilingFactor"},
				});

			s_Data.QuadVertexBuffer->SetLayout(layout);
		}
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		s_Data.QuadIndicesPtr = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			s_Data.QuadIndicesPtr[i + 0] = offset + 0;
			s_Data.QuadIndicesPtr[i + 1] = offset + 1;
			s_Data.QuadIndicesPtr[i + 2] = offset + 2;

			s_Data.QuadIndicesPtr[i + 3] = offset + 2;
			s_Data.QuadIndicesPtr[i + 4] = offset + 3;
			s_Data.QuadIndicesPtr[i + 5] = offset + 0;

			offset += 4;
		}
		s_Data.QuadIndexBuffer = IndexBuffer::Create(s_Data.MaxIndices, s_Data.QuadIndicesPtr);
		s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.StandardShader = Shader::Create("Assets/Shaders/StandardShader2D.glsl");
		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		Ref<Texture2D> defaultTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		defaultTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = defaultTexture; //0 is the default texture

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		//delete s_Data;
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const CameraComponent* camera)
	{
		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetMat4("u_ViewProjectionMatrix", camera->ViewProjectionMatrix);

		PrepareNextBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
			s_Data.QuadIndexBuffer->SetData(s_Data.QuadIndicesPtr, s_Data.QuadIndexCount);
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		}
	}

	void Renderer2D::PrepareNextBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const float tilingFactor, const Ref<Texture2D>& texture, const glm::vec2* texCoords, const glm::vec4& colour)
	{
		glm::mat4 transform;
		if (rotation == 0)
		{
			transform = glm::scale(glm::translate(glm::mat4(1.0f), { position, 0.0f }), { size.x, size.y, 1.0f });
		}
		else
		{
			transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), { position, 0.0f }), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		}
		DrawQuad(transform, tilingFactor, texture, texCoords, colour);
	}

	void Renderer2D::DrawQuad(const glm::mat4 transform, const float tilingFactor, const Ref<Texture2D>& texture, const glm::vec2* texCoords, const glm::vec4& colour)
	{
		if (s_Data.QuadIndexCount >= Renderer2DConfig::MaxIndices)
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
				if (s_Data.TextureSlotIndex >= Renderer2DConfig::MaxTextureSlots)
				{
					Flush();
					PrepareNextBatch();
				}
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Colour = colour;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}
}
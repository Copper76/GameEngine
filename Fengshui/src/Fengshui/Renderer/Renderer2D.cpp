#include "fspch.h"
#include "Renderer2D.h"
#include "Fengshui/Renderer/RenderCommand.h"

namespace Fengshui
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Colour;
		glm::vec2 TexCoord;
		float textureIndex;
		float tilingFactor;
	};

	struct Renderer2DConfig
	{
		static const uint32_t maxQuads = 10000;//Maximum number of quads that can be drawn with a single draw call
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;

		uint32_t quadIndexCount = 0;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> standardShader;
		//Ref<Texture2D> defaultTexture;

		QuadVertex* QuadVertexBufferBase = 0;
		QuadVertex* QuadVertexBufferPtr = 0;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];
	};

	static Renderer2DConfig s_Data;

	void Renderer2D::Init()
	{
		//s_Data.reset(new Renderer2DConfig);
		//s_Data = Renderer2DConfig();

		s_Data.quadVertexArray = VertexArray::Create();

		s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				{ShaderDataType::Vec4, "a_Colour"},
				{ShaderDataType::Vec2, "a_TexCoord"},
				{ShaderDataType::Float, "a_TexIndex"},
				{ShaderDataType::Float, "a_TilingFactor"},
				});

			s_Data.quadVertexBuffer->SetLayout(layout);
		}
		s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.maxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(s_Data.maxIndices, quadIndices);
		s_Data.quadVertexArray->SetIndexBuffer(indexBuffer);

		delete[] quadIndices;

		int32_t samplers[s_Data.maxTextureSlots];
		for (uint32_t i = 0; i < s_Data.maxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.standardShader = Shader::Create("Assets/Shaders/StandardShader2D.glsl");
		s_Data.standardShader->Bind();
		s_Data.standardShader->SetIntArray("u_Textures", samplers, s_Data.maxTextureSlots);

		Ref<Texture2D> defaultTexture = Texture2D::Create(1, 1);
		uint32_t whieTextureData = 0xffffffff;
		defaultTexture->SetData(&whieTextureData, sizeof(uint32_t));
		/**
		s_Data.defaultTexture = Texture2D::Create(1, 1);
		uint32_t whieTextureData = 0xffffffff;
		s_Data.defaultTexture->SetData(&whieTextureData, sizeof(uint32_t));

		s_Data.textureSlots[0] = s_Data.defaultTexture;
		**/

		s_Data.textureSlots[0] = defaultTexture; //0 is the default texture

		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		//memset(s_Data.textureSlots.data(), 0, s_Data.textureSlots.size() * sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		//delete s_Data;
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(Ref<Scene> scene)
	{
		s_Data.standardShader->Bind();
		s_Data.standardShader->SetMat4("u_ViewProjectionMatrix", scene->GetCameraComponent()->GetViewProjectionMatrix());

		PrepareNextBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.quadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++)
		{
			s_Data.textureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
	}

	void Renderer2D::PrepareNextBatch()
	{
		s_Data.quadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.textureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const float tilingFactor, const Ref<Texture2D>& texture, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, tilingFactor, texture, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const float tilingFactor, const Ref<Texture2D>& texture, const glm::vec4& colour)
	{

		if (s_Data.quadIndexCount >= Renderer2DConfig::maxIndices)
		{
			Flush();
			PrepareNextBatch();
		}

		float textureIndex = 0.0f;

		if (texture)
		{
			for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
			{
				if (*s_Data.textureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				if (s_Data.textureSlotIndex >= Renderer2DConfig::maxTextureSlots)
				{
					Flush();
					PrepareNextBatch();
				}
				textureIndex = (float)s_Data.textureSlotIndex;
				s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
				s_Data.textureSlotIndex++;
			}
		}

		glm::mat4 transform;
		if (rotation == 0)
		{
			transform = glm::scale(glm::translate(glm::mat4(1.0f), position), { size.x, size.y, 1.0f });
		}
		else
		{
			transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		}

		glm::vec2 texCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Colour = colour;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->textureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
	}

	void Renderer2D::DrawSubQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const float tilingFactor, const Ref<SubTexture2D>& subTexture, const glm::vec4& colour)
	{
		DrawSubQuad({ position.x, position.y, 0.0f }, size, rotation, tilingFactor, subTexture, colour);
	}

	void Renderer2D::DrawSubQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const float tilingFactor, const Ref<SubTexture2D>& subTexture, const glm::vec4& colour)
	{
		const glm::vec2* texCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_Data.quadIndexCount >= Renderer2DConfig::maxIndices)
		{
			Flush();
			PrepareNextBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex >= Renderer2DConfig::maxTextureSlots)
			{
				Flush();
				PrepareNextBatch();
			}
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		glm::mat4 transform;
		if (rotation == 0)
		{
			transform = glm::scale(glm::translate(glm::mat4(1.0f), position), { size.x, size.y, 1.0f });
		}
		else
		{
			transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Colour = colour;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->textureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(Ref<TransformComponent2D> transform, const float tilingFactor, const Ref<Texture2D>& texture, glm::vec2* texCoords, const glm::vec4& colour)
	{

		if (s_Data.quadIndexCount >= Renderer2DConfig::maxIndices)
		{
			Flush();
			PrepareNextBatch();
		}

		float textureIndex = 0.0f;

		if (texture)
		{
			for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
			{
				if (*s_Data.textureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				if (s_Data.textureSlotIndex >= Renderer2DConfig::maxTextureSlots)
				{
					Flush();
					PrepareNextBatch();
				}
				textureIndex = (float)s_Data.textureSlotIndex;
				s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
				s_Data.textureSlotIndex++;
			}
		}

		glm::vec2 coords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		if (texCoords == nullptr)
		{
			texCoords = coords;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform->GetTransform() * s_Data.quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Colour = colour;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->textureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
	}

	void Renderer2D::DrawSubQuad(Ref<TransformComponent2D> transform, const float tilingFactor, const Ref<SubTexture2D>& subTexture, const glm::vec4& colour)
	{
		const glm::vec2* texCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_Data.quadIndexCount >= Renderer2DConfig::maxIndices)
		{
			Flush();
			PrepareNextBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.textureSlotIndex >= Renderer2DConfig::maxTextureSlots)
			{
				Flush();
				PrepareNextBatch();
			}
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform->GetTransform() * s_Data.quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Colour = colour;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->textureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
	}
}

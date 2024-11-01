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
		static const uint32_t MaxIndices = 100000;
		static const uint32_t MaxVertices = MaxIndices;
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

		glm::quat* CubeRotation;

		std::array<std::array<glm::vec4, 4>, 6> CubeVertexPositions;
	};

	static RendererConfig s_Data;

	void Renderer::Init()
	{
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

		s_Data.ShapeIndexBufferPtr = new uint32_t[s_Data.MaxIndices];

		s_Data.ShapeIndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);
		s_Data.ShapeVertexArray->SetIndexBuffer(s_Data.ShapeIndexBuffer);

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

		s_Data.CubeRotation = new glm::quat[6];

		s_Data.CubeRotation[0] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		s_Data.CubeRotation[1] = glm::quat(glm::radians(glm::vec3(180.0f, 0.0f, 0.0f)));
		s_Data.CubeRotation[2] = glm::quat(glm::radians(glm::vec3(90.0f, 0.0f, 0.0f)));
		s_Data.CubeRotation[3] = glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f)));
		s_Data.CubeRotation[4] = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
		s_Data.CubeRotation[5] = glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));


		s_Data.CubeVertexPositions[0] = { glm::vec4{ -0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f } };
		s_Data.CubeVertexPositions[1] = { glm::vec4{ 0.5f, -0.5f, -0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f } };
		s_Data.CubeVertexPositions[2] = { glm::vec4{ 0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, 0.5f, 1.0f} , { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } };
		s_Data.CubeVertexPositions[3] = { glm::vec4{ -0.5f, -0.5f, 0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f } };
		s_Data.CubeVertexPositions[4] = { glm::vec4{ -0.5f, 0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f }, { -0.5f, 0.5f, 0.5f, 1.0f } };
		s_Data.CubeVertexPositions[5] = { glm::vec4{ -0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.5f, -0.5f, -0.5f, 1.0f }, { -0.5f, -0.5f, -0.5f, 1.0f } };
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const CameraComponent* camera)
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

		uint32_t indexBase = s_Data.ShapeIndexCount;
		uint32_t vertexBase = s_Data.ShapeVertexCount;
		for (uint32_t face = 0; face < 6; face++)
		{

			s_Data.ShapeIndexBufferPtr[indexBase + 0] = vertexBase + 0;
			s_Data.ShapeIndexBufferPtr[indexBase + 1] = vertexBase + 1;
			s_Data.ShapeIndexBufferPtr[indexBase + 2] = vertexBase + 2;

			s_Data.ShapeIndexBufferPtr[indexBase + 3] = vertexBase + 2;
			s_Data.ShapeIndexBufferPtr[indexBase + 4] = vertexBase + 3;
			s_Data.ShapeIndexBufferPtr[indexBase + 5] = vertexBase + 0;

			for (uint32_t i = 0; i < 4; i++)
			{
				s_Data.ShapeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[face][i];
				s_Data.ShapeVertexBufferPtr->Colour = colour;
				s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr++;
			}
			vertexBase += 4;
			indexBase += 6;
		}

		s_Data.ShapeVertexCount += 24;
		s_Data.ShapeIndexCount += 36;
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const std::vector<glm::vec3>& vertexCoords, const float tilingFactor, const Ref<Texture>& texture, const glm::vec2* texCoords, const glm::vec4& colour, const glm::vec3* normals)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
				glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
				glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawCube(transform, vertexCoords, tilingFactor, texture, texCoords, colour, normals);
	}

	void Renderer::DrawCube(const glm::mat4 transform, const std::vector<glm::vec3>& vertexCoords, const float tilingFactor, const Ref<Texture>& texture, const glm::vec2* texCoords, const glm::vec4& colour, const glm::vec3* normals)
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

		uint32_t indexBase = s_Data.ShapeIndexCount;
		uint32_t vertexBase = s_Data.ShapeVertexCount;
		if (normals != nullptr)
		{
			glm::vec4 adjustedColour;
			for (uint32_t face = 0; face < 6; face++)
			{

				s_Data.ShapeIndexBufferPtr[indexBase + 0] = vertexBase + 0;
				s_Data.ShapeIndexBufferPtr[indexBase + 1] = vertexBase + 1;
				s_Data.ShapeIndexBufferPtr[indexBase + 2] = vertexBase + 2;

				s_Data.ShapeIndexBufferPtr[indexBase + 3] = vertexBase + 2;
				s_Data.ShapeIndexBufferPtr[indexBase + 4] = vertexBase + 3;
				s_Data.ShapeIndexBufferPtr[indexBase + 5] = vertexBase + 0;

				for (uint32_t i = 0; i < 4; i++)
				{
					s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[face * 4 + i], 1.0f };
					adjustedColour = colour * glm::vec4(normals[face * 4 + i], 1.0f);
					adjustedColour.a = colour.a;
					s_Data.ShapeVertexBufferPtr->Colour = adjustedColour;
					s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
					s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
					s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
					s_Data.ShapeVertexBufferPtr++;
				}
				vertexBase += 4;
				indexBase += 6;
			}
		}
		else
		{
			for (uint32_t face = 0; face < 6; face++)
			{

				s_Data.ShapeIndexBufferPtr[indexBase + 0] = vertexBase + 0;
				s_Data.ShapeIndexBufferPtr[indexBase + 1] = vertexBase + 1;
				s_Data.ShapeIndexBufferPtr[indexBase + 2] = vertexBase + 2;

				s_Data.ShapeIndexBufferPtr[indexBase + 3] = vertexBase + 2;
				s_Data.ShapeIndexBufferPtr[indexBase + 4] = vertexBase + 3;
				s_Data.ShapeIndexBufferPtr[indexBase + 5] = vertexBase + 0;

				for (uint32_t i = 0; i < 4; i++)
				{
					s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[face * 4 + i], 1.0f };
					s_Data.ShapeVertexBufferPtr->Colour = colour;
					s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
					s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
					s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
					s_Data.ShapeVertexBufferPtr++;
				}
				vertexBase += 4;
				indexBase += 6;
			}
		}

		s_Data.ShapeVertexCount += 24;
		s_Data.ShapeIndexCount += 36;
	}

	void Renderer::DrawConvex(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const std::vector<glm::vec3>& vertexCoords, const  std::vector<Triangle>& tris, const glm::vec4& colour, const glm::vec3* normals)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
			glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
			glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawConvex(transform, vertexCoords, tris, colour, normals);
	}

	void Renderer::DrawConvex(const glm::mat4 transform, const std::vector<glm::vec3>& vertexCoords, const std::vector<Triangle>& tris, const glm::vec4& colour, const glm::vec3* normals)
	{
		if (s_Data.ShapeIndexCount >= RendererConfig::MaxIndices)
		{
			Flush();
			PrepareNextBatch();
		}

		float textureIndex = 0.0f;

		//if (texture)
		//{
		//	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		//	{
		//		if (*s_Data.TextureSlots[i] == *texture)
		//		{
		//			textureIndex = (float)i;
		//			break;
		//		}
		//	}
		//	if (textureIndex == 0.0f)
		//	{
		//		if (s_Data.TextureSlotIndex >= RendererConfig::MaxTextureSlots)
		//		{
		//			Flush();
		//			PrepareNextBatch();
		//		}
		//		textureIndex = (float)s_Data.TextureSlotIndex;
		//		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		//		s_Data.TextureSlotIndex++;
		//	}
		//

		if (normals != nullptr)
		{
			glm::vec4 adjustedColour;
			for (Triangle tri : tris)
			{
				s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[tri.a], 1.0f };
				//s_Data.ShapeVertexBufferPtr->Colour = colour;
				adjustedColour = colour * glm::vec4(normals[tri.a], 1.0f);
				adjustedColour.a = colour.a;
				s_Data.ShapeVertexBufferPtr->Colour = adjustedColour;
				//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
				s_Data.ShapeVertexBufferPtr++;

				s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[tri.b], 1.0f };
				//s_Data.ShapeVertexBufferPtr->Colour = colour;
				adjustedColour = colour * glm::vec4(normals[tri.b], 1.0f);
				adjustedColour.a = colour.a;
				s_Data.ShapeVertexBufferPtr->Colour = adjustedColour;
				//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
				s_Data.ShapeVertexBufferPtr++;

				s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[tri.c], 1.0f };
				//s_Data.ShapeVertexBufferPtr->Colour = colour;
				adjustedColour = colour * glm::vec4(normals[tri.c], 1.0f);
				adjustedColour.a = colour.a;
				s_Data.ShapeVertexBufferPtr->Colour = adjustedColour;
				//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
				s_Data.ShapeVertexBufferPtr++;
			}
		}
		else
		{
			for (Triangle tri : tris)
			{
				s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[tri.a], 1.0f };
				s_Data.ShapeVertexBufferPtr->Colour = colour;
				//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
				s_Data.ShapeVertexBufferPtr++;

				s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[tri.b], 1.0f };
				s_Data.ShapeVertexBufferPtr->Colour = colour;
				//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
				s_Data.ShapeVertexBufferPtr++;

				s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ vertexCoords[tri.c], 1.0f };
				s_Data.ShapeVertexBufferPtr->Colour = colour;
				//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
				s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
				s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
				//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
				s_Data.ShapeVertexBufferPtr++;
			}
		}

		uint32_t indexCount = static_cast<uint32_t>(tris.size()) * 3;
		uint32_t vertexBase = s_Data.ShapeVertexCount;
		uint32_t indexBase = s_Data.ShapeIndexCount;

		for (uint32_t offset = 0; offset < indexCount; offset++)
		{
			s_Data.ShapeIndexBufferPtr[indexBase+offset] = vertexBase + offset;
		}

		s_Data.ShapeVertexCount += indexCount;
		s_Data.ShapeIndexCount += indexCount;
	}

	void Renderer::DrawSphere(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const float radius, const int divisions, const float tilingFactor, const Ref<Texture>& texture, const glm::vec4& colour)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
			glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
			glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawSphere(transform, radius, divisions, tilingFactor, texture, colour);
	}

	void Renderer::DrawSphere(const glm::mat4 transform, const float radius, const int divisions, const float tilingFactor, const Ref<Texture>& texture, const glm::vec4& colour)
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

		const int numVertsOneFace = (divisions + 1) * (divisions + 1);
		//All the points on one face
		std::vector<glm::vec3> v = std::vector<glm::vec3>();
		for (int y = 0; y < divisions + 1; y++) {
			for (int x = 0; x < divisions + 1; x++) {
				float xf = ((float)x / (float)divisions) - 0.5f;
				float yf = ((float)y / (float)divisions) - 0.5f;
				v.push_back(glm::normalize(glm::vec3(xf, yf, 0.5f)) * radius);
			}
		}

		uint32_t indexBase = s_Data.ShapeIndexCount;
		uint32_t vertexBase = s_Data.ShapeVertexCount;

		int offset = 0;
		for (int i = 0; i < 6; i++)
		{
			for (int y = 0; y < divisions; y++) {
				for (int x = 0; x < divisions; x++) {
					int y0 = y;
					int y1 = y + 1;
					int x0 = x;
					int x1 = x + 1;

					s_Data.ShapeIndexBufferPtr[indexBase + offset + 0] = vertexBase + i * numVertsOneFace + y0 * (divisions + 1) + x0;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 1] = vertexBase + i * numVertsOneFace + y1 * (divisions + 1) + x0;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 2] = vertexBase + i * numVertsOneFace + y1 * (divisions + 1) + x1;

					s_Data.ShapeIndexBufferPtr[indexBase + offset + 3] = vertexBase + i * numVertsOneFace + y0 * (divisions + 1) + x0;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 4] = vertexBase + i * numVertsOneFace + y1 * (divisions + 1) + x1;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 5] = vertexBase + i * numVertsOneFace + y0 * (divisions + 1) + x1;

					offset += 6;
				}
			}
		}

		s_Data.ShapeIndexCount += 6 * 6 * divisions * divisions;

		for (int side = 0; side < 6; side++) {
			const glm::quat& orient = s_Data.CubeRotation[side];

			for (int y = 0; y < divisions + 1; y++) {
				for (int x = 0; x < divisions + 1; x++) {
					float xf = ((float)x / (float)divisions);
					float yf = ((float)y / (float)divisions);

					s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ orient * v[y * (divisions + 1) + x], 1.0f};
					s_Data.ShapeVertexBufferPtr->Colour = colour;
					s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2(xf, yf);
					s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
					s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
					s_Data.ShapeVertexBufferPtr++;
				}
			}
		}

		s_Data.ShapeVertexCount += 6 * numVertsOneFace;
	}

	void Renderer::DrawSphere(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const std::vector<glm::vec3>& vertexCoords, const int divisions, const float tilingFactor, const Ref<Texture>& texture, const glm::vec4& colour, const glm::vec3* normals)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
			glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
			glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawSphere(transform, vertexCoords, divisions, tilingFactor, texture, colour, normals);
	}

	void Renderer::DrawSphere(const glm::mat4 transform, const std::vector<glm::vec3>& vertexCoords, const int divisions, const float tilingFactor, const Ref<Texture>& texture, const glm::vec4& colour, const glm::vec3* normals)
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

		const int numVertsOneFace = (divisions + 1) * (divisions + 1);

		uint32_t indexBase = s_Data.ShapeIndexCount;
		uint32_t vertexBase = s_Data.ShapeVertexCount;

		int offset = 0;
		for (int i = 0; i < 6; i++)
		{
			for (int y = 0; y < divisions; y++) {
				int y0 = y;
				int y1 = y + 1;
				for (int x = 0; x < divisions; x++) {
					int x0 = x;
					int x1 = x + 1;

					s_Data.ShapeIndexBufferPtr[indexBase + offset + 0] = vertexBase + i * numVertsOneFace + y0 * (divisions + 1) + x0;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 1] = vertexBase + i * numVertsOneFace + y1 * (divisions + 1) + x0;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 2] = vertexBase + i * numVertsOneFace + y1 * (divisions + 1) + x1;

					s_Data.ShapeIndexBufferPtr[indexBase + offset + 3] = vertexBase + i * numVertsOneFace + y0 * (divisions + 1) + x0;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 4] = vertexBase + i * numVertsOneFace + y1 * (divisions + 1) + x1;
					s_Data.ShapeIndexBufferPtr[indexBase + offset + 5] = vertexBase + i * numVertsOneFace + y0 * (divisions + 1) + x1;

					offset += 6;
				}
			}

		}

		s_Data.ShapeIndexCount += 6 * 6 * divisions * divisions;

		for (int side = 0; side < 6; side++) {

			for (int y = 0; y < divisions + 1; y++) {
				for (int x = 0; x < divisions + 1; x++) {
					float xf = ((float)x / (float)divisions);
					float yf = ((float)y / (float)divisions);

					s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{  vertexCoords[side * numVertsOneFace + y * (divisions + 1) + x], 1.0f };
					glm::vec4 adjustedColour = colour;
					if (normals != nullptr)
					{
						adjustedColour = colour * glm::vec4(normals[side * numVertsOneFace + y * (divisions + 1) + x], 1.0f);
						adjustedColour.a = colour.a;
					}
					s_Data.ShapeVertexBufferPtr->Colour = adjustedColour;
					s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2(xf, yf);
					s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
					s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
					s_Data.ShapeVertexBufferPtr++;
				}
			}
		}

		s_Data.ShapeVertexCount += 6 * numVertsOneFace;
	}
}
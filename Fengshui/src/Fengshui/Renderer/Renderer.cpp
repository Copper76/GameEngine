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
		static const uint32_t MaxIndices = 10000;
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

		uint32_t indexOffset = s_Data.ShapeIndexCount;
		uint32_t vertexOffset = s_Data.ShapeVertexCount;
		for (uint32_t face = 0; face < 6; face++)
		{

			s_Data.ShapeIndexBufferPtr[indexOffset + 0] = vertexOffset + 0;
			s_Data.ShapeIndexBufferPtr[indexOffset + 1] = vertexOffset + 1;
			s_Data.ShapeIndexBufferPtr[indexOffset + 2] = vertexOffset + 2;

			s_Data.ShapeIndexBufferPtr[indexOffset + 3] = vertexOffset + 2;
			s_Data.ShapeIndexBufferPtr[indexOffset + 4] = vertexOffset + 3;
			s_Data.ShapeIndexBufferPtr[indexOffset + 5] = vertexOffset + 0;

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
			vertexOffset += 4;
			indexOffset += 6;
		}

		s_Data.ShapeVertexCount += 24;
		s_Data.ShapeIndexCount += 36;
	}

	void Renderer::DrawConvex(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const std::vector<glm::vec3>& vertexCoords, const  std::vector<Triangle>& tris, const glm::vec4& colour)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
			glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
			glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawConvex(transform, vertexCoords, tris, colour);
	}

	void Renderer::DrawConvex(const glm::mat4 transform, const std::vector<glm::vec3>& vertexCoords, const std::vector<Triangle>& tris, const glm::vec4& colour)
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

	void Renderer::DrawSphere(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const float radius, const int divisions, const glm::vec4& colour)
	{
		glm::mat4 transform;
		transform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
			glm::radians(rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
			glm::radians(rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f }), size);
		DrawSphere(transform, radius, divisions, colour);
	}

	void Renderer::DrawSphere(const glm::mat4 transform, const float radius, const int divisions, const glm::vec4& colour)
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

		const int numnum = (divisions + 1) * (divisions + 1);
		glm::vec3* v = new glm::vec3[numnum];
		for (int y = 0; y < divisions + 1; y++) {
			for (int x = 0; x < divisions + 1; x++) {
				float xf = (((float)x / (float)divisions) - 0.5f) * 2.0f;
				float yf = (((float)y / (float)divisions) - 0.5f) * 2.0f;
				v[y * (divisions + 1) + x] = glm::vec3(xf, yf, 1.0f);
			}
		}

		const int numFaces = divisions * divisions;

		int faceIdx = 0;
		int* faceIdxs = new int[3 * 2 * numFaces + 1];
		for (int y = 0; y < divisions; y++) {
			for (int x = 0; x < divisions; x++) {
				int y0 = y;
				int y1 = y + 1;
				int x0 = x;
				int x1 = x + 1;

				faceIdxs[faceIdx * 6 + 1] = y0 * (divisions + 1) + x0;
				faceIdxs[faceIdx * 6 + 0] = y1 * (divisions + 1) + x0;
				faceIdxs[faceIdx * 6 + 2] = y1 * (divisions + 1) + x1;

				faceIdxs[faceIdx * 6 + 4] = y0 * (divisions + 1) + x0;
				faceIdxs[faceIdx * 6 + 3] = y1 * (divisions + 1) + x1;
				faceIdxs[faceIdx * 6 + 5] = y0 * (divisions + 1) + x1;

				faceIdx++;
			}
		}

		glm::mat3 matOrients[6];
		for (int i = 0; i < 6; i++) {
			matOrients[i] = glm::mat3();
		}
		// px
		matOrients[0][0] = glm::vec3(0.0f, 0.0f, 1.0f);
		matOrients[0][1] = glm::vec3(1.0f, 0.0f, 0.0f);
		matOrients[0][2] = glm::vec3(0.0f, 1.0f, 0.0f);
		// nx
		matOrients[1][0] = glm::vec3(0.0f, 0.0f, -1.0f);
		matOrients[1][1] = glm::vec3(-1.0f, 0.0f, 0.0f);
		matOrients[1][2] = glm::vec3(0.0f, 1.0f, 0.0f);

		// py
		matOrients[2][0] = glm::vec3(1.0f, 0.0f, 0.0f);
		matOrients[2][1] = glm::vec3(0.0f, 0.0f, 1.0f);
		matOrients[2][2] = glm::vec3(0.0f, -1.0f, 0.0f);
		// ny
		matOrients[3][0] = glm::vec3(1.0f, 0.0f, 0.0f);
		matOrients[3][1] = glm::vec3(0.0f, 0.0f, -1.0f);
		matOrients[3][2] = glm::vec3(0.0f, 1.0f, 0.0f);

		// pz
		matOrients[4][0] = glm::vec3(1.0f, 0.0f, 0.0f);
		matOrients[4][1] = glm::vec3(0.0f, 1.0f, 0.0f);
		matOrients[4][2] = glm::vec3(0.0f, 0.0f, 1.0f);
		// nz
		matOrients[5][0] = glm::vec3(-1.0f, 0.0f, 0.0f);
		matOrients[5][1] = glm::vec3(0.0f, 1.0f, 0.0f);
		matOrients[5][2] = glm::vec3(0.0f, 0.0f, -1.0f);

		const int numIdxs = 3 * 2 * 6 * numFaces;
		const int numVerts = 4 * 6 * numFaces;
		int* cubeIdxs = new int[numIdxs];
		glm::vec3* cubeVerts = new glm::vec3[numVerts];

		for (int side = 0; side < 6; side++) {
			const glm::mat3& mat = matOrients[side];

			for (int vid = 0; vid < numnum; vid++) {
				const glm::vec3 xyz = mat * v[vid];

				cubeVerts[side * numnum + vid] = glm::normalize(xyz) * radius;
			}

			for (int idx = 0; idx < 3 * 2 * numFaces; idx++) {
				const int offset = 3 * 2 * numFaces * side;
				cubeIdxs[idx + offset] = faceIdxs[idx] + numnum * side;
			}
		}

		for (int i = 0;i<numVerts;i++)
		{
			s_Data.ShapeVertexBufferPtr->Position = transform * glm::vec4{ cubeVerts[i], 1.0f};
			s_Data.ShapeVertexBufferPtr->Colour = colour;
			//s_Data.ShapeVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.ShapeVertexBufferPtr->TexCoord = glm::vec2{ 0.0f };
			s_Data.ShapeVertexBufferPtr->TextureIndex = textureIndex;
			//s_Data.ShapeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.ShapeVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.ShapeVertexBufferPtr++;
		}

		uint32_t vertexBase = s_Data.ShapeVertexCount;
		uint32_t indexBase = s_Data.ShapeIndexCount;

		for (uint32_t offset = 0; offset < numIdxs; offset++)
		{
			s_Data.ShapeIndexBufferPtr[indexBase + offset] = vertexBase + cubeIdxs[offset];
		}

		s_Data.ShapeVertexCount += numVerts;
		s_Data.ShapeIndexCount += numIdxs;

		free(v);
		free(faceIdxs);
		free(cubeVerts);
		free(cubeIdxs);
	}
}
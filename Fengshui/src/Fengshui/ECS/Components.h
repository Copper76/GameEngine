#pragma once

#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"
#include "Fengshui/Renderer/SubTexture2D.h"
#include "Fengshui/Renderer/Camera.h"

#include <set>

namespace Fengshui
{
	//Overall preparation
	enum class RenderShape
	{
		Custom, Quad,
	};

	struct Transform2D
	{
		glm::vec2 Position = glm::vec2(0.0f);
		float Depth = 0.0f;
		float Rotation = 0.0f;
		glm::vec2 Scale = glm::vec2(1.0f);

		Transform2D()
		{

		}

		Transform2D(glm::vec2 pos)
		{
			Position = pos;
		}

		Transform2D(glm::vec2 pos, float depth, float rotation, glm::vec2 scale)
		{
			Position = pos;
			Depth = depth;
			Rotation = rotation;
			Scale = scale;
		}
	};

	struct Transform
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);
	};

	struct Render2D
	{
		Ref<Texture2D> Texture;
		glm::vec2* TexCoords;
		glm::vec4 Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		RenderShape Shape = RenderShape::Quad;
	};

	struct Hierarchy
	{
		uint32_t Parent = -1;
		std::set<uint32_t> Children = {};

		Hierarchy()
		{

		}

		Hierarchy(uint32_t parent)
		{
			Parent = parent;
		}
	};

	struct CameraComponent
	{
		float ZoomLevel = 1.0f;
		float CameraMoveSpeed = 2.0f;
		glm::vec3 m_CameraPos = glm::vec3(0.0f);

		float AspectRatio = 1280.0f / 720.0f;
		Ref<OrthographicCamera> Camera;
	};
}
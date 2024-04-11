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

	struct Tag
	{
		std::string Name = "GameEntity";

		Tag() = default;

		Tag(const std::string& name) : Name(name)
		{

		}
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
			: Position(pos), Depth(depth), Rotation(rotation), Scale(scale)
		{

		}
	};

	struct Transform
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		Transform()
		{

		}
	};

	struct Render2D
	{
		Ref<Texture2D> Texture = nullptr;
		glm::vec2* TexCoords = nullptr;
		glm::vec4 Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		RenderShape Shape = RenderShape::Quad;
		
		Render2D()
		{

		}

		Render2D(Ref<Texture2D> texture) : Texture(texture)
		{

		}

		Render2D(Ref<Texture2D> texture, glm::vec2* coords) : Texture(texture)
		{
			TexCoords = new glm::vec2[4];
			for (int i = 0; i < 4; i++)
			{
				TexCoords[i] = coords[i];
			}
		}

		Render2D(glm::vec4 colour) : Colour(colour)
		{

		}

		Render2D(Ref<Texture2D> texture, glm::vec2* coords, glm::vec4 colour, float tilingFactor, RenderShape shape) 
			: Texture(texture), Colour(colour), TilingFactor(tilingFactor), Shape(shape)
		{
			TexCoords = new glm::vec2[4];
			for (int i = 0; i < 4; i++)
			{
				TexCoords[i] = coords[i];
			}
		}

		~Render2D()
		{
			//delete[] TexCoords;
		}
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
		bool Primary;
		float ZoomLevel = 1.0f;
		glm::vec3 CameraPos = glm::vec3(0.0f);

		float AspectRatio = 1280.0f / 720.0f;
		Ref<OrthographicCamera> Camera;

		CameraComponent()
		{
			Camera = std::make_shared<OrthographicCamera>(-1.0f * 1280.0f / 720.0f * 1.0f, 1.0f * 1280.0f / 720.0f * 1.0f, -1.0f * 1.0f, 1.0f * 1.0f);
			Primary = false;
		}

		CameraComponent(bool primary, float zoomLevel, glm::vec3 cameraPos, float aspectRatio, Ref<OrthographicCamera> camera) 
			: Primary(primary), ZoomLevel(zoomLevel), CameraPos(cameraPos), AspectRatio(aspectRatio), Camera(camera)
		{

		}
	};
}
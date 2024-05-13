#pragma once

#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"
#include "Fengshui/Renderer/SubTexture2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include <set>

namespace Fengshui
{
	//Overall preparation
	enum class RenderShape2D
	{
		Custom, Quad
	};

	enum class RenderShape
	{
		Custom, Cube
	};

	struct Tag
	{
		std::string Name = "GameEntity";

		Tag() = default;

		Tag(const std::string& name) : Name(name)
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

		Transform(glm::vec3 pos) : Position(pos)
		{

		}

		Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f))
			: Position(pos), Rotation(rotation), Scale(scale)
		{

		}

		glm::mat4 GetTransform()
		{
			return glm::scale(GetRotationMatrix() * glm::translate(glm::mat4(1.0f), Position), Scale);
		}

		glm::mat4 GetRotationMatrix()
		{
			return glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f }),
				glm::radians(Rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f }),
				glm::radians(Rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f });
		}
	};

	struct Transform2D
	{
		glm::vec3 Position = glm::vec3(0.0f);
		float Rotation = 0.0f;
		glm::vec2 Scale = glm::vec2(1.0f);

		Transform2D()
		{

		}

		Transform2D(glm::vec2 pos)
		{
			Position = glm::vec3(pos.x, pos.y, 0.0f);
		}

		Transform2D(glm::vec3 pos) : Position(pos)
		{

		}

		Transform2D(glm::vec2 pos, float depth, float rotation, glm::vec2 scale)
			: Rotation(rotation), Scale(scale)
		{
			Position = glm::vec3(pos.x, pos.y, depth);
		}

		Transform2D(glm::vec3 pos, float rotation, glm::vec2 scale)
			: Position(pos), Rotation(rotation), Scale(scale)
		{

		}

		glm::mat4 GetTransform()
		{
			if (Rotation == 0)
			{
				return glm::scale(glm::translate(glm::mat4(1.0f), Position), { Scale.x, Scale.y, 1.0f });
			}
			else
			{
				return glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), Position), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }), { Scale.x, Scale.y, 1.0f });
			}
		}
	};

	struct Render2D
	{
		Ref<Texture2D> Texture = nullptr;
		glm::vec2* TexCoords = nullptr;
		glm::vec4 Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		RenderShape2D Shape = RenderShape2D::Quad;
		
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

		Render2D(Ref<Texture2D> texture, glm::vec2* coords, glm::vec4 colour, float tilingFactor, RenderShape2D shape) 
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

	struct Render
	{
		Ref<Texture> Texture = nullptr;
		glm::vec2* TexCoords = nullptr;
		glm::vec4 Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		RenderShape Shape = RenderShape::Cube;

		Render()
		{

		}

		Render(Ref<Fengshui::Texture> texture) : Texture(texture)
		{

		}

		Render(Ref<Fengshui::Texture> texture, glm::vec2* coords) : Texture(texture)
		{
			TexCoords = new glm::vec2[4];
			for (int i = 0; i < 4; i++)
			{
				TexCoords[i] = coords[i];
			}
		}

		Render(glm::vec4 colour) : Colour(colour)
		{

		}

		Render(Ref<Fengshui::Texture> texture, glm::vec2* coords, glm::vec4 colour, float tilingFactor, RenderShape shape)
			: Texture(texture), Colour(colour), TilingFactor(tilingFactor), Shape(shape)
		{
			TexCoords = new glm::vec2[4];
			for (int i = 0; i < 4; i++)
			{
				TexCoords[i] = coords[i];
			}
		}
	};

	struct Hierarchy
	{
		uint32_t Parent = 0;
		std::set<uint32_t> Children;

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
		//Settings
		bool Primary;
		float AspectRatio = 1280.0f / 720.0f;
		bool IsOrtho;
		float NearPlane = -1.0f;
		float FarPlane = 1.0f;

		//Common Derived
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix = 1.0f;
		glm::mat4 ViewProjectionMatrix;

		//Orthographic
		float ZoomLevel = 1.0f;

		//Perspective
		float FOV = 45.0f;

		CameraComponent(bool isOrtho = true, bool isPrimary = false)
		{
			if (isOrtho)
			{
				ProjectionMatrix = glm::ortho(-1.0f * AspectRatio, 1.0f * AspectRatio, -1.0f, 1.0f, NearPlane, FarPlane);
			}
			else
			{
				ProjectionMatrix = glm::perspective(glm::radians(FOV), AspectRatio, NearPlane, FarPlane);
				ViewMatrix = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}

			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			Primary = isPrimary;
			IsOrtho = isOrtho;
		}

		CameraComponent(bool primary, float zoomLevel, float aspectRatio)
			: Primary(primary), ZoomLevel(zoomLevel), AspectRatio(aspectRatio),
			ProjectionMatrix(glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f))
		{
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			IsOrtho = true;
		}

		CameraComponent(bool primary, float fov, float aspectRatio, float nearPlane, float farPlane)
			: Primary(primary), FOV(fov), AspectRatio(aspectRatio), NearPlane(nearPlane), FarPlane(farPlane), ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane))
		{
			ViewMatrix = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			IsOrtho = false;
		}
	};
}
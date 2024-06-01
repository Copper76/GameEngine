#pragma once

#include "Fengshui/Renderer/RenderShapes/RenderShapes.h"
#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"
#include "Fengshui/Renderer/SubTexture2D.h"

#include "Fengshui/Physics/Shapes.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <set>

namespace Fengshui
{
	//Overall preparation
	enum class RenderShape2D
	{
		Custom, Quad
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
		glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		Transform()
		{

		}

		Transform(glm::vec3 pos) : Position(pos)
		{

		}

		Transform(glm::vec3 pos, glm::quat rotation, glm::vec3 scale = glm::vec3(1.0f))
			: Position(pos), Rotation(rotation), Scale(scale)
		{

		}

		Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f))
			: Position(pos), Scale(scale)
		{
			glm::vec3 radians = glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z));
			Rotation = glm::quat(radians);
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
	};

	struct Rigidbody
	{
		glm::vec3 LinearVelocity = glm::vec3(0.0f);
		glm::vec3 AngularVelocity = glm::vec3(0.0f);

		float InvMass = 1.0f;
		float Elasticity = 0.5f;
		float Friction = 0.5f;
		glm::vec3 Gravity = glm::vec3(0.0f, -10.0f, 0.0f);

		Rigidbody()
		{
			
		}

		Rigidbody(float mass)
		{
			InvMass = mass == 0.0f ? 0.0f : 1.0f / mass;
		}
	};

	struct Collider
	{
		glm::vec3 Offset = glm::vec3(0.0f);
		glm::vec3 Size = glm::vec3(1.0f);
		PhysicalShape* Shape = nullptr;

		Collider()
		{
			Shape = new PhysicalShapeBox();
		}

		Collider(PhysicalShape* shape) : Shape(shape)
		{

		}

		Collider(PhysicalShape* shape, glm::vec3 offset, glm::vec3 size) : Shape(shape), Offset(offset), Size(size)
		{
			Shape->Rebuild(Offset, Size);
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

		}
	};

	struct Render
	{
		Ref<Texture> Texture = nullptr;
		glm::vec2* TexCoords = nullptr;
		glm::vec4 Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		RenderShape* Shape = nullptr;

		Render()
		{
			Shape = new RenderShapeCube();
		}

		Render(RenderShape* shape) : Shape(shape)
		{

		}

		Render(Ref<Fengshui::Texture> texture) : Texture(texture)
		{
			Shape = new RenderShapeCube();
		}

		Render(Ref<Fengshui::Texture> texture, glm::vec2* coords) : Texture(texture)
		{
			TexCoords = new glm::vec2[4];
			for (int i = 0; i < 4; i++)
			{
				TexCoords[i] = coords[i];
			}

			Shape = new RenderShapeCube();
		}

		Render(glm::vec4 colour) : Colour(colour)
		{
			Shape = new RenderShapeCube();
		}

		Render(Ref<Fengshui::Texture> texture, glm::vec2* coords, glm::vec4 colour, float tilingFactor, RenderShape* shape)
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
		float OrthoNearPlane = -1.0f;
		float OrthoFarPlane = 1.0f;
		float PersNearPlane = 0.1f;
		float PersFarPlane = 1000.0f;

		//Common Derived
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix = 0.0f;
		glm::mat4 ViewProjectionMatrix;

		//Orthographic
		float ZoomLevel = 1.0f;

		//Perspective
		float FOV = 45.0f;

		CameraComponent(bool isOrtho = true, bool isPrimary = false)
		{
			if (isOrtho)
			{
				ProjectionMatrix = glm::ortho(-1.0f * AspectRatio, 1.0f * AspectRatio, -1.0f, 1.0f, OrthoNearPlane, OrthoFarPlane);
			}
			else
			{
				ProjectionMatrix = glm::perspective(glm::radians(FOV), AspectRatio, PersNearPlane, PersFarPlane);
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
			: Primary(primary), FOV(fov), AspectRatio(aspectRatio), PersNearPlane(nearPlane), PersFarPlane(farPlane), ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane))
		{
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			IsOrtho = false;
		}
	};
}
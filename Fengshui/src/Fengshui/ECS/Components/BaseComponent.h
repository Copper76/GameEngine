#pragma once
#include "Fengshui/Events/Event.h"

namespace Fengshui
{
	enum class ComponentType : int
	{
		None = 0,
		ComponentHierarchy, ComponentTransform, ComponentRender, ComponentCamera,
	};

	#define COMPONENT_CLASS_TYPE(type) static ComponentType GetStaticType() { return ComponentType::type; }\
									virtual ComponentType GetComponentType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }
	class Component
	{
	public:
		Component();

		virtual ComponentType GetComponentType() const = 0;
		virtual const char* GetName() const = 0;

		virtual void OnUpdate(float dt) {};

		virtual void OnEvent(Event& e) {};

		const uint32_t GetEntityID() const { return m_EntityID; }
		//const uint32_t GetComponentID() const { return m_ComponentID; }

		inline void SetEntityID(const uint32_t id) { m_EntityID = id; }
		//inline void SetComponentID(const uint32_t id) { m_ComponentID = id; }

	protected:
		uint32_t m_EntityID;
		//uint32_t m_ComponentID;
		//Ref<Entity> m_Entity//Should it reference the entity?
	};
}
#pragma once

namespace Fengshui
{
	enum class ComponentType
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

		const uint32_t GetEntityID() const { return m_EntityID; }

		inline void SetEntityID(const uint32_t id) { m_EntityID = id; }

	protected:
		uint32_t m_EntityID;
		//Ref<Entity> m_Entity//Should it reference the entity?
	};
}
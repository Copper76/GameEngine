#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"

namespace Fengshui
{
	class HierarchyComponent : public Component, public std::enable_shared_from_this<HierarchyComponent>
	{
	public:
		HierarchyComponent();

		virtual ~HierarchyComponent() = default;

		COMPONENT_CLASS_TYPE(ComponentHierarchy);

		Ref<HierarchyComponent> GetParent();
		void SetParent(Ref<HierarchyComponent> parent);

		std::vector<Ref<HierarchyComponent>> GetChildren();
		Ref<HierarchyComponent> GetChild(Ref<HierarchyComponent> child);

	private:
		void AddChild(Ref<HierarchyComponent> child);
		void RemoveChild(Ref<HierarchyComponent> child);

	private:
		WeakRef<HierarchyComponent> m_Parent;
		std::vector<Ref<HierarchyComponent>> m_Children;
	};
}


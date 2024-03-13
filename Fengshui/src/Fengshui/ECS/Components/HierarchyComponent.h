#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"

namespace Fengshui
{
	class HierarchyComponent : public Component
	{
	public:
		HierarchyComponent();

		virtual ~HierarchyComponent() = default;

		COMPONENT_CLASS_TYPE(ComponentHierarchy);

		Ref<HierarchyComponent> GetParent();
		void SetParent(Ref<HierarchyComponent> parent);

		std::vector<Ref<HierarchyComponent>> GetChildren();
		Ref<HierarchyComponent> GetChild(Ref<HierarchyComponent> child);
		void AddChild(Ref<HierarchyComponent> child);
		void RemoveChild(Ref<HierarchyComponent> child);

	private:
		Ref<HierarchyComponent> m_Parent;
		std::vector<Ref<HierarchyComponent>> m_Children;
		Ref<HierarchyComponent> self;
	};
}


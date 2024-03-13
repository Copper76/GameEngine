#include "fspch.h"
#include "HierarchyComponent.h"

namespace Fengshui
{
	Fengshui::HierarchyComponent::HierarchyComponent()
	{
		self.reset(this);
	}

	Ref<HierarchyComponent> Fengshui::HierarchyComponent::GetParent()
	{
		return m_Parent;
	}

	void HierarchyComponent::SetParent(Ref<HierarchyComponent> parent)
	{
		if (parent != m_Parent)
		{
			if (parent)
			{
				parent->RemoveChild(self);
			}
			m_Parent = parent;
			m_Parent->AddChild(self);
		}
	}

	std::vector<Ref<HierarchyComponent>> Fengshui::HierarchyComponent::GetChildren()
	{
		return m_Children;
	}

	Ref<HierarchyComponent> HierarchyComponent::GetChild(Ref<HierarchyComponent> child)
	{
		auto it = find(m_Children.begin(), m_Children.end(), child);
		if (it != m_Children.end())
		{
			return *it;
		}
		return nullptr;
	}

	void HierarchyComponent::AddChild(Ref<HierarchyComponent> child)
	{
		auto it = find(m_Children.begin(), m_Children.end(), child);
		if (it == m_Children.end())
		{
			m_Children.emplace_back(child);
		}
	}

	void HierarchyComponent::RemoveChild(Ref<HierarchyComponent> child)
	{
		auto it = find(m_Children.begin(), m_Children.end(), child);
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}
	}
}

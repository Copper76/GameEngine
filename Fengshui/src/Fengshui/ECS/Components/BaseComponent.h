#pragma once

namespace Fengshui
{
	class Component
	{
	public:
		Component() : m_EntityID(0)
		{

		}

		Component(uint32_t entityID) : m_EntityID(entityID)
		{

		}

		const uint32_t GetEntityID() const { return m_EntityID; }

		inline void SetEntityID(const uint32_t id) { m_EntityID = id; }

	protected:
		uint32_t m_EntityID;
	};
}
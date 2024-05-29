#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class EntityManager
	{
	public:
		EntityManager()
		{
			for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				m_AvailableEntities.push(entity);
			}
		}

		EntityID Create()
		{
			FS_ENGINE_ASSERT(m_UsedEntityCount < MAX_ENTITIES, "Entity exceeded limit");

			EntityID id = m_AvailableEntities.front();
			m_AvailableEntities.pop();
			++m_UsedEntityCount;

			return id;
		}

		void Destroy(EntityID entity)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity >= 0, "Entity out of range");

			m_Signatures[entity].reset();

			m_AvailableEntities.push(entity);
			--m_UsedEntityCount;
		}

		void SetSignature(EntityID entity, Signature signature)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity >= 0, "Entity out of range");
			m_Signatures[entity] = signature;
		}

		Signature GetSignature(EntityID entity)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity >= 0, "Entity out of range");
			return m_Signatures[entity];
		}

	private:
		std::queue<EntityID> m_AvailableEntities;

		std::array<Signature, MAX_ENTITIES> m_Signatures;

		uint32_t m_UsedEntityCount = 0;
	};
}
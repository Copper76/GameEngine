#pragma once
#include "Fengshui/ECS/Components.h"

#include <bitset>
#include <queue>
#include <set>
#include <array>

#include <glm/glm.hpp>

namespace Fengshui
{
#pragma region Settings
	//Entity Settings
	using EntityID = uint32_t;

	const EntityID MAX_ENTITIES = 5000;

	//Component Settings
	using ComponentType = uint8_t;

	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
#pragma endregion

#pragma region Data Structure
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void OnEntityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(EntityID entity, T component)
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) return;//don't add component more than once

			size_t newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			++m_Size;
		}

		void RemoveData(EntityID entity)
		{
			if (m_Size == 0) return;
			if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end()) return;//the component doesn't exist

			size_t removeIndex = m_EntityToIndexMap[entity];
			size_t lastIndex = m_Size - 1;
			m_ComponentArray[removeIndex] = m_ComponentArray[lastIndex];

			EntityID lastEntity = m_IndexToEntityMap[lastIndex];
			m_EntityToIndexMap[lastEntity] = removeIndex;
			m_IndexToEntityMap[removeIndex] = lastEntity;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(lastIndex);

			--m_Size;
		}

		bool HasData(EntityID entity)
		{
			return m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end();
		}

		T& GetData(EntityID entity)
		{
			FS_ENGINE_ASSERT(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end(), "Component doesn't exist");//the component doesn't exist

			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void OnEntityDestroyed(EntityID entity) override
		{
			RemoveData(entity);//delegate the checking to later RemoveData
		}

	private:
		std::array<T, MAX_ENTITIES> m_ComponentArray;
		std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
		std::unordered_map<size_t, EntityID> m_IndexToEntityMap;

		size_t m_Size;
	};

	class System
	{
	public:
		std::set<EntityID> m_Entities;//Entities are stored as set in system
	};

#pragma endregion
}
#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class SystemManager
	{
	public:
		template<typename T>
		Ref<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			if (m_Systems.find(typeName) != m_Systems.end()) return std::static_pointer_cast<T>(m_Systems.find(typeName)->second);//System already registered

			auto system = std::make_shared<T>();
			m_Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		Ref<T> GetSystem()
		{
			const char* typeName = typeid(T).name();
			FS_ENGINE_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System was never registered");//Dangerous function
			return std::static_pointer_cast<T>(m_Systems[typeName]);
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			FS_ENGINE_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System was never registered");//Dangerous function

			m_Signatures.insert({ typeName, signature });
		}

		void OnEntityDestroyed(EntityID entity)
		{
			for (auto& pair : m_Systems)
			{
				auto const& system = pair.second;
				system->m_Entities.erase(entity);
			}
		}

		void OnEntitySignatureChanged(EntityID entity, Signature entitySignature) {
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->m_Entities.insert(entity);//add if matching
				}
				else
				{
					system->m_Entities.erase(entity);//remove if no longer matching
				}
			}
		}

	private:
		std::unordered_map<const char*, Signature> m_Signatures;
		std::unordered_map<const char*, Ref<System>> m_Systems;
	};
}
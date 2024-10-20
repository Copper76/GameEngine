#include "fspch.h"
#include "GameScriptSystem.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	void GameScriptSystem::Awake()
	{
		for (EntityID entity : m_Entities)
		{
			auto& script = GeneralManager::GetComponent<ScriptComponent>(entity);

			script.Awake();
		}
	}

	void GameScriptSystem::Start()
	{
		for (EntityID entity : m_Entities)
		{
			auto& script = GeneralManager::GetComponent<ScriptComponent>(entity);

			script.Start();
		}
	}
}
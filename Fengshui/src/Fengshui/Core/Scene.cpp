#include "fspch.h"
#include "Scene.h"

namespace Fengshui
{
	void Scene::Init()
	{

	}

	uint32_t Scene::CreateEntity()
	{
		GameEntity* entity = new GameEntity(m_NextEntityID);
		TransformComponent* transform = new TransformComponent(m_NextEntityID);
		transform->SetEntityID(m_NextEntityID);
		entity->AddComponent(transform);
		m_TransformComponents.emplace_back(transform);
		m_GameEntities.emplace_back(entity);
		return m_NextEntityID++;
	}

	void Scene::OnUpdate(float dt)
	{

	}
}
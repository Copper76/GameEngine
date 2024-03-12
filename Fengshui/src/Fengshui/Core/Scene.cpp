#include "fspch.h"
#include "Scene.h"

namespace Fengshui
{
	Scene::Scene()
	{
		m_CameraComponent = std::make_shared<CameraComponent>();
	}

	Ref<Scene> Scene::Init()
	{
		return std::make_shared<Scene>();
	}

	uint32_t Scene::RegisterEntity(GameEntity* entity)
	{
		m_GameEntities[m_NextEntityID] = entity;
		return m_NextEntityID++;
	}

	bool Scene::RegisterComponent(Ref<Component> component, uint32_t entityID)
	{
		if (m_Components[component->GetComponentType()][entityID] != nullptr)
		{
			return false;
		}
		component->SetEntityID(entityID);
		m_Components[component->GetComponentType()][entityID] = component;
		return true;
	}

	void Scene::RemoveComponent(Ref<Component> component, uint32_t entityID)
	{
		m_Components[component->GetComponentType()].erase(entityID);
	}

	void Scene::OnUpdate(float dt)
	{

	}
}
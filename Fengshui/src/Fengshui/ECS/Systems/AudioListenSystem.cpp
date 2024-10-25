#include "fspch.h"
#include "AudioListenSystem.h"

#include "Fengshui/ECS/GeneralManager.h"
#include "Fengshui/Core/Scene.h"

namespace Fengshui
{
	void AudioListenSystem::OnUpdate(const float dt)
	{
		UpdateAudioListenerTransform();
	}

	void AudioListenSystem::UpdateAudioListenerTransform()
	{
		for (EntityID entity : m_Entities)
		{
			auto& audioListener = GeneralManager::GetComponent<AudioListenerComponent>(entity);
			if (audioListener.IsListener)
			{
				auto& transformComp = GeneralManager::GetComponent<Transform>(entity);
				Ref<AudioListener> audioListener = GeneralManager::GetActiveScene()->GetSceneManager()->GetComponent<AudioListenerComponent>().Listener;
				audioListener->UpdateLocation(transformComp.Position);
				glm::mat3 rotationMatrix = glm::mat3_cast(transformComp.Rotation);;
				glm::vec3 forward = -rotationMatrix[2];
				glm::vec3 up = rotationMatrix[1];
				audioListener->UpdateOrientation(forward, up);
				break;
			}
		}
	}
}
#include "fspch.h"
#include "AudioPlayerSystem.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	void AudioPlayerSystem::OnUpdate(const float dt)
	{
		//will be used to implement logic such as refreshing buffer
	}

	void AudioPlayerSystem::PlayOnStart()
	{
		for (EntityID entity : m_Entities)
		{
			auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);

			if (audioSourceComp.Settings.PlayOnStart)
			{
				audioSourceComp.Source->Play();
			}
		}
	}

	void AudioPlayerSystem::StopAll()
	{
		for (EntityID entity : m_Entities)
		{
			auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);
			audioSourceComp.Source->Stop();
		}
	}

	void AudioPlayerSystem::SetSettings(EntityID entity, AudioSetting settings)
	{
		auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);
		audioSourceComp.Source->SetSettings(settings);
		audioSourceComp.Settings = settings;
	}

	void AudioPlayerSystem::Play(EntityID entity, bool restart)
	{
		auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);
		audioSourceComp.Source->Play(restart);
	}

	void AudioPlayerSystem::Stop(EntityID entity)
	{
		auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);
		audioSourceComp.Source->Stop();
	}

	void AudioPlayerSystem::Pause(EntityID entity)
	{
		auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);
		audioSourceComp.Source->Pause();
	}

	void AudioPlayerSystem::Unpause(EntityID entity)
	{
		auto& audioSourceComp = GeneralManager::GetComponent<AudioSourceComponent>(entity);
		audioSourceComp.Source->Unpause();
	}
}
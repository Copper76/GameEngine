#include "fspch.h"
#include "AudioPlayerSystem.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	void AudioPlayerSystem::OnUpdate(const float dt)
	{
		for (EntityID entity : m_Entities)
		{
			UpodateAudioSourceLocation(entity);
		}
	}

	void AudioPlayerSystem::UpodateAudioSourceLocation(EntityID entity)
	{
		auto& audioSource = GeneralManager::GetComponent<AudioSourceComponent>(entity);
		auto& transformComp = GeneralManager::GetComponent<Transform>(entity);
		audioSource.Source->UpdateLocation(transformComp.Position);
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
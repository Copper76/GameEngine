#include "fspch.h"
#include "OpenALAudioSource.h"
#include "OpenALAudioBufferManager.h"

#include <AL/al.h>

namespace Fengshui
{
	OpenALAudioSource::OpenALAudioSource(char* filePath)
	{
		Ref<OpenALAudioBuffer> buffer = OpenALAudioBufferManager::GetFileBuffer(filePath);
		if (buffer)
		{
			m_Buffer = buffer;
		}
		else
		{
			m_Buffer = MakeRef<OpenALAudioBuffer>(filePath);
			OpenALAudioBufferManager::AddFileMapping(filePath, m_Buffer); //register in the manager for future uses
		}

		Bind();
	}

	OpenALAudioSource::OpenALAudioSource(const Ref<OpenALAudioBuffer> buffer)
	{
		if (buffer)
		{
			m_Buffer = buffer;

			Bind();
		}
		else
		{
			FS_ERROR("The buffer is not valid");
		}
	}

	OpenALAudioSource::~OpenALAudioSource()
	{
		/*UnBind();*/ //I might need to call unbind from component as I need a reference to this for removing from manager
	}

	void OpenALAudioSource::Bind()
	{
		alGenSources(1, &m_AudioSourceID);

		int bufferNum = m_Buffer->GetBufferNum();
		if (bufferNum == 1)
		{
			alSourcei(m_AudioSourceID, AL_BUFFER, m_Buffer->GetBufferIDs()[0]);
		}
		else
		{
			alSourceQueueBuffers(m_AudioSourceID, bufferNum, m_Buffer->GetBufferIDs());
		}
	}

	void OpenALAudioSource::UnBind()
	{
		alDeleteSources(1, &m_AudioSourceID);

		OpenALAudioBufferManager::RemoveSourceMapping(shared_from_this(), m_Buffer);
	}

	void OpenALAudioSource::UpdateLocation(glm::vec3 position)
	{
		alSource3f(m_AudioSourceID, AL_POSITION, position.x, position.y, position.z);
	}

	void OpenALAudioSource::SetBuffer(Ref<OpenALAudioBuffer> buffer)
	{
		if (buffer)
		{
			m_Buffer = buffer;

			Bind();

			OpenALAudioBufferManager::AddSourceMapping(m_Buffer, shared_from_this()); //register in the manager for future uses
		}
		else
		{
			FS_ERROR("The buffer is not valid"); //unsuccessful bind but it is not problematic enough to break
		}
	}

	void OpenALAudioSource::SetSettings(AudioSetting settings)
	{
		alSourcei(m_AudioSourceID, AL_LOOPING, settings.IsLoop);

		alSourcef(m_AudioSourceID, AL_GAIN, settings.Volume);

		alSourcef(m_AudioSourceID, AL_REFERENCE_DISTANCE, settings.MinAttunmentDistance);
		alSourcef(m_AudioSourceID, AL_ROLLOFF_FACTOR,  settings.AttunmentRate);
		alSourcef(m_AudioSourceID, AL_PITCH,  settings.Pitch);
	}

	void OpenALAudioSource::Play(bool restart)
	{
		if (!restart)
		{
			ALint state;
			alGetSourcei(m_AudioSourceID, AL_SOURCE_STATE, &state);
			if (state == AL_PLAYING) return;
		}

		alSourcePlay(m_AudioSourceID);
	}

	void OpenALAudioSource::Stop()
	{
		alSourceStop(m_AudioSourceID);
	}

	void OpenALAudioSource::Pause()
	{
		alSourcePause(m_AudioSourceID);
	}

	void OpenALAudioSource::Unpause()
	{
		ALint state;
		alGetSourcei(m_AudioSourceID, AL_SOURCE_STATE, &state);

		if (state == AL_PAUSED)
		{
			alSourcePlay(m_AudioSourceID);
		}
	}
}
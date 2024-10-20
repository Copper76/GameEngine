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
		/*UnBind();*/ //because I will be calling unbind from component, it might not need to unbind again as it will lose all reference with unbind
	}

	void OpenALAudioSource::Bind()
	{
		alGenSources(1, &m_AudioSourceID);
		alSourcei(m_AudioSourceID, AL_BUFFER, m_Buffer->GetID());
	}

	void OpenALAudioSource::UnBind()
	{
		alDeleteSources(1, &m_AudioSourceID);

		OpenALAudioBufferManager::RemoveSourceMapping(shared_from_this(), m_Buffer);
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

	void OpenALAudioSource::SetSetting(AudioSetting setting)
	{
		m_AudioSetting = setting;
	}

	AudioSetting& OpenALAudioSource::GetSetting()
	{
		return m_AudioSetting;
	}

	void OpenALAudioSource::Play()
	{
		alSourcePlay(m_AudioSourceID);
	}
}
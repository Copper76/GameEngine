#include "fspch.h"
#include "OpenALAudioBuffer.h"

#include <AL/al.h>

namespace Fengshui
{
	OpenALAudioBuffer::OpenALAudioBuffer(const char* filePath)
	{
        Bind(filePath);
	}

	OpenALAudioBuffer::~OpenALAudioBuffer()
	{
        UnBind();
	}

    void OpenALAudioBuffer::Bind(const char* filePath)
    {
        short* audioData = AudioLoader::Load(filePath, m_AudioData);

        ALenum format;
        if (m_AudioData.Channels == 1 && m_AudioData.BitsPerSample == 8)
            format = AL_FORMAT_MONO8;
        else if (m_AudioData.Channels == 1 && m_AudioData.BitsPerSample == 16)
            format = AL_FORMAT_MONO16;
        else if (m_AudioData.Channels == 2 && m_AudioData.BitsPerSample == 8)
            format = AL_FORMAT_STEREO8;
        else if (m_AudioData.Channels == 2 && m_AudioData.BitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
        else
        {
            FS_ERROR("channels: {}, bits per sample: {}", m_AudioData.Channels, m_AudioData.BitsPerSample);
            FS_ASSERT(false, "ERROR: unrecognised wave format");
        }

        ALsizei size = m_AudioData.NumSamples * m_AudioData.Channels * (m_AudioData.BitsPerSample / 8);

        alGenBuffers(1, &m_AudioID);
        alBufferData(m_AudioID, format, audioData, size, m_AudioData.SampleRate);
    }

    void OpenALAudioBuffer::UnBind()
    {
        alDeleteBuffers(1, &m_AudioID);
    }
}
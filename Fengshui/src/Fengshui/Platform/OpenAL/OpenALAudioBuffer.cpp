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
        int bytesPerSample = (m_AudioData.BitsPerSample / 8);
        ALsizei dataSize = m_AudioData.NumSamples * m_AudioData.Channels * bytesPerSample;
        int bufferDurationMs = 250;  // Each buffer holds 250 ms of audio
        int bufferSize = (m_AudioData.SampleRate * bufferDurationMs / 1000) * bytesPerSample * m_AudioData.Channels;

        m_BufferNum = (dataSize + bufferSize - 1) / bufferSize;

        m_BufferIDs = new ALuint[m_BufferNum];

        alGenBuffers(m_BufferNum, m_BufferIDs);

        for (int i = 0; i < m_BufferNum; ++i) {
            int offset = i * bufferSize;
            int size = (i == m_BufferNum - 1) ? (dataSize - offset) : bufferSize;

            alBufferData(m_BufferIDs[i], format,
                audioData + offset / bytesPerSample, size, m_AudioData.SampleRate);
        }
    }

    void OpenALAudioBuffer::UnBind()
    {
        alDeleteBuffers(m_BufferNum, m_BufferIDs);
    }
}
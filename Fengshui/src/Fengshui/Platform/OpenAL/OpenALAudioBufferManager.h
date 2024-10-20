#pragma once

#include <set>
#include "OpenALAudioBuffer.h"
#include "OpenALAudioSource.h"

namespace Fengshui
{
	//This class manages a mapping from file name to buffer so we do not create duplicate buffers
	class OpenALAudioBufferManager
	{
	public:
		OpenALAudioBufferManager() = default;

		static const bool IsFileLoaded(char* filePath)
		{
			return m_Instance->m_FileToBufferMapping.find(filePath) != m_Instance->m_FileToBufferMapping.end();
		}

		static const Ref<OpenALAudioBuffer> GetFileBuffer(char* filePath)
		{
			if (m_Instance->m_FileToBufferMapping.find(filePath) == m_Instance->m_FileToBufferMapping.end())
			{
				return nullptr; //return nullptr to indicate that there is no buffer associated with the file
			}
			else {
				return m_Instance->m_FileToBufferMapping[filePath];
			}
		}

		static void AddFileMapping(char* filePath, Ref<OpenALAudioBuffer> buffer)
		{
			m_Instance->m_FileToBufferMapping.insert({ filePath, buffer });
			m_Instance->m_BufferToFileMapping.insert({ buffer, filePath });
		}

		static void RemoveFileMapping(Ref<OpenALAudioBuffer> buffer)
		{
			m_Instance->m_FileToBufferMapping.erase(m_Instance->m_BufferToFileMapping[buffer]);
			m_Instance->m_BufferToFileMapping.erase(buffer);
			if (m_Instance->m_BufferToSourceMapping.find(buffer) != m_Instance->m_BufferToSourceMapping.end())
			{
				std::set<Ref<OpenALAudioSource>> sources = m_Instance->m_BufferToSourceMapping[buffer];
				for (const Ref<OpenALAudioSource> source : sources)
				{
					source->SetBuffer(nullptr);
				}
				m_Instance->m_BufferToSourceMapping.erase(buffer);
			}
		}

		static void AddSourceMapping(Ref<OpenALAudioBuffer> buffer, Ref<OpenALAudioSource> source)
		{
			std::set<Ref<OpenALAudioSource>>& sourceSet = m_Instance->m_BufferToSourceMapping[buffer];

			sourceSet.insert(source);
		}

		static void RemoveSourceMapping(Ref<OpenALAudioSource> source, Ref<OpenALAudioBuffer> buffer)
		{
			if (m_Instance->m_BufferToSourceMapping.find(buffer) == m_Instance->m_BufferToSourceMapping.end())
			{
				return;
			}

			std::set<Ref<OpenALAudioSource>>& sourceSet = m_Instance->m_BufferToSourceMapping[buffer];
			sourceSet.erase(source);
			if (sourceSet.empty())
			{
				RemoveFileMapping(buffer);
			}
		}

	private:
		static Scope<OpenALAudioBufferManager> m_Instance;

		std::unordered_map<char*, Ref<OpenALAudioBuffer>> m_FileToBufferMapping;
		std::unordered_map<Ref<OpenALAudioBuffer>, char*> m_BufferToFileMapping;
		std::unordered_map<Ref<OpenALAudioBuffer>, std::set<Ref<OpenALAudioSource>>> m_BufferToSourceMapping;
	};
}
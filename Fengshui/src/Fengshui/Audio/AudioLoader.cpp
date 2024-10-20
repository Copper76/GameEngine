#include "fspch.h"
#include "AudioLoader.h"
#include <miniaudio.h>

namespace Fengshui
{
    short* AudioLoader::Load(const char* filePath, AudioData& data)
    {
        ma_decoder decoder;

        ma_result result = ma_decoder_init_file(filePath, NULL, &decoder);
        if (result != MA_SUCCESS) {
            FS_ASSERT(false, "Audio file load error");
            return nullptr;
        }

        ma_uint64 totalPCMFrameCount;

        if (ma_decoder_get_length_in_pcm_frames(&decoder, &totalPCMFrameCount) != MA_SUCCESS || totalPCMFrameCount == 0)
        {
            FS_ERROR(totalPCMFrameCount)
            FS_ASSERT(false, "Failed to identify data length.");
            return nullptr;
        }

        data.SampleRate = decoder.outputSampleRate;
        data.Channels = decoder.outputChannels;
        data.NumSamples = totalPCMFrameCount;

        switch (decoder.outputFormat)
        {
        case ma_format_u8:
            data.BitsPerSample = 8;
            break;
        case ma_format_s16:
            data.BitsPerSample = 16;
            break;
        case ma_format_s24:
            data.BitsPerSample = 24;
            break;
        case ma_format_s32:
            data.BitsPerSample = 32;
            break;
        case ma_format_f32:
            data.BitsPerSample = 32;
            break;
        default:
            FS_ASSERT(false, "Unknown bit format.");
            return nullptr;
        }

        size_t dataSize = totalPCMFrameCount * decoder.outputChannels * sizeof(short);
        short* audioData = (short*)malloc(dataSize);
        if (!audioData) {
            FS_ASSERT(false, "Failed to allocate memory for raw audio data.");
            ma_decoder_uninit(&decoder);
            return nullptr;
        }

        ma_decoder_read_pcm_frames(&decoder, audioData, totalPCMFrameCount, NULL);

        ma_decoder_uninit(&decoder);

        return audioData;
    }
}
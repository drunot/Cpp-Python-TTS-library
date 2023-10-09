/**
 * @file main.cpp
 * @author Anton Vigen Smolarz (anton@smolarz.dk)
 * @brief Simple example of how to use C++ Python TTS library on Linux
 * @version 1.0
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023
 */

#include"TTS.hpp"
#include<iostream>
#include<thread>
#include<alsa/asoundlib.h>

/**
 * @brief Naïve implementation of PlaySound equivalent from WinApi on Linux with ALSA.
 *
 * @param data_ The wave file in memory to play.
 */
void PlaySound(void* data_);

int main(int argc, char* argv[]) {
    std::wcout << L"Initializing TTS...\n";
    TTS::initialize();
    std::wcout << L"Available Models:\n";
    size_t model_num;
    auto names = TTS::list_models(model_num);
    
    for(size_t i = 0; i < model_num; ++i) {
        std::wcout << i << ": " << names[i] << L"\n";
    }
    
    TTS* tts = new TTS(names[10]);
    TTS::free_list(names, model_num);
    size_t data_size;
    auto data = tts->tts(data_size, L"This is a TTS test!");
    PlaySound(data);
    TTS::free_buffer(data);
    // Delete tts before finalization.
    delete tts;
    TTS::finalize();
}

void PlaySound(void* data_) {
    auto data = reinterpret_cast<unsigned char*>(data_);
    // This roughly reads the WAVE header.
    // It does not check if it is correct nor
    // if the fmt size is 16. It just assumes it.
    // It also assumes that the data is PCM.
    uint16_t sample_rate = static_cast<uint16_t>(data[24]) | (static_cast<uint16_t>(data[25]) << 8);
    uint16_t channels = static_cast<uint16_t>(data[22]) | (static_cast<uint16_t>(data[23]) << 8);
    uint32_t data_section_size = static_cast<uint32_t>(data[40]) | (static_cast<uint32_t>(data[41]) << 8) | (static_cast<uint32_t>(data[42]) << 16) | (static_cast<uint32_t>(data[43]) << 24);
    uint16_t bits = static_cast<uint16_t>(data[34]) | (static_cast<uint16_t>(data[35]) << 8);
    snd_pcm_format_t format;
    int size_divisor;
    
    // Find size devisor and format from the bits per samples in the header.
    switch(bits) {
        case 8:
            format = SND_PCM_FORMAT_S8;
            size_divisor = 1;
            break;
            
        case 16:
            format = SND_PCM_FORMAT_S16_LE;
            size_divisor = 2;
            break;
            
        case 24:
            format = SND_PCM_FORMAT_S24_LE;
            size_divisor = 3;
            break;
            
        case 32:
            format = SND_PCM_FORMAT_S32_LE;
            size_divisor = 4;
            break;
            
        default:
            throw std::runtime_error("Unsupported format!");
            return;
    }
    
    // Open the sound device
    snd_pcm_t* pcm;
    snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_t* hw_params;
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(pcm, hw_params);
    snd_pcm_hw_params_set_access(pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm, hw_params, format);
    snd_pcm_hw_params_set_channels(pcm, hw_params, 1);
    snd_pcm_hw_params_set_rate(pcm, hw_params, sample_rate, 0);
    snd_pcm_hw_params_set_periods(pcm, hw_params, 10, 0);
    snd_pcm_hw_params_set_period_time(pcm, hw_params, 100000, 0); // 0.1 seconds
    snd_pcm_hw_params(pcm, hw_params);
    // Discard the wave header by collecting the data at space 44.
    snd_pcm_writei(pcm, &data[44], data_section_size / size_divisor);
    // Wait for the buffer to play and close the device.
    snd_pcm_drain(pcm);
    snd_pcm_close(pcm);
}
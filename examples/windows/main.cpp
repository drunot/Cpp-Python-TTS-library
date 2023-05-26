/**
 * @file main.cpp
 * @author Anton Vigen Smolarz (anton@smolarz.dk)
 * @brief Simple example of how to use C++ Python TTS library on Windows
 * @version 1.0
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023
 */

#include"TTS.hpp"
#include<iostream>
#include<thread>
#include<Windows.h>

int main(int argc, char* argv[]) {
    std::wcout << L"Initializing TTS...\n";
    TTS::initialize();
    std::wcout << L"Available Models:\n";
    size_t model_num;
    auto names = TTS::list_models(model_num);
    
    for(size_t i = 0; i < model_num; ++i) {
        std::wcout << i << ": " << names[i] << L"\n";
    }
    
    TTS* tts = new TTS(names[6]);
    TTS::free_list(names, model_num);
    size_t data_size;
    auto data = tts->tts(data_size, L"This is a TTS test!");
    PlaySound(reinterpret_cast<LPCSTR>(data), GetModuleHandle(NULL), SND_MEMORY);
    TTS::free_buffer(data);
    // Delete tts before finalization.
    delete tts;
    TTS::finalize();
}
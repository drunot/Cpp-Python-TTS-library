#include<cwchar>
#include"core.hpp"
#include"PythonMaintainer.hpp"

#ifndef B1F75213_1FEA_432B_A5FC_84692FB2740D
#define B1F75213_1FEA_432B_A5FC_84692FB2740D

// Abstract away the PyObject type.
using PythonObject = void *;

class TTS_API TTS {
  public:
    TTS(const wchar_t* model_name = L"",
        const wchar_t* model_path = L"",
        const wchar_t* config_path = L"",
        const wchar_t* vocoder_path = L"",
        const wchar_t* vocoder_config_path = L"",
        bool progress_bar = true,
        bool gpu = false);
    ~TTS();
    TTS(const TTS& copy);
    TTS(TTS&& move);
    TTS& operator=(const TTS& copy);
    TTS& operator=(TTS&& move);
    static void initialize();
    static void finalize();
    
    wchar_t** models(size_t& size);
    bool is_multi_speaker();
    bool is_multi_lingual();
    wchar_t** speakers(size_t& size);
    wchar_t** languages(size_t& size);
    static wchar_t* get_models_file_path();
    static wchar_t** list_models(size_t& size);
    static void free_list(wchar_t** list, const size_t size);
    static void free_string(wchar_t* string);
    static void free_buffer(unsigned char* buffer);
    wchar_t** download_model_by_name(const wchar_t* model_name, size_t& size);
    void load_model_by_name(const wchar_t* model_name, bool gpu = false);
    void load_model_by_path(const wchar_t* model_path, const wchar_t* config_path, const wchar_t* vocoder_path = L"", const wchar_t* vocoder_config = L"", bool gpu = false);
    unsigned char* tts(size_t& size, const wchar_t* text, const wchar_t* speaker = L"", const wchar_t* language = L"", const wchar_t* speaker_wav = L"");
    void tts_to_file(const wchar_t* text, const wchar_t* speaker = L"", const wchar_t* language = L"", const wchar_t* speaker_wav = L"", const wchar_t* file_path = L"output.wav");
    wchar_t* ToString();
    unsigned int get_sample_rate();
    
  private:
    static bool _initialized;
    static PythonObject _pTTS;
    static PythonObject _pModels;
    static PythonObject _pIs_multi_speaker;
    static PythonObject _pIs_multi_lingual;
    static PythonObject _pSpeakers;
    static PythonObject _pLanguages;
    static PythonObject _pGet_models_file_path;
    static PythonObject _pList_models;
    static PythonObject _pDownload_model_by_name;
    static PythonObject _pLoad_model_by_name;
    static PythonObject _pLoad_model_by_path;
    static PythonObject _pTts;
    static PythonObject _pTts_to_file;
    // Numpy module:
    static PythonObject _pNumPy;
    static PythonObject _pFloat32;
    static PythonObject _pToBytes;
    
    PythonObject _self = nullptr;
};

#endif /* B1F75213_1FEA_432B_A5FC_84692FB2740D */

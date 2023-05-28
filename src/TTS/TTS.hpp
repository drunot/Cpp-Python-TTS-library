#include<cwchar>
#include"core.hpp"
#include"PythonMaintainer.hpp"

#ifndef B1F75213_1FEA_432B_A5FC_84692FB2740D
#define B1F75213_1FEA_432B_A5FC_84692FB2740D

// Abstract away the PyObject type.
using PythonObject = void *;

class TTS_API TTS {
  public:
    /**
     * @brief Construct a new TTS object
     *
     * @param model_name          The name of the model.
     * @param model_path          The path to the TTS model.
     * @param config_path         The path to the TTS model config.
     * @param vocoder_path        The path to the vocoder model.
     * @param vocoder_config_path The path to the vocoder model config.
     * @param progress_bar        Show a progress bar when downloading new models?
     * @param gpu                 Use GPU acceleration for the model?
     */
    TTS(const wchar_t* model_name = L"",
        const wchar_t* model_path = L"",
        const wchar_t* config_path = L"",
        const wchar_t* vocoder_path = L"",
        const wchar_t* vocoder_config_path = L"",
        bool progress_bar = true,
        bool gpu = false);
    /**
     * @brief Destroy the TTS object
     *
     */
    ~TTS();
    /**
     * @brief Copy a TTS object.
     *
     * @param copy The TTS object to copy.
     */
    TTS(const TTS& copy);
    /**
     * @brief Move a TTS object.
     *
     * @param move The TTS object to move.
     */
    TTS(TTS&& move);
    /**
     * @brief Copy assign a TTS object.
     *
     * @param copy  The TTS object to copy.
     * @return TTS& A copy of the TTS object.
     */
    TTS& operator=(const TTS& copy);
    /**
     * @brief Move assign a TTS object.
     *
     * @param move  The TTS object to move.
     * @return TTS& The object the TTS object was moved to.
     */
    TTS& operator=(TTS&& move);
    /**
     * @brief Initialize python and the TTS module. Should be called before any objects are created.
     *
     */
    static void initialize();
    /**
     * @brief Finalize python and the TTS module. Should be called after all TTS objects are deleted.
     *
     */
    static void finalize();
    /**
     * @brief Lists the names of all available models.
     *
     * @param size        [out] The size of the list.
     * @return wchar_t**  The list of the names of all available models. (Should be freed with free_list.)
     */
    wchar_t** models(size_t& size);
    /**
     * @brief Indicates if the current model has multiple speakers.
     *
     * @return true   The model has multiple speakers.
     * @return false  The model has a single speaker.
     */
    bool is_multi_speaker();
    /**
     * @brief Indicates if the current model has multiple languages.
     *
     * @return true   The model has multiple languages.
     * @return false  The model has a single language.
     */
    bool is_multi_lingual();
    /**
     * @brief Returns a list of all speakers available in the current model.
     *
     * @param size        [out] The size of the list.
     * @return wchar_t**  The list of speakers. (Should be freed with free_list)
     */
    wchar_t** speakers(size_t& size);
    /**
     * @brief Returns a list of all languages available in the current model.
     *
     * @param size        [out] The size of the list.
     * @return wchar_t**  The list of languages. (Should be freed with free_list)
     */
    wchar_t** languages(size_t& size);
    static wchar_t* get_models_file_path();
    /**
     * @brief Lists the names of all available models.
     *
     * @param size        [out] The size of the list.
     * @return wchar_t**  The list of the names of all available models. (Should be freed with free_list.)
     */
    static wchar_t** list_models(size_t& size);
    /**
     * @brief Frees a wchar_t** list allocated by this module.
     *
     * @param list The list to free.
     * @param size The size of the list.
     */
    static void free_list(wchar_t** list, const size_t size);
    /**
     * @brief Frees a wchar_t* string allocated by this module.
     *
     * @param string The string to free.
     */
    static void free_string(wchar_t* string);
    /**
     * @brief Frees a unsigned char * buffer generated by this module.
     *
     * @param buffer The buffer to free.
     */
    static void free_buffer(unsigned char* buffer);
    /**
     * @brief Downloads a model with a given name.
     *
     * @param model_name  The name of the model to download.
     * @param size        [out] Size of the returned list. (Always 4 on success and 0 if it failed.)
     * @return wchar_t**  A list containing: 0. Path to tts model 1. Path to tts model config 2. Path to vocoder model 3. Path to vocoder model config. (Should be freed with free_list)
     */
    wchar_t** download_model_by_name(const wchar_t* model_name, size_t& size);
    /**
     * @brief Loads a model with the given name.
     *
     * @param model_name Model name
     * @param gpu        Should the model be gpu accelerated?
     */
    void load_model_by_name(const wchar_t* model_name, bool gpu = false);
    /**
     * @brief Loads a model from the given paths.
     *
     * @param model_path      Path to TTS model.
     * @param config_path     Path to TTS model config.
     * @param vocoder_path    Path to Vocoder model.
     * @param vocoder_config  Path to Vocoder model config.
     * @param gpu             Should the model be gpu accelerated?
     */
    void load_model_by_path(const wchar_t* model_path, const wchar_t* config_path, const wchar_t* vocoder_path = L"", const wchar_t* vocoder_config = L"", bool gpu = false);
    /**
     * @brief Convert text to speech.
     *
     * @param size              [out] The size of the returned buffer.
     * @param text              The text to turn into speech.
     * @param speaker           If the model has multiple speakers: Here one is specified.
     * @param language          If the model has multiple languages: Here one is specified.
     * @param speaker_wav       Path to a wav file for voice cloning if the model supports it.
     * @return unsigned char*   The buffer containing the speech. The first 44 bytes will be a wave header. (Should be freed with free_buffer)
     */
    unsigned char* tts(size_t& size, const wchar_t* text, const wchar_t* speaker = L"", const wchar_t* language = L"", const wchar_t* speaker_wav = L"");
    /**
     * @brief Convert text to speech and save it to a file.
     *
     * @param text        The text to turn into speech.
     * @param speaker     If the model has multiple speakers: Here one is specified.
     * @param language    If the model has multiple languages: Here one is specified.
     * @param speaker_wav Path to a wav file for voice cloning if the model supports it.
     * @param file_path   File path to where the speech should be saved.
     */
    void tts_to_file(const wchar_t* text, const wchar_t* speaker = L"", const wchar_t* language = L"", const wchar_t* speaker_wav = L"", const wchar_t* file_path = L"output.wav");
    /**
     * @brief String representation of the TTS python object.
     *
     * @return wchar_t* The string representation. (Should be freed with free_string)
     */
    wchar_t* ToString();
    /**
     * @brief Get the sample rate of the synthesiser.
     *
     * @return unsigned int The sample rate.
     */
    unsigned int get_sample_rate();
    static constexpr int bits_pr_sample = 32; //!< The bits_pr_sample of the synthesiser.
    static constexpr int number_of_channels = 1; //!< The number of channels the synthesiser will create.
    
  private:
    static bool _initialized;
    static PythonObject _pTTS;
    static PythonObject _pIs_multi_speaker;
    static PythonObject _pIs_multi_lingual;
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

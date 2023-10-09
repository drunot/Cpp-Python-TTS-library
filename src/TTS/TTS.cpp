#include<Python.h>
#include<iostream>
#include<cwchar>
#include <numpy/arrayobject.h>
#include"TTS.hpp"
#include"WAV/WAVHeader.hpp"

PythonObject TTS::_pTTS;
PythonObject TTS::_pIs_multi_speaker;
PythonObject TTS::_pIs_multi_lingual;
PythonObject TTS::_pGet_models_file_path;
PythonObject TTS::_pList_models;
PythonObject TTS::_pDownload_model_by_name;
PythonObject TTS::_pLoad_model_by_name;
PythonObject TTS::_pLoad_model_by_path;
PythonObject TTS::_pTts;
PythonObject TTS::_pTts_to_file;
// Numpy module:
PythonObject TTS::_pNumPy;
PythonObject TTS::_pFloat32;
PythonObject TTS::_pToBytes;

bool TTS::_initialized = false;

TTS::TTS(const wchar_t* model_name,
         const wchar_t* model_path,
         const wchar_t* config_path,
         const wchar_t* vocoder_path,
         const wchar_t* vocoder_config_path,
         bool progress_bar,
         bool gpu) {
    initialize();
    PyObject* args = PyTuple_Pack(7,
                                  !std::wcscmp(model_name, L"") ? Py_None : PyUnicode_FromWideChar(model_name, std::wcslen(model_name)),
                                  !std::wcscmp(model_path, L"") ? Py_None : PyUnicode_FromWideChar(model_path, std::wcslen(model_path)),
                                  !std::wcscmp(config_path, L"") ? Py_None : PyUnicode_FromWideChar(config_path, std::wcslen(config_path)),
                                  !std::wcscmp(vocoder_path, L"") ? Py_None : PyUnicode_FromWideChar(vocoder_path, std::wcslen(vocoder_path)),
                                  !std::wcscmp(vocoder_config_path, L"") ? Py_None : PyUnicode_FromWideChar(vocoder_config_path, std::wcslen(vocoder_config_path)),
                                  progress_bar ? Py_True : Py_False,
                                  gpu ? Py_True : Py_False
                                 );
    _self = PyObject_CallObject(reinterpret_cast<PyObject*>(_pTTS), args);
    Py_DECREF(args);
    
    if(!_self) {
        throw std::runtime_error("TTS could was null after initialization.");
    }
}

TTS::~TTS() {
    if(_self) {
        Py_DECREF(_self);
    }
}

TTS::TTS(const TTS& copy) {
    _self = copy._self;
    Py_INCREF(_self);
}

TTS::TTS(TTS&& move) {
    _self = move._self;
    move._self = nullptr;
}

TTS& TTS::operator=(const TTS& copy) {
    if(_self) {
        Py_DECREF(_self);
    }
    
    _self = copy._self;
    Py_INCREF(_self);
    return *this;
}

TTS& TTS::operator=(TTS&& move) {
    // Decref self if it is not the same as move's self.
    if(_self && !PyObject_RichCompareBool(reinterpret_cast<PyObject*>(_self), reinterpret_cast<PyObject*>(move._self), Py_EQ)) {
        Py_DECREF(_self);
    }
    
    _self = move._self;
    move._self = nullptr;
    return *this;
}

bool TTS::initialize() {
    if(_initialized) {
        return true;
    }
    
    // Init python.
    PythonMaintainer* pPM = PythonMaintainer::getInstance();
    pPM->python_initialize();
    // Import TTS from TTS.api
    PyObject* pFromList = PyList_New(1);
    PyObject* pTTSModuleName = PyUnicode_FromString("TTS");
    PyList_SetItem(pFromList, 0, pTTSModuleName);
    PyObject* pTTSModule = PyImport_ImportModuleEx("TTS.api", nullptr, nullptr, pFromList);
    
    if(pTTSModule) {
        // "Select" TTS from TTS.api (like the python code. "from TTS.api import TTS")
        _pTTS = PyObject_GetAttrString(pTTSModule, "TTS");
        
        if(_pTTS) {
            PyObject* pIs_multi_speakerProp = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "is_multi_speaker");
            
            if(pIs_multi_speakerProp) {
                _pIs_multi_speaker = PyObject_GetAttrString(pIs_multi_speakerProp, "__get__");
            } else {
                std::wcout << L"is_multi_speaker was not found\n";
                throw std::runtime_error("is_multi_speaker was not found");
                return false;
            }
            
            Py_DECREF(pIs_multi_speakerProp);
            PyObject* pIs_multi_lingualProp = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "is_multi_lingual");
            
            if(pIs_multi_lingualProp) {
                _pIs_multi_lingual = PyObject_GetAttrString(pIs_multi_lingualProp, "__get__");
            } else {
                std::wcout << L"is_multi_lingual was not found\n";
                throw std::runtime_error("is_multi_lingual was not found");
                return false;
            }
            
            Py_DECREF(pIs_multi_lingualProp);
            _pGet_models_file_path = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "get_models_file_path");
            _pList_models = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "list_models");
            _pDownload_model_by_name = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "download_model_by_name");
            _pLoad_model_by_name = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "load_model_by_name");
            _pLoad_model_by_path = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "load_model_by_path");
            _pTts = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "tts");
            _pTts_to_file = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pTTS), "tts_to_file");
        } else {
            std::wcout << L"'TTS.api.TSS' module could not be loaded.\n";
            throw std::runtime_error("'TTS.api.TSS' module could not be loaded.");
            return false;
        }
    } else {
        std::wcout << L"'TTS.api' module could not be loaded.\n";
        throw std::runtime_error("'TTS.api' module could not be loaded.");
        return false;
    }
    
    _pNumPy = PyImport_ImportModule("numpy");
    
    if(_pNumPy) {
        _pFloat32 = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pNumPy), "float32");
        
        if(_pFloat32) {
            _pToBytes = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_pFloat32), "tobytes");
        } else {
            throw std::runtime_error("'float32' type could not be loaded.\n");
            return false;
        }
    } else {
        throw std::runtime_error("'numpy' module could not be loaded.");
        return false;
    }
    
    _initialized = true;
    return true;
}

void TTS::finalize() {
    Py_DECREF(_pIs_multi_speaker);
    Py_DECREF(_pIs_multi_lingual);
    Py_DECREF(_pGet_models_file_path);
    Py_DECREF(_pList_models);
    Py_DECREF(_pDownload_model_by_name);
    Py_DECREF(_pLoad_model_by_name);
    Py_DECREF(_pLoad_model_by_path);
    Py_DECREF(_pTts);
    Py_DECREF(_pTts_to_file);
    Py_DECREF(_pTTS);
    // Numpy module:
    Py_DECREF(_pFloat32);
    Py_DECREF(_pToBytes);
    Py_DECREF(_pNumPy);
    PythonMaintainer::getInstance()->python_finalize();
}

wchar_t** TTS::models(size_t& size) {
    // PyObject* args = PyTuple_Pack(1, _self);
    PyObject* pValue;
    pValue = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_self), "models");
    
    // std::wcout << L"PyList_Check(pValue): " << PyList_Check(pValue) << "\n";
    if(PyList_Check(pValue)) {
        size = PyList_Size(pValue);
        wchar_t** ret = new wchar_t* [size];
        
        for(size_t i = 0; i < size; ++i) {
            // temp is not a new object and should *not* be decrefed!
            PyObject* temp = PyList_GetItem(pValue, i);
            wchar_t* tempStr = PyUnicode_AsWideCharString(temp, nullptr);
            ret[i] = tempStr;
        }
        
        // Py_DECREF(args);
        Py_DECREF(pValue);
        return ret;
    }
    
    // Py_DECREF(args);
    Py_DECREF(pValue);
    size = 0;
    return nullptr;
}

bool TTS::is_multi_speaker() {
    PyObject* args = PyTuple_Pack(1, _self);
    PyObject* pValue = PyObject_CallObject(reinterpret_cast<PyObject*>(_pIs_multi_speaker), args);
    bool ret = false;
    
    if(pValue) {
        ret = PyObject_IsTrue(pValue);
    }
    
    Py_DECREF(args);
    Py_DECREF(pValue);
    return ret;
}

bool TTS::is_multi_lingual() {
    PyObject* args = PyTuple_Pack(1, _self);
    PyObject* pValue = PyObject_CallObject(reinterpret_cast<PyObject*>(_pIs_multi_lingual), args);
    bool ret = false;
    
    if(pValue) {
        ret = PyObject_IsTrue(pValue);
    }
    
    Py_DECREF(args);
    Py_DECREF(pValue);
    return ret;
}

wchar_t** TTS::speakers(size_t& size) {
    PyObject* args = PyTuple_Pack(1, _self);
    PyObject* pValue;
    pValue = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_self), "speakers");
    
    // std::wcout << L"PyList_Check(pValue): " << PyList_Check(pValue) << "\n";
    if(PyList_Check(pValue)) {
        size = PyList_Size(pValue);
        wchar_t** ret = new wchar_t* [size];
        
        for(size_t i = 0; i < size; ++i) {
            // temp is not a new object and should *not* be decrefed!
            PyObject* temp = PyList_GetItem(pValue, i);
            wchar_t* tempStr = PyUnicode_AsWideCharString(temp, nullptr);
            ret[i] = tempStr;
        }
        
        Py_DECREF(args);
        Py_DECREF(pValue);
        return ret;
    }
    
    Py_DECREF(args);
    Py_DECREF(pValue);
    size = 0;
    return nullptr;
}

wchar_t** TTS::languages(size_t& size) {
    PyObject* pValue;
    pValue = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_self), "languages");
    
    // std::wcout << L"PyList_Check(pValue): " << PyList_Check(pValue) << "\n";
    if(PyList_Check(pValue)) {
        size = PyList_Size(pValue);
        wchar_t** ret = new wchar_t* [size];
        
        for(size_t i = 0; i < size; ++i) {
            // temp is not a new object and should *not* be decrefed!
            PyObject* temp = PyList_GetItem(pValue, i);
            wchar_t* tempStr = PyUnicode_AsWideCharString(temp, nullptr);
            ret[i] = tempStr;
        }
        
        Py_DECREF(pValue);
        return ret;
    }
    
    Py_DECREF(pValue);
    size = 0;
    return nullptr;
}


unsigned int TTS::get_sample_rate() {
    PyObject* syth = PyObject_GetAttrString(reinterpret_cast<PyObject*>(_self), "synthesizer");
    PyObject* pValue = PyObject_GetAttrString(syth, "output_sample_rate");
    return static_cast<unsigned int>(PyLong_AsLong(pValue));
}

wchar_t* TTS::get_models_file_path() {
    initialize();
    PyObject* pValue = PyObject_CallObject(reinterpret_cast<PyObject*>(_pGet_models_file_path), NULL);
    
    if(PyList_Check(pValue)) {
        wchar_t* ret = PyUnicode_AsWideCharString(pValue, nullptr);
        Py_DECREF(pValue);
        return ret;
    }
    
    Py_DECREF(pValue);
    return L"";
}


wchar_t** TTS::list_models(size_t& size) {
    initialize();
    size = 0;
    PyObject* pValue = PyObject_CallObject(reinterpret_cast<PyObject*>(_pList_models), NULL);
    
    if(PyList_Check(pValue)) {
        size = PyList_Size(pValue);
        wchar_t** ret = new wchar_t* [size];
        
        for(size_t i = 0; i < size; ++i) {
            // temp is not a new object and should *not* be decrefed!
            PyObject* temp = PyList_GetItem(pValue, i);
            wchar_t* tempStr = PyUnicode_AsWideCharString(temp, nullptr);
            ret[i] = tempStr;
        }
        
        return ret;
    }
    
    Py_DECREF(pValue);
    size = 0;
    return nullptr;
}

void TTS::free_string(wchar_t* string) {
    PyMem_Free(string);
}

void TTS::free_list(wchar_t** list, const size_t size) {
    for(size_t i = 0; i < size; ++i) {
        PyMem_Free(list[i]);
    }
    
    delete [] list;
}
void TTS::free_buffer(unsigned char* buffer) {
    delete [] buffer;
}

wchar_t** TTS::download_model_by_name(const wchar_t* model_name, size_t& size) {
    PyObject* args = PyTuple_Pack(2, _self, PyUnicode_FromWideChar(model_name, std::wcslen(model_name)));
    PyObject* pValue = PyObject_CallObject(reinterpret_cast<PyObject*>(_pDownload_model_by_name), args);
    
    if(pValue) {
        size = 4;
        wchar_t** ret = new wchar_t* [4];
        PyObject* item0 = PyTuple_GetItem(pValue, 0);
        wchar_t* tempStr0 = PyUnicode_AsWideCharString(item0, nullptr);
        ret[0] = tempStr0;
        Py_DECREF(item0);
        PyObject* item1 = PyTuple_GetItem(pValue, 1);
        wchar_t* tempStr1 = PyUnicode_AsWideCharString(item1, nullptr);
        ret[1] = tempStr1;
        Py_DECREF(item1);
        PyObject* item2 = PyTuple_GetItem(pValue, 2);
        wchar_t* tempStr2 = PyUnicode_AsWideCharString(item2, nullptr);
        ret[2] = tempStr2;
        Py_DECREF(item2);
        PyObject* item3 = PyTuple_GetItem(pValue, 3);
        wchar_t* tempStr3 = PyUnicode_AsWideCharString(item3, nullptr);
        ret[3] = tempStr3;
        Py_DECREF(item3);
        Py_DECREF(args);
        Py_DECREF(pValue);
        return ret;
    }
    
    Py_DECREF(args);
    Py_DECREF(pValue);
    size = 0;
    return nullptr;
}
void TTS::load_model_by_name(const wchar_t* model_name, bool gpu) {
    PyObject* args = PyTuple_Pack(3,
                                  _self,
                                  PyUnicode_FromWideChar(model_name, std::wcslen(model_name)),
                                  gpu ? Py_True : Py_False);
    PyObject_CallObject(reinterpret_cast<PyObject*>(_pLoad_model_by_name), args);
    Py_DECREF(args);
}
void TTS::load_model_by_path(const wchar_t* model_path, const wchar_t* config_path, const wchar_t* vocoder_path, const wchar_t* vocoder_config, bool gpu) {
    PyObject* args = PyTuple_Pack(6,
                                  _self,
                                  PyUnicode_FromWideChar(model_path, std::wcslen(model_path)),
                                  PyUnicode_FromWideChar(config_path, std::wcslen(config_path)),
                                  !std::wcscmp(vocoder_path, L"") ? Py_None : PyUnicode_FromWideChar(vocoder_path, std::wcslen(vocoder_path)),
                                  !std::wcscmp(vocoder_config, L"") ? Py_None : PyUnicode_FromWideChar(vocoder_config, std::wcslen(vocoder_config)),
                                  gpu ? Py_True : Py_False);
    PyObject_CallObject(reinterpret_cast<PyObject*>(_pLoad_model_by_path), args);
    Py_DECREF(args);
}
unsigned char* TTS::tts(size_t& size, const wchar_t* text, const wchar_t* speaker, const wchar_t* language, const wchar_t* speaker_wav) {
    PyObject* args = PyTuple_Pack(5,
                                  reinterpret_cast<PyObject*>(_self),
                                  PyUnicode_FromWideChar(text, std::wcslen(text)),
                                  !std::wcscmp(speaker, L"") ? Py_None : PyUnicode_FromWideChar(speaker, std::wcslen(speaker)),
                                  !std::wcscmp(language, L"") ? Py_None : PyUnicode_FromWideChar(language, std::wcslen(language)),
                                  !std::wcscmp(speaker_wav, L"") ? Py_None : PyUnicode_FromWideChar(speaker_wav, std::wcslen(speaker_wav)));
    PyObject* pValue = PyObject_CallObject(reinterpret_cast<PyObject*>(_pTts), args);
    auto t = PyList_Check(pValue);
    
    if(PyList_Check(pValue)) {
        Py_ssize_t valueSize = PyList_Size(pValue);
        size = valueSize * sizeof(npy_float32) + WAV::WAVHeader::size();
        unsigned char* ret = new unsigned char[size];
        WAV::WAVHeader header(valueSize * sizeof(npy_float32), get_sample_rate(), WAV::type_format_e::PCM, bits_pr_sample, number_of_channels);
        header.insert_header(ret);
        
        for(Py_ssize_t i = 0; i < valueSize * sizeof(npy_float32); i += sizeof(npy_float32)) {
            // temp is not a new object and should *not* be decrefed!
            PyObject* temp = PyList_GetItem(pValue, i / sizeof(npy_float32));
            
            // For some reason if the value is zero the it is
            // stored as an int instead of a npy_float32.
            if(PyLong_Check(temp)) {
                for(Py_ssize_t k = 0; k < sizeof(npy_float32); ++k) {
                    ret[i + k + WAV::WAVHeader::size()] = 0;
                }
                
                continue;
            }
            
            PyObject* toBytes_args = PyTuple_Pack(1, temp);
            PyObject* tempData = PyObject_CallObject(reinterpret_cast<PyObject*>(_pToBytes), toBytes_args);
            auto data_int32 = std::int32_t(double(*reinterpret_cast<float*>(PyBytes_AsString(tempData))) * 2147483647);
            unsigned char* tempDataPtr = reinterpret_cast<unsigned char*>(&data_int32);
            
            for(Py_ssize_t k = 0; k < sizeof(npy_float32); ++k) {
                ret[i + k + WAV::WAVHeader::size()] = tempDataPtr[k];
            }
            
            Py_DECREF(tempData);
            Py_DECREF(toBytes_args);
        }
        
        Py_DECREF(args);
        Py_DECREF(pValue);
        return ret;
    }
    
    Py_DECREF(args);
    Py_DECREF(pValue);
    size = 0;
    return nullptr;
}
void TTS::tts_to_file(const wchar_t* text, const wchar_t* speaker, const wchar_t* language, const wchar_t* speaker_wav, const wchar_t* file_path) {
    PyObject* args = PyTuple_Pack(6,
                                  reinterpret_cast<PyObject*>(_self),
                                  PyUnicode_FromWideChar(text, std::wcslen(text)),
                                  !std::wcscmp(speaker, L"") ? Py_None : PyUnicode_FromWideChar(speaker, std::wcslen(speaker)),
                                  !std::wcscmp(language, L"") ? Py_None : PyUnicode_FromWideChar(language, std::wcslen(language)),
                                  !std::wcscmp(speaker_wav, L"") ? Py_None : PyUnicode_FromWideChar(speaker_wav, std::wcslen(speaker_wav)),
                                  !std::wcscmp(file_path, L"") ? Py_None : PyUnicode_FromWideChar(file_path, std::wcslen(file_path)));
    PyObject_CallObject(reinterpret_cast<PyObject*>(_pTts_to_file), args);
    Py_DECREF(args);
}
wchar_t* TTS::ToString() {
    PyObject* pObjRepr = PyObject_Repr(reinterpret_cast<PyObject*>(_self));
    wchar_t* ret = PyUnicode_AsWideCharString(pObjRepr, nullptr);
    Py_DECREF(pObjRepr);
    return ret;
}
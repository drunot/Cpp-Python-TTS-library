#ifndef AC5B83A5_3F4E_49DA_9991_1863BCB7A0DC
#define AC5B83A5_3F4E_49DA_9991_1863BCB7A0DC

#if defined(_MSC_VER)
    #ifdef DLL_EXPORTS
        #define TTS_API __declspec(dllexport)
    #else
        #define TTS_API __declspec(dllimport)
    #endif
    
#elif defined(__GNUC__)
    //  GCC
    #ifdef DLL_EXPORTS
        #define TTS_API __attribute__((visibility("default")))
    #else
        #define TTS_API
    #endif
#else
    #define TTS_API
    #pragma warning Unknown dynamic link import/export semantics.
#endif //PLT_WIN

#endif /* AC5B83A5_3F4E_49DA_9991_1863BCB7A0DC */

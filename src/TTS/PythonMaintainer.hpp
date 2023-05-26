#include"core.hpp"
#ifndef CFB80A7C_343D_49EA_9975_10B2C8EA41DA
#define CFB80A7C_343D_49EA_9975_10B2C8EA41DA

// This is kinda cursed but we do not want any
// of the c++ standard library in the header.
using mutex = void *;
using PythonThreadState = void *;

class TTS_API PythonMaintainer {
  public:
    static PythonMaintainer* getInstance();
    
    void python_initialize();
    
    void python_finalize();
    ~PythonMaintainer();
  private:
    bool _instantiated = false;
    static mutex _safe_create;
    static PythonMaintainer* _instance;
    PythonThreadState _state = nullptr;
    PythonMaintainer();
};

#endif /* CFB80A7C_343D_49EA_9975_10B2C8EA41DA */

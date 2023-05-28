#include"core.hpp"
#ifndef CFB80A7C_343D_49EA_9975_10B2C8EA41DA
#define CFB80A7C_343D_49EA_9975_10B2C8EA41DA

// This is kinda cursed but we do not want any
// of the c++ standard library in the header.
using mutex = void *;
using PythonThreadState = void *;

class TTS_API PythonMaintainer {
  public:
    /**
     * @brief Get the instance of the Python Maintainer.
     *
     * @return PythonMaintainer* The instance.
     */
    static PythonMaintainer* getInstance();
    
    /**
     * @brief Initialize python.
     *
     */
    void python_initialize();
    
    /**
     * @brief Finalize python.
     *
     */
    void python_finalize();
    /**
     * @brief Destroy the Python Maintainer.
     *
     */
    ~PythonMaintainer();
  private:
    bool _instantiated = false;
    static mutex _safe_create;
    static PythonMaintainer* _instance;
    PythonThreadState _state = nullptr;
    PythonMaintainer();
};

#endif /* CFB80A7C_343D_49EA_9975_10B2C8EA41DA */

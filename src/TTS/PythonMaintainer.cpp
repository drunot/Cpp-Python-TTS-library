#include<Python.h>
#include<mutex>
#include<iostream>
#include"PythonMaintainer.hpp"

PythonMaintainer* PythonMaintainer::getInstance() {
    // Create instance if it hasn't been requested before.
    if(_instance == nullptr) {
        // Create instance thread safe.
        auto lck = std::scoped_lock(*reinterpret_cast<std::mutex*>(_safe_create));
        
        // Check if the instance was created while beeing locked.
        if(_instance == nullptr) {
            _instance = new PythonMaintainer();
        }
    }
    
    return _instance;
}
void PythonMaintainer::python_initialize() {
    if(_instantiated) {
        return;
    }
    
    _instantiated = true;
    Py_SetProgramName(L"python");
    Py_Initialize();
}

void PythonMaintainer::python_finalize() {
    if(!_instantiated) {
        return;
    }
    
    _instantiated = false;
    Py_FinalizeEx();
}
PythonMaintainer::~PythonMaintainer() {
    python_finalize();
}
mutex PythonMaintainer::_safe_create = reinterpret_cast<mutex>(new std::mutex());
PythonMaintainer* PythonMaintainer::_instance = nullptr;
PythonMaintainer::PythonMaintainer() = default;

file(GLOB_RECURSE Sources *.cpp)
file(GLOB_RECURSE Headers *.hpp)
add_library(CppPyTTS SHARED ${Sources} ${Headers})
target_compile_features(CppPyTTS PRIVATE cxx_std_17)
target_compile_definitions(CppPyTTS PRIVATE DLL_EXPORTS)

find_package(Python3 COMPONENTS Development NumPy REQUIRED)
target_link_libraries(CppPyTTS PRIVATE Python3::Python Python3::NumPy)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CPP_PYTHON_TTS_LIB_PATH "lib/CppPyTTS")
set(CPP_PYTHON_TTS_BIN_PATH "bin/CppPyTTS")
set(CPP_PYTHON_TTS_INCLUDE_PATH "include/CppPyTTS")
set(CPP_PYTHON_TTS_CONFIG_PATH "${CPP_PYTHON_TTS_LIB_PATH}/cmake")
set(CPP_PYTHON_TTS_CONFIG_FILE "CppPyTTSConfig.cmake")

install(TARGETS CppPyTTS EXPORT CppPyTTS
    ARCHIVE DESTINATION "${CPP_PYTHON_TTS_LIB_PATH}" # static and import libs installed to lib
    LIBRARY DESTINATION "${CPP_PYTHON_TTS_LIB_PATH}" # .so installed to lib
    RUNTIME DESTINATION "${CPP_PYTHON_TTS_BIN_PATH}" # .dlls installed to bin 
    )

# install directory containing public headers to include/my_lib
install(DIRECTORY "." DESTINATION ${CPP_PYTHON_TTS_INCLUDE_PATH}
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    FILES_MATCHING PATTERN "*.hpp")

# create cmake config to lib/cmake/my_libConfig.cmake
install(EXPORT CppPyTTS
    DESTINATION ${CPP_PYTHON_TTS_CONFIG_PATH}
    FILE ${CPP_PYTHON_TTS_CONFIG_FILE})

configure_file(CppPyTTSConfig.cmake.in CppPyTTSConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CppPyTTSConfig.cmake" DESTINATION "${CPP_PYTHON_TTS_LIB_PATH}")

message("${RUNTIME_OUTPUT_DIRECTORY}")
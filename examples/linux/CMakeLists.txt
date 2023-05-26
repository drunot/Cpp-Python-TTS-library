cmake_minimum_required(VERSION 3.0.0)
project(TTSLinuxExample VERSION 0.1.0)
add_executable(${PROJECT_NAME} "main.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
add_subdirectory(../../src/ cppPythonTTS)
find_package(ALSA  REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC optimized CppPyTTS ALSA::ALSA)
include_directories(../../src)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy "$<TARGET_FILE_DIR:CppPyTTS>/libCppPyTTS.so" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
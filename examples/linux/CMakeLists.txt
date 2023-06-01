cmake_minimum_required(VERSION 3.0.0)
project(TTSLinuxExample VERSION 0.1.0)
add_executable(${PROJECT_NAME} "main.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
#add_subdirectory(../../src/ cppPythonTTS)
find_package(ALSA  REQUIRED)
find_package(CppPyTTS  REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC CppPyTTS ALSA::ALSA)

add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:CppPyTTS> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

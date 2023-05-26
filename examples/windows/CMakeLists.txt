cmake_minimum_required(VERSION 3.0.0)
project(TTSWinExample VERSION 0.1.0)
add_executable(${PROJECT_NAME} "main.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
add_subdirectory(../../src/ cppPythonTTS)
target_link_libraries(${PROJECT_NAME} PUBLIC optimized CppPyTTS Winmm.lib)
include_directories(C:/Users/anton/Documents/Repos/Cpp-Python-TTS-library/src)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy "$<TARGET_FILE_DIR:CppPyTTS>/CppPyTTS.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy "$<TARGET_FILE_DIR:CppPyTTS>/zlib1.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
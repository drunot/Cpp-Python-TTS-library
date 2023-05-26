
file(GLOB_RECURSE Sources *.cpp)
file(GLOB_RECURSE Headers *.hpp)
add_library(CppPyTTS SHARED ${Sources} ${Headers})
target_compile_features(CppPyTTS PRIVATE cxx_std_17)
target_compile_definitions(CppPyTTS PUBLIC DLL_EXPORTS)

find_package(Python3 COMPONENTS Development NumPy REQUIRED)
target_link_libraries(CppPyTTS PUBLIC Python3::Python Python3::NumPy)
message(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR})
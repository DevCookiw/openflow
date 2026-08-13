cmake_minimum_required(VERSION 3.17)
project(OpenFlow VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern/llama.cpp)
add_subdirectory(extern/whisper.cpp)
add_subdirectory(extern/libuiohook)

include_directories(
    include
    extern
    extern/whisper.cpp
    extern/llama.cpp/common
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    whisper
    llama
    uiohook
)

if (APPLE)
  find_library(CORE_AUDIO CoreAudio)
  find_library(AUDIO_TOOLBOX AudioToolbox)
  find_library(CORE_GRAPHICS CoreGraphics)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${CORE_AUDIO} ${AUDIO_TOOLBOX} ${CORE_GRAPHICS})
endif()

cmake_minimum_required(VERSION 3.17)
project(OpenFlow VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# CRITICAL FOR ZED: This generates compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Add whisper.cpp (Speech-to-Text)
add_subdirectory(extern/whisper.cpp)

# 2. Add llama.cpp (LLM cleanup)
add_subdirectory(extern/llama.cpp)

# 3. Add libuiohook (Global Keystrokes)
add_subdirectory(extern/libuiohook)

# Include directories so you can use #include "whisper.h" cleanly
include_directories(
    include
    extern
    extern/whisper.cpp
    extern/llama.cpp/common
)

# Your main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link everything together
target_link_libraries(${PROJECT_NAME} PRIVATE
    whisper
    llama
    uiohook
)

# macOS specific frameworks for audio and keystrokes
if (APPLE)
    find_library(CORE_AUDIO CoreAudio)
    find_library(AUDIO_TOOLBOX AudioToolbox)
    find_library(CORE_GRAPHICS CoreGraphics)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CORE_AUDIO} ${AUDIO_TOOLBOX} ${CORE_GRAPHICS})
endif()

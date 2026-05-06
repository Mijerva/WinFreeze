cmake_minimum_required(VERSION 3.20)
project(WinFreeze VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "WinFreeze only supports Windows")
endif()

# Dependencies
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

# Shared compile settings applied to all WinFreeze targets
set(WINFREEZE_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:MSVC>:/sdl>
    $<$<CXX_COMPILER_ID:MSVC>:/GS>
    $<$<CXX_COMPILER_ID:MSVC>:/guard:cf>
    $<$<CXX_COMPILER_ID:MSVC>:/external:anglebrackets>
    $<$<CXX_COMPILER_ID:MSVC>:/external:W0>
)

set(WINFREEZE_LINK_OPTIONS
    $<$<CXX_COMPILER_ID:MSVC>:/GUARD:CF>
)

set(WINFREEZE_COMPILE_DEFINITIONS
    UNICODE
    _UNICODE
    WINVER=0x0A00
    _WIN32_WINNT=0x0A00
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    WINFREEZE_SAFETY_ASSERTIONS
)

# Core static library — linked by both WinFreeze and WinFreezeTests
add_library(WinFreezeCore STATIC
    src/core/ProcessManager.cpp
    src/core/ProcessGuard.cpp
    src/core/ProfileManager.cpp
    src/core/VmDetector.cpp
    src/ui/MainWindow.cpp
    src/ui/ProcessList.cpp
    src/ui/SettingsDialog.cpp
)

target_include_directories(WinFreezeCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(WinFreezeCore PUBLIC ${WINFREEZE_COMPILE_DEFINITIONS})
target_compile_options(WinFreezeCore PRIVATE ${WINFREEZE_COMPILE_OPTIONS})
target_link_options(WinFreezeCore INTERFACE ${WINFREEZE_LINK_OPTIONS})

target_link_libraries(WinFreezeCore PUBLIC
    nlohmann_json::nlohmann_json
    psapi
)

# Main executable
add_executable(WinFreeze WIN32
    src/main.cpp
)

target_compile_options(WinFreeze PRIVATE ${WINFREEZE_COMPILE_OPTIONS})
target_link_libraries(WinFreeze PRIVATE WinFreezeCore)

# Tests
enable_testing()
add_subdirectory(tests)

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

add_executable(WinFreeze WIN32
    src/main.cpp
    src/core/ProcessManager.cpp
    src/core/ProcessGuard.cpp
    src/core/ProfileManager.cpp
    src/core/VmDetector.cpp
    src/ui/MainWindow.cpp
    src/ui/ProcessList.cpp
    src/ui/SettingsDialog.cpp
)

target_include_directories(WinFreeze PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(WinFreeze PRIVATE
    UNICODE
    _UNICODE
    WINVER=0x0A00
    _WIN32_WINNT=0x0A00
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

target_compile_options(WinFreeze PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(WinFreeze PRIVATE
    nlohmann_json::nlohmann_json
    psapi
)

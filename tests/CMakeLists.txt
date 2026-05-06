add_executable(WinFreezeTests
    test_main.cpp
)

target_compile_options(WinFreezeTests PRIVATE ${WINFREEZE_COMPILE_OPTIONS})

target_link_libraries(WinFreezeTests PRIVATE
    WinFreezeCore
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(WinFreezeTests)

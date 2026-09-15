cmake_minimum_required(VERSION 3.25)

project(PsycleAdapter VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Handle fallback flags safely for the Windows runner (clang-cl)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-unknown-pragmas)
    if(MSVC OR CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
        add_compile_options(-clang:-Wno-unknown-pragmas)
    endif()
endif()

# 2. Point to your iPlug2OOS framework directory
add_subdirectory(iPlug2)

# 3. Define your processing and UI source trees (including config.h)
set(SOURCE_FILES
    config.h
    PsycleAdapter.h
    PsycleAdapter.cpp
    PsycleAdapterUI.cpp
)

# 4. Initialize the iPlug2 target generator macro (CLEANED UP FIXED VERSION)
iplug_add_plugin(PsycleAdapter
    SOURCES ${SOURCE_FILES}
    FORMATS VST3 Standalone
)

# 5. Link the hardware accelerated vector graphics library layer
target_link_libraries(PsycleAdapter PRIVATE iPlug2::IGraphics)

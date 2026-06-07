cmake_minimum_required(VERSION 3.14)
project(wertheim_cipher VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Find OpenSSL ──────────────────────────────────────────────
find_package(OpenSSL REQUIRED)

# ── Library target ─────────────────────────────────────────────
add_library(wertheim
    src/wertheim.cpp
    src/tarr_data.cpp
)

target_include_directories(wertheim PUBLIC include)
target_link_libraries(wertheim PRIVATE OpenSSL::Crypto)

# ── Executable (test/demo) ─────────────────────────────────────
add_executable(wertheim_demo src/main.cpp)
target_link_libraries(wertheim_demo PRIVATE wertheim)
target_include_directories(wertheim_demo PRIVATE include)

# ── Install ────────────────────────────────────────────────────
install(TARGETS wertheim wertheim_demo DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

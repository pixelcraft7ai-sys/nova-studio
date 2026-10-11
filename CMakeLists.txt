cmake_minimum_required(VERSION 3.21)

project(NovaStudio
  VERSION 0.0.1
  DESCRIPTION "NOVA Studio - Arabic-first desktop creative studio"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(NOVA_BUILD_TESTS "Build the test suite" ON)
option(NOVA_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(NOVA_SANITIZE "Enable AddressSanitizer + UBSan (GCC/Clang only)" OFF)
option(NOVA_ENABLE_CLANG_TIDY "Run clang-tidy during the build" OFF)

include(cmake/NovaWarnings.cmake)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Widgets Test)

if(NOVA_ENABLE_CLANG_TIDY)
  find_program(NOVA_CLANG_TIDY NAMES clang-tidy REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY "${NOVA_CLANG_TIDY};--extra-arg=-Wno-gnu-zero-variadic-macro-arguments")
endif()

# Layering (enforced by tests/scripts/check_layering.cmake):
#   core  <-  document  <-  render, formats       (no Qt, no GUI)
#   core  <-  platform (Qt Core only)
#   app   -> everything (the only module allowed to use QtWidgets)
add_subdirectory(src/core)
add_subdirectory(src/document)
add_subdirectory(src/render)
add_subdirectory(src/formats)
add_subdirectory(src/platform)
add_subdirectory(src/app)

if(NOVA_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

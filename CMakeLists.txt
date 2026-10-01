cmake_minimum_required(VERSION 3.28...4.3)

project(concurrent-cpp-containers
        VERSION 0.1.0
        DESCRIPTION "Lock-free and wait-free concurrent containers for C++"
        LANGUAGES CXX
)

# Standard for the project's own targets (tests, benchmarks). Guarded so that
# -DCMAKE_CXX_STANDARD=23/26 can be passed to check forward compatibility.
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CCC_BUILD_TESTS "Build the tests" ${PROJECT_IS_TOP_LEVEL})
option(CCC_BUILD_BENCHMARKS "Build the benchmarks" OFF)

add_library(concurrent_cpp_containers INTERFACE)
add_library(ccc::ccc ALIAS concurrent_cpp_containers)

target_include_directories(concurrent_cpp_containers INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# Minimum standard for consumers; newer standards stay allowed.
target_compile_features(concurrent_cpp_containers INTERFACE cxx_std_20)

if(CCC_BUILD_TESTS OR CCC_BUILD_BENCHMARKS)
  list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
  include(CompilerWarnings)
  # Before any dependency is fetched, so that they get instrumented too.
  include(Sanitizers)
endif()

if(CCC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CCC_BUILD_BENCHMARKS)
  add_subdirectory(tests/benchmarks)
endif()

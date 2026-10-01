include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
# Warnings in a dependency must not break our build on newer compilers.
set(BENCHMARK_ENABLE_WERROR OFF)

FetchContent_Declare(googlebenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.tar.gz
  URL_HASH SHA256=9631341c82bac4a288bef951f8b26b41f69021794184ece969f8473977eaa340
  SYSTEM
  EXCLUDE_FROM_ALL
)
# Boost.Lockfree is the reference lock-free implementation to compare against.
# An installed Boost is used if there is one; otherwise the release is
# downloaded and only Lockfree and its dependencies are configured.
set(BOOST_INCLUDE_LIBRARIES lockfree)
FetchContent_Declare(Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.92.0/boost-1.92.0-cmake.tar.xz
  URL_HASH SHA256=9bed76128d4e46755dbe818487788c6fceb6f72b378f4daa49b7e1e600d9088d
  SYSTEM
  EXCLUDE_FROM_ALL
  FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(googlebenchmark Boost)

# A downloaded Boost has a target per library; an installed one has only
# Boost::headers, which is enough for the header-only Lockfree.
if(TARGET Boost::lockfree)
  set(ccc_boost_lockfree Boost::lockfree)
else()
  set(ccc_boost_lockfree Boost::headers)
endif()

add_executable(spsc_queue_benchmark wait_free/spsc_queue_benchmark.cpp)
target_include_directories(spsc_queue_benchmark PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(spsc_queue_benchmark PRIVATE
  ccc::ccc
  benchmark::benchmark_main
  ${ccc_boost_lockfree}
)
ccc_enable_warnings(spsc_queue_benchmark)

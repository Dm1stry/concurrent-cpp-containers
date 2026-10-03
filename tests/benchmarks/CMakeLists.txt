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
# Header-only queues from the libraries in references.md. Only their headers are
# used: SOURCE_SUBDIR names a directory without a CMakeLists.txt, so their own
# build scripts never run.
FetchContent_Declare(rigtorp_spscqueue
  URL https://github.com/rigtorp/SPSCQueue/archive/1053918dbd251fbff69b24ef27fa5d51c29ec2af.tar.gz
  URL_HASH SHA256=cb3412d9455235defed8ab04133cc8e0eae4555de27b91f74d664c23a2a46180
  SOURCE_SUBDIR headers-only
)
FetchContent_Declare(moodycamel_readerwriterqueue
  URL https://github.com/cameron314/readerwriterqueue/archive/refs/tags/v1.0.7.tar.gz
  URL_HASH SHA256=532224ed052bcd5f4c6be0ed9bb2b8c88dfe7e26e3eb4dd9335303b059df6691
  SOURCE_SUBDIR headers-only
)
FetchContent_Declare(atomic_queue
  URL https://github.com/max0x7ba/atomic_queue/archive/refs/tags/v1.9.4.tar.gz
  URL_HASH SHA256=fecfa9ca12fc203e40fd967c2bfa8033f77d0bde0e3b21c8ca5f5f9eeeabe022
  SOURCE_SUBDIR headers-only
)
FetchContent_Declare(dnedic_lockfree
  URL https://github.com/DNedic/lockfree/archive/refs/tags/3.0.1.tar.gz
  URL_HASH SHA256=b75641bb321b1a446c84819c4f9fc6c5b876e3d41de7c627e932261ae79eef27
  SOURCE_SUBDIR headers-only
)
set(ccc_third_party_queues rigtorp_spscqueue moodycamel_readerwriterqueue atomic_queue dnedic_lockfree)

# xenium and libcds contain architecture-specific code, so they are only built
# where that has been checked: x86-64 with GCC or Clang.
if(NOT MSVC AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
  set(ccc_benchmark_xenium_and_libcds ON)
  FetchContent_Declare(xenium
    URL https://github.com/mpoeter/xenium/archive/1c449ae953ce2a440b0d16c5ed1181d2754860ab.tar.gz
    URL_HASH SHA256=3057321e33c30be3a2ff5d0691cc70cdda8da046d73e5b822ccee4c9d12c83d4
    SOURCE_SUBDIR headers-only
  )
  FetchContent_Declare(libcds
    URL https://github.com/khizmax/libcds/archive/9985d2a87feaa3e92532e28f4ab762a82855a49c.tar.gz
    URL_HASH SHA256=17de6789e873e2660b07d10c2fe0fd333d33c543593d2da140e83e1f3bae6c3a
    SOURCE_SUBDIR headers-only
  )
  list(APPEND ccc_third_party_queues xenium libcds)
endif()

FetchContent_MakeAvailable(googlebenchmark Boost ${ccc_third_party_queues})

# A downloaded Boost has a target per library; an installed one has only
# Boost::headers, which is enough for the header-only Lockfree.
if(TARGET Boost::lockfree)
  set(ccc_boost_lockfree Boost::lockfree)
else()
  set(ccc_boost_lockfree Boost::headers)
endif()

add_executable(spsc_queue_benchmark wait_free/spsc_queue_benchmark.cpp)
target_include_directories(spsc_queue_benchmark PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(spsc_queue_benchmark SYSTEM PRIVATE
  "${rigtorp_spscqueue_SOURCE_DIR}/include"
  "${moodycamel_readerwriterqueue_SOURCE_DIR}"
  "${atomic_queue_SOURCE_DIR}/include"
  "${dnedic_lockfree_SOURCE_DIR}"
)
# DNedic/lockfree takes these from its own CMake build; these are its defaults.
target_compile_definitions(spsc_queue_benchmark PRIVATE LOCKFREE_CACHE_COHERENT=1 LOCKFREE_CACHELINE_LENGTH=64)
if(ccc_benchmark_xenium_and_libcds)
  target_include_directories(spsc_queue_benchmark SYSTEM PRIVATE
    "${xenium_SOURCE_DIR}"
    "${libcds_SOURCE_DIR}"
  )
  target_compile_definitions(spsc_queue_benchmark PRIVATE CCC_BENCHMARK_XENIUM=1 CCC_BENCHMARK_LIBCDS=1)
endif()
target_link_libraries(spsc_queue_benchmark PRIVATE
  ccc::ccc
  benchmark::benchmark_main
  ${ccc_boost_lockfree}
)
ccc_enable_warnings(spsc_queue_benchmark)

include(FetchContent)

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
# Use the same MSVC runtime library (/MD) as the tests.
set(gtest_force_shared_crt ON)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
  URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
  SYSTEM
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(googletest)

add_executable(ccc_tests
  wait_free/spsc_queue_test.cpp
)
target_link_libraries(ccc_tests PRIVATE ccc::ccc GTest::gtest_main)
ccc_enable_warnings(ccc_tests)

include(GoogleTest)
# PRE_TEST: list the tests when ctest runs rather than at build time, so the
# build never has to start a sanitizer-instrumented binary.
gtest_discover_tests(ccc_tests DISCOVERY_MODE PRE_TEST)

# Compile-only checks, built as part of the normal build:
#   * every public header compiles on its own (includes what it uses);
#   * the library builds with exceptions disabled.
file(GLOB_RECURSE ccc_public_headers CONFIGURE_DEPENDS
  RELATIVE "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
)
set(ccc_header_check_sources no_exceptions_check.cpp)
foreach(header IN LISTS ccc_public_headers)
  string(MAKE_C_IDENTIFIER "${header}" header_id)
  set(source "${CMAKE_CURRENT_BINARY_DIR}/header_check/${header_id}.cc")
  file(CONFIGURE OUTPUT "${source}" CONTENT "#include \"${header}\"\n")
  list(APPEND ccc_header_check_sources "${source}")
endforeach()

add_library(ccc_compile_checks OBJECT ${ccc_header_check_sources})
target_link_libraries(ccc_compile_checks PRIVATE ccc::ccc)
ccc_enable_warnings(ccc_compile_checks)
if(NOT MSVC)
  target_compile_options(ccc_compile_checks PRIVATE -fno-exceptions)
endif()

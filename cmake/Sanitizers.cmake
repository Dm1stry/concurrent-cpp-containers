# Builds everything below the including directory, fetched dependencies
# included, with the sanitizers listed in CCC_SANITIZERS.

set(CCC_SANITIZERS "" CACHE STRING
    "Sanitizers for tests and benchmarks: e.g. \"address;undefined\" or \"thread\"")

if(CCC_SANITIZERS)
  if(MSVC)
    message(FATAL_ERROR "CCC_SANITIZERS supports only GCC and Clang")
  endif()
  if("thread" IN_LIST CCC_SANITIZERS AND "address" IN_LIST CCC_SANITIZERS)
    message(FATAL_ERROR "ThreadSanitizer cannot be combined with AddressSanitizer")
  endif()

  list(JOIN CCC_SANITIZERS "," ccc_sanitizer_list)
  add_compile_options(
    -fsanitize=${ccc_sanitizer_list}
    -fno-omit-frame-pointer
    # Make every report fail the test instead of just printing it.
    -fno-sanitize-recover=all
  )
  add_link_options(-fsanitize=${ccc_sanitizer_list})
endif()

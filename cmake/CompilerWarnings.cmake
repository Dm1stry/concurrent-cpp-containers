# Warning flags for the project's own targets (tests, benchmarks, compile
# checks). Consumers of ccc::ccc never get these flags.

option(CCC_WARNINGS_AS_ERRORS
       "Treat warnings in tests and benchmarks as errors" OFF)

function(ccc_enable_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE
      /W4
      /permissive-
      # "Structure was padded due to alignment specifier" is exactly what
      # alignas is used for here.
      /wd4324
    )
  else()
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
      -Wshadow
      -Wold-style-cast
      -Wnon-virtual-dtor
      -Woverloaded-virtual
    )
  endif()
  if(CCC_WARNINGS_AS_ERRORS)
    set_target_properties(${target} PROPERTIES COMPILE_WARNING_AS_ERROR ON)
  endif()
endfunction()

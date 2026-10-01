# concurrent-cpp-containers

[![CI](https://github.com/Dm1stry/concurrent-cpp-containers/actions/workflows/ci.yml/badge.svg)](https://github.com/Dm1stry/concurrent-cpp-containers/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
![C++20](https://img.shields.io/badge/C%2B%2B-20%20%7C%2023%20%7C%2026-blue.svg)

Header-only lock-free and wait-free concurrent containers for C++20.

Each container documents its progress guarantee and thread-safety contract,
is tested under ThreadSanitizer on both x86-64 and ARM64, and works with
exceptions disabled.

## Containers

| Container | Header | Progress guarantee | Status |
|---|---|---|---|
| `ccc::wait_free::spsc_queue`: bounded single-producer single-consumer ring buffer | `ccc/wait_free/spsc_queue.hpp` | wait-free | in progress |
| Hazard pointers (API of C++26 `std::hazard_pointer`, P2530) | | | planned |
| Treiber stack | | lock-free | planned |
| Michael–Scott queue | | lock-free | planned |
| Harris–Michael ordered list | | lock-free | planned |
| Hash map (Michael; split-ordered lists for resizing) | | lock-free | planned |
| Skip list | | lock-free | planned |

## Example

```cpp
#include <optional>
#include <thread>

#include <ccc/wait_free/spsc_queue.hpp>

int main() {
  ccc::wait_free::spsc_queue<int> queue(1024);

  std::jthread producer([&queue] {
    for (int i = 0; i < 1000; ++i) {
      while (!queue.push(i)) {
        // Full: retry, back off or drop the value.
      }
    }
  });

  for (int received = 0; received < 1000;) {
    if (std::optional<int> value = queue.pop()) {
      ++received;
    }
  }
}
```

## Using the library

The library is header-only and depends only on the standard library. With
CMake:

```cmake
include(FetchContent)
FetchContent_Declare(ccc
  GIT_REPOSITORY https://github.com/Dm1stry/concurrent-cpp-containers.git
  GIT_TAG main
)
FetchContent_MakeAvailable(ccc)

target_link_libraries(my_app PRIVATE ccc::ccc)
```

`add_subdirectory()` works the same way. Tests and benchmarks are only built
when the project is the top-level one.

Requires a C++20 compiler and CMake 3.28 or newer. CI covers GCC 15 and 16,
Clang 22, AppleClang and MSVC.

## Design rules

- **Nothrow moves.** Element types must be nothrow move constructible, which
  is checked with `static_assert`. Wrap other types in `std::unique_ptr`.
- **No exceptions of its own.** The library never throws or catches, and it
  compiles with `-fno-exceptions`. An exception thrown by the constructor of an
  element propagates and leaves the container unchanged: an element is fully
  constructed before other threads can see it.
- **Documented contracts.** Every class states which threads may call which
  member functions and the progress guarantee of each operation. Every memory
  order in the implementation comes with a comment explaining why it is
  sufficient.
- **Style.** [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html),
  except that the public API follows standard-library naming (`spsc_queue`,
  `push`, `pop`) so the containers read like the standard ones. Namespaces
  follow the progress guarantee: `ccc::wait_free`, `ccc::lock_free`.

## Building and testing

The CMake presets cover the common configurations:

| Preset | Configuration |
|---|---|
| `debug` | Tests, no optimization |
| `release` | Optimized tests and benchmarks |
| `asan` | AddressSanitizer + UndefinedBehaviorSanitizer (GCC/Clang) |
| `tsan` | ThreadSanitizer (GCC/Clang) |

```sh
cmake --workflow --preset debug   # configure, build and run the tests
cmake --workflow --preset tsan
```

x86-64 hides most memory-ordering mistakes: its hardware keeps loads and
stores in program order except for a store followed by a load. A test that
passes on x86-64 therefore proves little about acquire/release correctness.
ThreadSanitizer checks the orders the code actually requests, and CI also runs
the tests on ARM64, whose memory model is weaker. To hammer a change locally:

```sh
ctest --preset tsan -R Concurrency --repeat until-fail:100
```

## Continuous integration

Every push and pull request runs:

- GCC 15, GCC 16 and Clang 22 on x86-64 Linux, in C++20, C++23 and C++26;
- AddressSanitizer + UndefinedBehaviorSanitizer and ThreadSanitizer;
- ARM64 Linux (optimized build, ThreadSanitizer) and ARM64 macOS (AppleClang,
  ThreadSanitizer);
- MSVC on x64 and ARM64 Windows;
- the minimum supported CMake version (3.28);
- compile-only checks that every public header is self-contained and that the
  library builds with `-fno-exceptions`;
- `clang-format`.

Warnings are errors in CI.

## Benchmarks

```sh
cmake --workflow --preset release
taskset -c 2,4 ./build/release/tests/benchmarks/spsc_queue_benchmark
```

The benchmarks compare each container with a mutex-protected equivalent and
with its [Boost.Lockfree](https://www.boost.org/libs/lockfree) counterpart
where there is one. An installed Boost is used if CMake finds it; otherwise
the release is downloaded when the project is configured. Results will be
published here once the implementations are complete.

## Related work

Other concurrent-container libraries are listed in
[references.md](references.md).

## License

[MIT](LICENSE)

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

The benchmarks compare each container with a mutex-protected equivalent, its
[Boost.Lockfree](https://www.boost.org/libs/lockfree) counterpart and the
matching containers of the libraries in [references.md](references.md). An
installed Boost is used if CMake finds it; otherwise the release is downloaded
when the project is configured, like the other libraries. xenium and libcds
are only built on x86-64 with GCC or Clang.

```sh
tools/update_benchmarks.py
```

builds the `release` preset, runs every benchmark several times with its
threads pinned to two different physical cores (Linux) and rewrites the tables
below. `--print` shows the tables without touching this file, `--help` lists
the other options. To run a benchmark by hand:

```sh
taskset -c 2,4 ./build/release/tests/benchmarks/spsc_queue_benchmark
```

<!-- benchmarks:begin -->
<!-- Generated by tools/update_benchmarks.py; do not edit by hand. -->
_Measured on 2026-10-03: AMD Ryzen 7 8845H w/ Radeon 780M Graphics, Linux, GCC 16.2.1, threads pinned to CPUs 2 and 4, median of 5 runs ± coefficient of variation (standard deviation / mean). Absolute numbers depend on the machine; compare the queues with each other._

**Throughput** (items per second by capacity, higher is better)

| Queue | 64 | 256 | 4096 | 65536 |
|---|---:|---:|---:|---:|
| `ccc::wait_free::spsc_queue` | 291 M ±0.8% | 171 M ±29% | 178 M ±0.2% | 175 M ±0.1% |
| `rigtorp::SPSCQueue` | 268 M ±0.0% | **284 M** ±0.3% | 179 M ±0.2% | **181 M** ±0.9% |
| `moodycamel::ReaderWriterQueue` | 164 M ±3% | 142 M ±0.9% | 141 M ±0.8% | 145 M ±2% |
| `moodycamel::BlockingReaderWriterCircularBuffer` | 20.9 M ±0.6% | 21.6 M ±0.5% | 18.5 M ±0.9% | 18.5 M ±1% |
| `atomic_queue::AtomicQueueB2 (SPSC)` | 79.4 M ±0.8% | 79.4 M ±1% | 80.5 M ±0.7% | 81.9 M ±1% |
| `boost::lockfree::spsc_queue` | 185 M ±0.1% | 148 M ±2% | 140 M ±0.1% | 135 M ±0.1% |
| `lockfree::spsc::Queue` | 156 M ±0.1% | 145 M ±0.2% | 140 M ±0.1% | 130 M ±0.1% |
| `cds::container::WeakRingBuffer` | **301 M** ±0.1% | 221 M ±1% | **182 M** ±0.2% | 179 M ±0.1% |
| `xenium::vyukov_bounded_queue (MPMC)` | 27.7 M ±0.5% | 28.3 M ±0.4% | 18.1 M ±1% | 19 M ±2% |
| `xenium::nikolaev_bounded_queue (MPMC)` | 12 M ±0.8% | 11.4 M ±0.5% | 6.21 M ±1% | 5.8 M ±1% |
| `std::deque + std::mutex` | 2.9 M ±7% | 4.17 M ±10% | 4.86 M ±5% | 5.47 M ±14% |

**Round trip** (time per iteration, lower is better)

| Queue | Time per iteration |
|---|---:|
| `ccc::wait_free::spsc_queue` | **139 ns** ±0.1% |
| `rigtorp::SPSCQueue` | 143 ns ±0.1% |
| `moodycamel::ReaderWriterQueue` | 142 ns ±2% |
| `moodycamel::BlockingReaderWriterCircularBuffer` | 208 ns ±0.1% |
| `atomic_queue::AtomicQueueB2 (SPSC)` | 151 ns ±0.2% |
| `boost::lockfree::spsc_queue` | 144 ns ±0.1% |
| `lockfree::spsc::Queue` | 155 ns ±0.1% |
| `cds::container::WeakRingBuffer` | 148 ns ±0.3% |
| `xenium::vyukov_bounded_queue (MPMC)` | 198 ns ±0.1% |
| `xenium::nikolaev_bounded_queue (MPMC)` | 497 ns ±0.1% |
| `std::deque + std::mutex` | 1.24 µs ±0.8% |
<!-- benchmarks:end -->

## Related work

Other concurrent-container libraries are listed in
[references.md](references.md).

## License

[MIT](LICENSE)

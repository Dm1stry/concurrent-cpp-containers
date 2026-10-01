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
_Measured on 2026-10-02: AMD Ryzen 7 8845H w/ Radeon 780M Graphics, Linux, GCC 16.2.1, threads pinned to CPUs 2 and 4, median of 5 runs ± coefficient of variation (standard deviation / mean). Absolute numbers depend on the machine; compare the queues with each other._

**Throughput** (items per second by capacity, higher is better)

| Queue | 64 | 256 | 4096 | 65536 |
|---|---:|---:|---:|---:|
| `ccc::wait_free::spsc_queue` | 409 M ±0.7% | 572 M ±1% | 637 M ±3% | **847 M** ±15% |
| `rigtorp::SPSCQueue` | 379 M ±0.3% | 479 M ±0.3% | 460 M ±0.4% | 466 M ±0.6% |
| `moodycamel::ReaderWriterQueue` | **458 M** ±0.9% | **598 M** ±3% | **980 M** ±32% | 509 M ±8% |
| `moodycamel::BlockingReaderWriterCircularBuffer` | 24.4 M ±0.7% | 24.5 M ±1% | 24.1 M ±0.6% | 23.6 M ±0.9% |
| `atomic_queue::AtomicQueueB2 (SPSC)` | 115 M ±0.5% | 115 M ±0.4% | 115 M ±0.1% | 115 M ±0.5% |
| `boost::lockfree::spsc_queue` | 246 M ±1% | 249 M ±3% | 249 M ±6% | 272 M ±0.3% |
| `lockfree::spsc::Queue` | 168 M ±3% | 255 M ±1% | 273 M ±0.1% | 275 M ±0.2% |
| `cds::container::WeakRingBuffer` | 416 M ±0.4% | 505 M ±0.5% | 476 M ±0.2% | 468 M ±0.4% |
| `xenium::vyukov_bounded_queue (MPMC)` | 15.8 M ±0.5% | 15.8 M ±0.5% | 18.5 M ±1.0% | 18.8 M ±1.0% |
| `xenium::nikolaev_bounded_queue (MPMC)` | 13.4 M ±0.6% | 13.9 M ±0.4% | 7.77 M ±4% | 7.32 M ±3% |
| `std::deque + std::mutex` | 9.05 M ±3% | 14.8 M ±9% | 16.7 M ±10% | 19.5 M ±15% |

**Round trip** (time per iteration, lower is better)

| Queue | Time per iteration |
|---|---:|
| `ccc::wait_free::spsc_queue` | 138 ns ±2% |
| `rigtorp::SPSCQueue` | **130 ns** ±5% |
| `moodycamel::ReaderWriterQueue` | 143 ns ±1% |
| `moodycamel::BlockingReaderWriterCircularBuffer` | 197 ns ±0.4% |
| `atomic_queue::AtomicQueueB2 (SPSC)` | 153 ns ±0.2% |
| `boost::lockfree::spsc_queue` | 140 ns ±0.5% |
| `lockfree::spsc::Queue` | 146 ns ±0.8% |
| `cds::container::WeakRingBuffer` | 148 ns ±0.4% |
| `xenium::vyukov_bounded_queue (MPMC)` | 193 ns ±0.1% |
| `xenium::nikolaev_bounded_queue (MPMC)` | 455 ns ±0.2% |
| `std::deque + std::mutex` | 1.15 µs ±2% |
<!-- benchmarks:end -->

## Related work

Other concurrent-container libraries are listed in
[references.md](references.md).

## License

[MIT](LICENSE)

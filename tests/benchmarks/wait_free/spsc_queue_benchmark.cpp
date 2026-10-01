// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT
//
// Throughput and round-trip latency of ccc::wait_free::spsc_queue compared with
// boost::lockfree::spsc_queue and a mutex-protected queue.
//
// Results depend heavily on which cores the two threads run on. For stable
// numbers pin the process to two physical cores, e.g.
//   taskset -c 2,4 ./spsc_queue_benchmark

#include <cstddef>
#include <cstdint>
#include <optional>
#include <thread>

#include "benchmark/benchmark.h"
#include "boost_queue.hpp"
#include "ccc/wait_free/spsc_queue.hpp"
#include "locked_queue.hpp"

namespace ccc {
namespace {

using Value = std::uint64_t;

// The benchmarked thread pushes one element per iteration while a consumer
// thread pops them. Arg: queue capacity.
template <typename Queue>
void BM_Throughput(benchmark::State& state) {
	Queue queue(static_cast<std::size_t>(state.range(0)));
	const benchmark::IterationCount count = state.max_iterations;
	std::thread consumer([&queue, count] {
		for (benchmark::IterationCount i = 0; i < count; ++i) {
			while (!queue.pop().has_value()) {
			}
		}
	});

	Value value = 0;
	for (auto _ : state) {
		while (!queue.push(value)) {
		}
		++value;
	}
	// Up to `capacity` elements are still in flight when the timer stops; that
	// is negligible next to the iteration count.
	consumer.join();
	state.SetItemsProcessed(state.iterations());
}

// One iteration sends an element to an echo thread through `ping` and waits
// until it comes back through `pong`.
template <typename Queue>
void BM_RoundTrip(benchmark::State& state) {
	Queue ping(64);
	Queue pong(64);
	const benchmark::IterationCount count = state.max_iterations;
	std::thread echo([&ping, &pong, count] {
		for (benchmark::IterationCount i = 0; i < count; ++i) {
			std::optional<Value> value;
			while (!(value = ping.pop())) {
			}
			while (!pong.push(*value)) {
			}
		}
	});

	Value value = 0;
	for (auto _ : state) {
		while (!ping.push(value)) {
		}
		while (!pong.pop().has_value()) {
		}
		++value;
	}
	echo.join();
}

BENCHMARK_TEMPLATE(BM_Throughput, wait_free::spsc_queue<Value>)
    ->RangeMultiplier(16)
    ->Range(64, 1 << 16)
    ->UseRealTime();
BENCHMARK_TEMPLATE(BM_Throughput, baseline::boost_spsc_queue<Value>)
    ->RangeMultiplier(16)
    ->Range(64, 1 << 16)
    ->UseRealTime();
BENCHMARK_TEMPLATE(BM_Throughput, baseline::locked_queue<Value>)
    ->RangeMultiplier(16)
    ->Range(64, 1 << 16)
    ->UseRealTime();

BENCHMARK_TEMPLATE(BM_RoundTrip, wait_free::spsc_queue<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::boost_spsc_queue<Value>)
    ->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::locked_queue<Value>)->UseRealTime();

}  // namespace
}  // namespace ccc

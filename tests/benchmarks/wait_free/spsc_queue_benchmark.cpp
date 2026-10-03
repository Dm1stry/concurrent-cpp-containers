// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT
//
// Throughput and round-trip latency of ccc::wait_free::spsc_queue compared with
// the queues of other libraries (see references.md) and a mutex-protected queue.
//
// Results depend heavily on which cores the two threads run on. For stable
// numbers pin the process to two physical cores, e.g.
//   taskset -c 2,4 ./spsc_queue_benchmark
// tools/update_benchmarks.py does that and puts the results into README.md.

#include <cstddef>
#include <cstdint>
#include <optional>
#include <thread>

#include "benchmark/benchmark.h"
#include "boost_queue.hpp"
#include "ccc/wait_free/spsc_queue.hpp"
#include "locked_queue.hpp"
#include "third_party_queues.hpp"

namespace ccc
{
namespace
{

using Value = std::uint64_t;

// Labels the results with the class the adapter wraps, if it is an adapter.
template <typename Queue>
void SetQueueLabel(benchmark::State& state)
{
	if constexpr (requires { Queue::kName; }) state.SetLabel(Queue::kName);
}

// The benchmarked thread pushes one element per iteration while a consumer
// thread pops them and sums their values. Without the sum the compiler could
// drop the read of each popped slot, and the benchmark would measure only the
// exchange of indices. Arg: queue capacity.
template <typename Queue>
void BM_Throughput(benchmark::State& state)
{
	SetQueueLabel<Queue>(state);
	Queue                           queue(static_cast<std::size_t>(state.range(0)));
	const benchmark::IterationCount count = state.max_iterations;
	std::thread                     consumer(
		[&queue, count]
		{
			Value sum = 0;
			for (benchmark::IterationCount i = 0; i < count; ++i)
			{
				std::optional<Value> value;
				while (!(value = queue.pop()))
				{
				}
				sum += *value;
			}
			benchmark::DoNotOptimize(sum);
		});

	Value value = 0;
	for (auto _ : state)
	{
		while (!queue.push(value))
		{
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
void BM_RoundTrip(benchmark::State& state)
{
	SetQueueLabel<Queue>(state);
	Queue                           ping(64);
	Queue                           pong(64);
	const benchmark::IterationCount count = state.max_iterations;
	std::thread                     echo(
		[&ping, &pong, count]
		{
			for (benchmark::IterationCount i = 0; i < count; ++i)
			{
				std::optional<Value> value;
				while (!(value = ping.pop()))
				{
				}
				while (!pong.push(*value))
				{
				}
			}
		});

	Value value = 0;
	Value sum = 0;  // Read every echoed element, as in BM_Throughput.
	for (auto _ : state)
	{
		while (!ping.push(value))
		{
		}
		std::optional<Value> echoed;
		while (!(echoed = pong.pop()))
		{
		}
		sum += *echoed;
		++value;
	}
	echo.join();
	benchmark::DoNotOptimize(sum);
}

// Capacities for BM_Throughput: 64, 256, 4096 and 65536.
void ThroughputArgs(benchmark::Benchmark* benchmark)
{
	benchmark->ArgName("capacity")->RangeMultiplier(16)->Range(64, 1 << 16)->UseRealTime();
}

// The queue whose capacity is a template argument gets one registration per
// capacity, with the same arguments as ThroughputArgs.
template <std::size_t kCapacity>
using DnedicQueue = baseline::dnedic_spsc_queue<Value, kCapacity>;

BENCHMARK_TEMPLATE(BM_Throughput, wait_free::spsc_queue<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, baseline::rigtorp_spsc_queue<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, baseline::moodycamel_reader_writer_queue<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, baseline::moodycamel_circular_buffer<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, baseline::atomic_queue_spsc<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, baseline::boost_spsc_queue<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, DnedicQueue<64>)->ArgName("capacity")->Arg(64)->UseRealTime();
BENCHMARK_TEMPLATE(BM_Throughput, DnedicQueue<256>)->ArgName("capacity")->Arg(256)->UseRealTime();
BENCHMARK_TEMPLATE(BM_Throughput, DnedicQueue<4096>)->ArgName("capacity")->Arg(4096)->UseRealTime();
BENCHMARK_TEMPLATE(BM_Throughput, DnedicQueue<65536>)->ArgName("capacity")->Arg(65536)->UseRealTime();
#if CCC_BENCHMARK_LIBCDS
BENCHMARK_TEMPLATE(BM_Throughput, baseline::libcds_weak_ring_buffer<Value>)->Apply(ThroughputArgs);
#endif
#if CCC_BENCHMARK_XENIUM
BENCHMARK_TEMPLATE(BM_Throughput, baseline::xenium_vyukov_queue<Value>)->Apply(ThroughputArgs);
BENCHMARK_TEMPLATE(BM_Throughput, baseline::xenium_nikolaev_queue<Value>)->Apply(ThroughputArgs);
#endif
BENCHMARK_TEMPLATE(BM_Throughput, baseline::locked_queue<Value>)->Apply(ThroughputArgs);

BENCHMARK_TEMPLATE(BM_RoundTrip, wait_free::spsc_queue<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::rigtorp_spsc_queue<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::moodycamel_reader_writer_queue<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::moodycamel_circular_buffer<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::atomic_queue_spsc<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::boost_spsc_queue<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, DnedicQueue<64>)->UseRealTime();
#if CCC_BENCHMARK_LIBCDS
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::libcds_weak_ring_buffer<Value>)->UseRealTime();
#endif
#if CCC_BENCHMARK_XENIUM
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::xenium_vyukov_queue<Value>)->UseRealTime();
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::xenium_nikolaev_queue<Value>)->UseRealTime();
#endif
BENCHMARK_TEMPLATE(BM_RoundTrip, baseline::locked_queue<Value>)->UseRealTime();

}  // namespace
}  // namespace ccc

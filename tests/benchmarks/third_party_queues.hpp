// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT

#ifndef CCC_BENCHMARKS_THIRD_PARTY_QUEUES_HPP_
#define CCC_BENCHMARKS_THIRD_PARTY_QUEUES_HPP_

// Queues from the libraries in references.md, behind the same push()/pop()
// interface as the ccc queues. kName is the original class; the benchmarks
// report it as their label. Popping into a default-constructed value needs a
// default-constructible T, which the benchmarked integers are.

#include <bit>
#include <cassert>
#include <cstddef>
#include <memory>
#include <optional>
#include <utility>

#include "atomic_queue/atomic_queue.h"
#include "lockfree/spsc/queue.hpp"
#include "readerwritercircularbuffer.h"
#include "readerwriterqueue.h"
#include "rigtorp/SPSCQueue.h"

#if CCC_BENCHMARK_XENIUM
#include "xenium/nikolaev_bounded_queue.hpp"
#include "xenium/vyukov_bounded_queue.hpp"
#endif

#if CCC_BENCHMARK_LIBCDS
#include "cds/container/weak_ringbuffer.h"
#endif

namespace ccc::baseline
{

template <typename T>
class rigtorp_spsc_queue
{
public:
	static constexpr const char* kName = "rigtorp::SPSCQueue";

	explicit rigtorp_spsc_queue(std::size_t capacity)
	 : queue_(capacity)
	{
	}

	bool push(const T& value) { return queue_.try_push(value); }

	std::optional<T> pop()
	{
		T* front = queue_.front();
		if (front == nullptr) return std::nullopt;
		std::optional<T> value(std::move(*front));
		queue_.pop();
		return value;
	}

private:
	rigtorp::SPSCQueue<T> queue_;
};

// Unbounded in general, but try_enqueue() never allocates, so the queue holds
// at most the capacity it was created with.
template <typename T>
class moodycamel_reader_writer_queue
{
public:
	static constexpr const char* kName = "moodycamel::ReaderWriterQueue";

	explicit moodycamel_reader_writer_queue(std::size_t capacity)
	 : queue_(capacity)
	{
	}

	bool push(const T& value) { return queue_.try_enqueue(value); }

	std::optional<T> pop()
	{
		T value{};
		if (!queue_.try_dequeue(value)) return std::nullopt;
		return value;
	}

private:
	moodycamel::ReaderWriterQueue<T> queue_;
};

template <typename T>
class moodycamel_circular_buffer
{
public:
	static constexpr const char* kName = "moodycamel::BlockingReaderWriterCircularBuffer";

	explicit moodycamel_circular_buffer(std::size_t capacity)
	 : queue_(capacity)
	{
	}

	bool push(const T& value) { return queue_.try_enqueue(value); }

	std::optional<T> pop()
	{
		T value{};
		if (!queue_.try_dequeue(value)) return std::nullopt;
		return value;
	}

private:
	moodycamel::BlockingReaderWriterCircularBuffer<T> queue_;
};

// AtomicQueueB2 rather than AtomicQueueB: the latter reserves one value of T
// (zero by default) to mark empty slots, and the benchmarks push zero.
template <typename T>
class atomic_queue_spsc
{
public:
	static constexpr const char* kName = "atomic_queue::AtomicQueueB2 (SPSC)";

	explicit atomic_queue_spsc(std::size_t capacity)
	 : queue_(static_cast<unsigned>(capacity))
	{
	}

	bool push(const T& value) { return queue_.try_push(value); }

	std::optional<T> pop()
	{
		T value{};
		if (!queue_.try_pop(value)) return std::nullopt;
		return value;
	}

private:
	// Allocator, MAXIMIZE_THROUGHPUT, TOTAL_ORDER, SPSC.
	atomic_queue::AtomicQueueB2<T, std::allocator<T>, true, false, true> queue_;
};

// The capacity is a template argument, so each capacity is a separate type; the
// constructor argument only has to agree with it. The library keeps one slot
// empty, hence the extra one.
template <typename T, std::size_t kCapacity>
class dnedic_spsc_queue
{
public:
	static constexpr const char* kName = "lockfree::spsc::Queue";

	explicit dnedic_spsc_queue([[maybe_unused]] std::size_t capacity) { assert(capacity == kCapacity); }

	bool push(const T& value) { return queue_.Push(value); }

	std::optional<T> pop() { return queue_.Pop(); }

private:
	lockfree::spsc::Queue<T, kCapacity + 1> queue_;
};

#if CCC_BENCHMARK_XENIUM

// A multi-producer multi-consumer queue, measured with one thread on each side.
template <typename T>
class xenium_vyukov_queue
{
public:
	static constexpr const char* kName = "xenium::vyukov_bounded_queue (MPMC)";

	// The size must be a power of two.
	explicit xenium_vyukov_queue(std::size_t capacity)
	 : queue_(std::bit_ceil(capacity))
	{
	}

	bool push(const T& value) { return queue_.try_push(value); }

	std::optional<T> pop()
	{
		T value{};
		if (!queue_.try_pop(value)) return std::nullopt;
		return value;
	}

private:
	xenium::vyukov_bounded_queue<T> queue_;
};

// A multi-producer multi-consumer queue, measured with one thread on each side.
template <typename T>
class xenium_nikolaev_queue
{
public:
	static constexpr const char* kName = "xenium::nikolaev_bounded_queue (MPMC)";

	explicit xenium_nikolaev_queue(std::size_t capacity)
	 : queue_(capacity)
	{
	}

	bool push(const T& value) { return queue_.try_push(value); }

	std::optional<T> pop()
	{
		T value{};
		if (!queue_.try_pop(value)) return std::nullopt;
		return value;
	}

private:
	xenium::nikolaev_bounded_queue<T> queue_;
};

#endif  // CCC_BENCHMARK_XENIUM

#if CCC_BENCHMARK_LIBCDS

template <typename T>
class libcds_weak_ring_buffer
{
public:
	static constexpr const char* kName = "cds::container::WeakRingBuffer";

	explicit libcds_weak_ring_buffer(std::size_t capacity)
	 : queue_(capacity)
	{
	}

	bool push(const T& value) { return queue_.push(value); }

	std::optional<T> pop()
	{
		T value{};
		if (!queue_.pop(value)) return std::nullopt;
		return value;
	}

private:
	cds::container::WeakRingBuffer<T> queue_;
};

#endif  // CCC_BENCHMARK_LIBCDS

}  // namespace ccc::baseline

#endif  // CCC_BENCHMARKS_THIRD_PARTY_QUEUES_HPP_

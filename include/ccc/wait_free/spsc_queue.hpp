// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT

#ifndef CCC_WAIT_FREE_SPSC_QUEUE_HPP
#define CCC_WAIT_FREE_SPSC_QUEUE_HPP

#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

#include "ccc/detail/cache_line.hpp"

namespace ccc::wait_free
{

// A bounded wait-free FIFO queue (ring buffer) for one producer thread and one
// consumer thread.
//
// Thread safety:
//   * push() and emplace() may be called only by the producer thread, pop()
//     only by the consumer thread. Two threads pushing, or two threads
//     popping, at the same time is a data race.
//   * capacity(), size() and empty() may be called from any thread.
//   * A role may move to another thread only through a happens-before handoff,
//     e.g. std::thread::join() or a mutex.
//   * The destructor must not run concurrently with any other member function:
//     both threads must be done with the queue, e.g. joined.
//
// Progress: push(), emplace() and pop() are wait-free. Each completes in a
// bounded number of steps whatever the other thread is doing.
//
// T must be nothrow move constructible and nothrow destructible; store
// std::unique_ptr<T> for other types. The queue is neither copyable nor
// movable.
template <typename T, typename Allocator = std::allocator<T>>
class spsc_queue
{
	static_assert(std::is_object_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>,
	              "ccc::spsc_queue requires a non-cv-qualified object type");
	static_assert(std::is_nothrow_move_constructible_v<T>,
	              "ccc::spsc_queue requires a nothrow move constructible T; "
	              "store std::unique_ptr<T> instead");
	static_assert(std::is_nothrow_destructible_v<T>, "ccc::spsc_queue requires a nothrow destructible T");

public:
	// Creates an empty queue for up to `capacity` elements, allocating
	// `capacity + 1` slots from `alloc`. Aborts if `alloc` cannot provide that
	// many elements.
	explicit spsc_queue(std::size_t capacity, const Allocator& alloc = Allocator());
	spsc_queue(const spsc_queue&) = delete;
	spsc_queue(spsc_queue&&) = delete;
	spsc_queue& operator=(const spsc_queue&) = delete;
	spsc_queue& operator=(spsc_queue&&) = delete;
	~spsc_queue();

	// Producer only. Each returns false if the queue is full, leaving the
	// arguments untouched. If constructing the element throws, the exception
	// propagates and the queue is left unchanged.
	[[nodiscard]]
	bool push(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
		requires std::copy_constructible<T>;

	[[nodiscard]]
	bool push(T&& value) noexcept;

	template <typename... Args>
	[[nodiscard]]
	bool emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
		requires std::constructible_from<T, Args...>;

	// Consumer only. Removes and returns the oldest element, or std::nullopt if
	// the queue is empty.
	[[nodiscard]]
	std::optional<T> pop() noexcept;

	// The maximum number of elements the queue holds.
	std::size_t capacity() const noexcept;

	// Exact while no push or pop runs concurrently. Otherwise a snapshot that
	// may be out of date by the time it returns: never below the real size in
	// the producer thread, never above it in the consumer thread, and within
	// [0, capacity()] in any other thread. empty() gives the same guarantees
	// as size() == 0.
	std::size_t size() const noexcept;
	bool empty() const noexcept;

private:
	std::size_t next_index(const std::size_t index) const noexcept;

	// index of last popped element of the ring buffer (last free element)
	alignas(detail::kDestructiveInterferenceSize) std::atomic<std::size_t> head_{0};

	// cached index of last popped element of the ring buffer (to reduce amount of atomic loads)
	alignas(detail::kDestructiveInterferenceSize) std::size_t head_cached_{0};

	// index of last pushed element of the ring buffer
	alignas(detail::kDestructiveInterferenceSize) std::atomic<std::size_t> tail_{0};

	// cached index of last pushed element of the ring buffer (to reduce amount of atomic loads)
	alignas(detail::kDestructiveInterferenceSize) std::size_t tail_cached_{0};

	alignas(detail::kDestructiveInterferenceSize) const std::size_t capacity_;

	[[no_unique_address]]
	Allocator allocator_;
	// Storage of the ring buffer
	T*        storage_;
};

template <typename T, typename Allocator>
spsc_queue<T, Allocator>::spsc_queue(std::size_t capacity, const Allocator& allocator)
 : capacity_(capacity + 1),  // Here we have to add 1 to capacity because we have to keep one empty slot
                             // in the ring buffer to distinguish between full and empty states
   allocator_(allocator)
{
	if (capacity >= std::allocator_traits<Allocator>::max_size(allocator_))
	{
		std::abort();
	}
	storage_ = std::allocator_traits<Allocator>::allocate(allocator_, capacity_);
}

template <typename T, typename Allocator>
spsc_queue<T, Allocator>::~spsc_queue()
{
	// Elements are alive exactly in [head_, tail_); the other slots are raw
	// memory. Both threads are done with the queue by now, so relaxed loads see
	// the final indices.
	const std::size_t tail = tail_.load(std::memory_order_relaxed);
	for (std::size_t i = head_.load(std::memory_order_relaxed); i != tail; i = next_index(i))
	{
		std::allocator_traits<Allocator>::destroy(allocator_, &storage_[i]);
	}
	std::allocator_traits<Allocator>::deallocate(allocator_, storage_, capacity_);
}

template <typename T, typename Allocator>
bool spsc_queue<T, Allocator>::push(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
	requires std::copy_constructible<T>
{
	return emplace(value);
}

template <typename T, typename Allocator>
bool spsc_queue<T, Allocator>::push(T&& value) noexcept
{
	return emplace(std::move(value));
}

template <typename T, typename Allocator>
template <typename... Args>
bool spsc_queue<T, Allocator>::emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
	requires std::constructible_from<T, Args...>
{
	// Here doesn't matter if curr_tail would be
	// loaded earlier of later for other thread,
	// because we the use tail_.load(std::memory_order_acquire)
	// that is creates second barrier
	const std::size_t curr_tail = tail_.load(std::memory_order_relaxed);

	// next tail is used multiple times so it
	// have to be saved
	const std::size_t next_tail = next_index(curr_tail);

	// If buffer is full return to inform user and make him wait
	// Here we create barrier that makes thread read head_ strictly
	// after it stored in other thread's pop

	// We check if last time there was no free space
	if (next_tail == head_cached_)
	{
		// and load actual value only if last time there wasn't any space
		head_cached_ = head_.load(std::memory_order_acquire);

		// to check is situation is changed
		if (next_tail == head_cached_)
		{
			// and if it isn't then we inform user that buffer is full
			return false;
		}
	}
	std::allocator_traits<Allocator>::construct(allocator_, &storage_[curr_tail],
	                                            std::forward<Args>(args)...);

	// Here we create barrier that guarantees that value placed in storgage_ by
	// the time of this string execution tail_ strictly after it stored here
	tail_.store(next_tail, std::memory_order_release);

	// Inform user that value is pushed
	return true;
}

template <typename T, typename Allocator>
std::optional<T> spsc_queue<T, Allocator>::pop() noexcept
{
	// Here doesn't matter if curr_head would be
	// loaded earlier of later for other thread,
	// because we the use head_.load(std::memory_order_acquire)
	// that is creates second barrier
	const std::size_t curr_head = head_.load(std::memory_order_relaxed);

	// We check if last time there was no elements in the buffer
	if (curr_head == tail_cached_)
	{
		// and load actual value only if last time there wasn't any elements
		tail_cached_ = tail_.load(std::memory_order_acquire);

		// to check is situation is changed
		if (curr_head == tail_cached_)
		{
			// and if it isn't then we inform user that buffer is empty
			return std::nullopt;
		}
	}

	// Move data from storage to the _value (for non-trivial types)
	std::optional<T> value(std::move(storage_[curr_head]));
	// Zero-cost for trivial types, but required for non-trivial types with fake move based on copy
	// constructor.
	std::allocator_traits<Allocator>::destroy(allocator_, &storage_[curr_head]);

	// Here we create barrier that makes other thread's push read
	// head_ strictly after it stored here
	head_.store(next_index(curr_head), std::memory_order_release);

	// Inform user that value is pop
	return value;
}

template <typename T, typename Allocator>
std::size_t spsc_queue<T, Allocator>::capacity() const noexcept
{
	return capacity_ - 1;
}

template <typename T, typename Allocator>
std::size_t spsc_queue<T, Allocator>::size() const noexcept
{
	std::size_t head = head_.load(std::memory_order_relaxed);
	std::size_t tail = tail_.load(std::memory_order_relaxed);
	std::size_t size = (tail >= head) ? (tail - head) : (capacity_ - head + tail);
	return size;
}

template <typename T, typename Allocator>
bool spsc_queue<T, Allocator>::empty() const noexcept
{
	return head_.load(std::memory_order_relaxed) == tail_.load(std::memory_order_relaxed);
}

template <typename T, typename Allocator>
std::size_t spsc_queue<T, Allocator>::next_index(const std::size_t index) const noexcept
{
	// Profiling showed that it's most efficient way
	return (index + 1 == capacity_) ? 0 : (index + 1);
}

}  // namespace ccc::wait_free

#endif  // CCC_WAIT_FREE_SPSC_QUEUE_HPP
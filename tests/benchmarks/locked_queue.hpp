// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT

#ifndef CCC_BENCHMARKS_LOCKED_QUEUE_HPP_
#define CCC_BENCHMARKS_LOCKED_QUEUE_HPP_

#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <utility>

namespace ccc::baseline
{

// A bounded FIFO queue guarded by a single std::mutex, with the same
// push()/pop() interface as the ccc queues. It is the baseline the
// benchmarks compare against.
template <typename T>
class locked_queue
{
public:
	static constexpr const char* kName = "std::deque + std::mutex";

	explicit locked_queue(std::size_t capacity)
	 : capacity_(capacity)
	{
	}

	bool push(T value)
	{
		std::lock_guard lock(mutex_);
		if (items_.size() == capacity_) return false;
		items_.push_back(std::move(value));
		return true;
	}

	std::optional<T> pop()
	{
		std::lock_guard lock(mutex_);
		if (items_.empty()) return std::nullopt;
		std::optional<T> value(std::move(items_.front()));
		items_.pop_front();
		return value;
	}

private:
	const std::size_t capacity_;
	std::mutex        mutex_;
	std::deque<T>     items_;
};

}  // namespace ccc::baseline

#endif  // CCC_BENCHMARKS_LOCKED_QUEUE_HPP_

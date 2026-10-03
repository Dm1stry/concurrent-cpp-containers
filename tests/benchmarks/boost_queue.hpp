// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT

#ifndef CCC_BENCHMARKS_BOOST_QUEUE_HPP_
#define CCC_BENCHMARKS_BOOST_QUEUE_HPP_

#include <cstddef>
#include <optional>

#include "boost/lockfree/spsc_queue.hpp"

namespace ccc::baseline
{

// boost::lockfree::spsc_queue with its capacity set at run time, behind the
// same push()/pop() interface as the ccc queues. It is the reference lock-free
// implementation the benchmarks compare against.
template <typename T>
class boost_spsc_queue
{
public:
	static constexpr const char* kName = "boost::lockfree::spsc_queue";

	explicit boost_spsc_queue(std::size_t capacity)
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
	boost::lockfree::spsc_queue<T> queue_;
};

}  // namespace ccc::baseline

#endif  // CCC_BENCHMARKS_BOOST_QUEUE_HPP_

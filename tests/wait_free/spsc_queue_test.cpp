// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT

#include "ccc/wait_free/spsc_queue.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

namespace ccc::wait_free {
namespace {

// -----------------------------------------------------------------------------
// Helper types.
// -----------------------------------------------------------------------------

// Has no default constructor, which rules out storing elements in a plain
// `T[]` or `std::vector<T>`.
struct NoDefault {
	explicit NoDefault(int v) : value(v) {}
	int value;
};

// Needs storage aligned beyond `alignof(std::max_align_t)`. UBSan reports a
// misaligned construction.
struct alignas(64) OverAligned {
	std::uint64_t value;
};

// Keeps `*live` equal to the number of LiveCounter objects that exist, so a
// leaked or doubly destroyed element shows up in the count.
class LiveCounter {
public:
	explicit LiveCounter(int* live) : live_(live) { ++*live_; }
	LiveCounter(const LiveCounter& other) : live_(other.live_) { ++*live_; }
	LiveCounter(LiveCounter&& other) noexcept : live_(other.live_) { ++*live_; }
	LiveCounter& operator=(const LiveCounter&) = delete;
	~LiveCounter() { --*live_; }

private:
	int* live_;
};

// Copying throws if `fail_copy` is set; moving never throws.
struct ThrowsOnCopy {
	explicit ThrowsOnCopy(int v, bool fail = false)
	    : value(v), fail_copy(fail) {}
	ThrowsOnCopy(const ThrowsOnCopy& other)
	    : value(other.value), fail_copy(other.fail_copy) {
		if (fail_copy) throw std::runtime_error("copy failed");
	}
	ThrowsOnCopy(ThrowsOnCopy&&) noexcept = default;

	int value;
	bool fail_copy;
};

// -----------------------------------------------------------------------------
// Single-threaded behaviour.
// -----------------------------------------------------------------------------

TEST(SpscQueueTest, NewQueueIsEmpty) {
	spsc_queue<int> queue(4);
	EXPECT_TRUE(queue.empty());
	EXPECT_EQ(queue.size(), 0u);
	EXPECT_EQ(queue.pop(), std::nullopt);
}

TEST(SpscQueueTest, CapacityIsAtLeastRequested) {
	constexpr std::size_t kRequested[] = {1, 2, 3, 7, 8, 1000, 1024};
	for (const std::size_t requested : kRequested) {
		spsc_queue<int> queue(requested);
		EXPECT_GE(queue.capacity(), requested);
	}
}

TEST(SpscQueueTest, PopsInFifoOrder) {
	spsc_queue<int> queue(8);
	for (int i = 0; i < 5; ++i) ASSERT_TRUE(queue.push(i));
	for (int i = 0; i < 5; ++i) EXPECT_EQ(queue.pop(), i);
	EXPECT_EQ(queue.pop(), std::nullopt);
}

TEST(SpscQueueTest, PushFailsOnlyWhenFull) {
	spsc_queue<int> queue(4);
	const std::size_t capacity = queue.capacity();
	ASSERT_GE(capacity, 4u);

	for (std::size_t i = 0; i < capacity; ++i) {
		ASSERT_TRUE(queue.push(static_cast<int>(i))) << "push #" << i;
	}
	EXPECT_FALSE(queue.push(-1));
	EXPECT_EQ(queue.size(), capacity);

	EXPECT_EQ(queue.pop(), 0);
	EXPECT_TRUE(queue.push(-1));
	EXPECT_FALSE(queue.push(-2));
}

TEST(SpscQueueTest, SizeTracksPushesAndPops) {
	spsc_queue<int> queue(8);
	ASSERT_GE(queue.capacity(), 8u);

	for (std::size_t i = 0; i < 8; ++i) {
		EXPECT_EQ(queue.size(), i);
		ASSERT_TRUE(queue.push(static_cast<int>(i)));
	}
	EXPECT_FALSE(queue.empty());
	for (std::size_t i = 8; i > 0; --i) {
		EXPECT_EQ(queue.size(), i);
		ASSERT_TRUE(queue.pop().has_value());
	}
	EXPECT_TRUE(queue.empty());
}

// Walks the occupied region around the end of the buffer at every fill level.
TEST(SpscQueueTest, KeepsFifoOrderAcrossWrapAround) {
	spsc_queue<int> queue(5);
	const std::size_t capacity = queue.capacity();
	ASSERT_GE(capacity, 5u);

	int next_push = 0;
	int next_pop = 0;
	for (std::size_t in_flight = 0; in_flight < capacity; ++in_flight) {
		for (std::size_t i = 0; i < in_flight; ++i) {
			ASSERT_TRUE(queue.push(next_push++));
		}
		for (std::size_t step = 0; step < 3 * capacity; ++step) {
			ASSERT_TRUE(queue.push(next_push++));
			ASSERT_EQ(queue.pop(), next_pop++);
		}
		for (std::size_t i = 0; i < in_flight; ++i) {
			ASSERT_EQ(queue.pop(), next_pop++);
		}
		ASSERT_TRUE(queue.empty());
	}
}

TEST(SpscQueueTest, SupportsMoveOnlyTypes) {
	spsc_queue<std::unique_ptr<int>> queue(2);
	ASSERT_TRUE(queue.push(std::make_unique<int>(42)));
	std::optional<std::unique_ptr<int>> value = queue.pop();
	ASSERT_TRUE(value.has_value());
	ASSERT_NE(*value, nullptr);
	EXPECT_EQ(**value, 42);
}

TEST(SpscQueueTest, SupportsTypesWithoutDefaultConstructor) {
	spsc_queue<NoDefault> queue(2);
	ASSERT_TRUE(queue.push(NoDefault(7)));
	std::optional<NoDefault> value = queue.pop();
	ASSERT_TRUE(value.has_value());
	EXPECT_EQ(value->value, 7);
}

TEST(SpscQueueTest, SupportsOverAlignedTypes) {
	spsc_queue<OverAligned> queue(4);
	for (std::uint64_t round = 0; round < 3; ++round) {
		for (std::uint64_t i = 0; i < 4; ++i) {
			ASSERT_TRUE(queue.push(OverAligned{round * 4 + i}));
		}
		for (std::uint64_t i = 0; i < 4; ++i) {
			std::optional<OverAligned> value = queue.pop();
			ASSERT_TRUE(value.has_value());
			EXPECT_EQ(value->value, round * 4 + i);
		}
	}
}

TEST(SpscQueueTest, EmplaceForwardsArguments) {
	spsc_queue<std::pair<int, std::string>> queue(2);
	ASSERT_TRUE(queue.emplace(7, "seven"));
	EXPECT_EQ(queue.pop(), std::make_pair(7, std::string("seven")));
}

TEST(SpscQueueTest, CopyPushLeavesSourceIntact) {
	spsc_queue<std::string> queue(2);
	const std::string source(100, 'x');  // Long enough to live on the heap.
	ASSERT_TRUE(queue.push(source));
	EXPECT_EQ(source, std::string(100, 'x'));
	EXPECT_EQ(queue.pop(), source);
}

TEST(SpscQueueTest, FailedPushLeavesArgumentIntact) {
	spsc_queue<std::unique_ptr<int>> queue(1);
	for (std::size_t i = 0; i < queue.capacity(); ++i) {
		ASSERT_TRUE(queue.push(std::make_unique<int>(0)));
	}

	// Using `value` after std::move is deliberate: a failed push promises not
	// to move from its argument.
	auto value = std::make_unique<int>(42);
	EXPECT_FALSE(queue.push(std::move(value)));
	ASSERT_NE(value, nullptr);
	EXPECT_FALSE(queue.emplace(std::move(value)));
	ASSERT_NE(value, nullptr);
	EXPECT_EQ(*value, 42);
}

TEST(SpscQueueTest, DestroysEveryElementExactlyOnce) {
	int live = 0;
	{
		spsc_queue<LiveCounter> queue(4);
		ASSERT_GE(queue.capacity(), 4u);
		// Pop a few elements first, so the remaining ones wrap around the end
		// of the buffer when the queue is destroyed.
		for (int i = 0; i < 3; ++i) ASSERT_TRUE(queue.push(LiveCounter(&live)));
		for (int i = 0; i < 2; ++i) ASSERT_TRUE(queue.pop().has_value());
		EXPECT_EQ(live, 1);
		for (int i = 0; i < 3; ++i) ASSERT_TRUE(queue.push(LiveCounter(&live)));
		EXPECT_EQ(live, 4);
		{
			std::optional<LiveCounter> popped = queue.pop();
			ASSERT_TRUE(popped.has_value());
			EXPECT_EQ(live, 4);  // Three in the queue and one in `popped`.
		}
		EXPECT_EQ(live, 3);
	}
	EXPECT_EQ(live, 0);
}

TEST(SpscQueueTest, ThrowingCopyLeavesQueueUnchanged) {
	spsc_queue<ThrowsOnCopy> queue(4);
	ASSERT_TRUE(queue.push(ThrowsOnCopy(1)));

	const ThrowsOnCopy poisoned(2, /*fail=*/true);
	EXPECT_THROW(static_cast<void>(queue.push(poisoned)), std::runtime_error);
	EXPECT_EQ(queue.size(), 1u);

	// The slot the failed copy was constructed in must still be usable.
	ASSERT_TRUE(queue.push(ThrowsOnCopy(3)));
	std::optional<ThrowsOnCopy> first = queue.pop();
	ASSERT_TRUE(first.has_value());
	EXPECT_EQ(first->value, 1);
	std::optional<ThrowsOnCopy> second = queue.pop();
	ASSERT_TRUE(second.has_value());
	EXPECT_EQ(second->value, 3);
	EXPECT_TRUE(queue.empty());
}

// -----------------------------------------------------------------------------
// One producer thread and one consumer thread.
//
// These tests pass on x86 even with wrong memory orders more often than not;
// run them under ThreadSanitizer (the `tsan` preset) and on ARM. To hammer a
// change locally, repeat them:
//   ctest --preset tsan -R Concurrency --repeat until-fail:100
// -----------------------------------------------------------------------------

// How many elements each test sends from the producer to the consumer.
constexpr std::size_t kTransferCount = 100'000;

// A push or pop that keeps failing for this long means the other side has
// stopped making progress, or the queue has lost an update.
constexpr auto kStallTimeout = std::chrono::seconds(5);

// Calls `op` until it returns true. Returns false if that takes longer than
// kStallTimeout.
template <typename Op>
bool RetryUntilSuccess(Op op) {
	const auto deadline = std::chrono::steady_clock::now() + kStallTimeout;
	while (!op()) {
		if (std::chrono::steady_clock::now() > deadline) return false;
		std::this_thread::yield();
	}
	return true;
}

struct TransferStats {
	std::size_t pushed = 0;
	std::size_t popped = 0;
	std::size_t mismatches = 0;
};

// Pushes make(0), ..., make(count - 1) from a new producer thread while the
// calling thread pops them, and counts the popped values for which
// `matches(value, index)` is false.
template <typename T, typename Make, typename Matches>
TransferStats TransferConcurrently(spsc_queue<T>& queue, std::size_t count,
                                   Make make, Matches matches) {
	TransferStats stats;
	std::thread producer([&queue, &stats, count, &make] {
		for (; stats.pushed < count; ++stats.pushed) {
			T value = make(stats.pushed);
			// Relies on a failed push leaving `value` untouched.
			if (!RetryUntilSuccess(
			        [&] { return queue.push(std::move(value)); })) {
				return;
			}
		}
	});
	for (; stats.popped < count; ++stats.popped) {
		std::optional<T> value;
		if (!RetryUntilSuccess([&] {
			    value = queue.pop();
			    return value.has_value();
		    })) {
			break;
		}
		if (!matches(*value, stats.popped)) ++stats.mismatches;
	}
	producer.join();
	return stats;
}

void ExpectCompleteTransfer(const TransferStats& stats) {
	EXPECT_EQ(stats.pushed, kTransferCount) << "the producer stalled";
	EXPECT_EQ(stats.popped, kTransferCount) << "the consumer stalled";
	EXPECT_EQ(stats.mismatches, 0u);
}

// Small capacities keep the queue switching between full and empty, which is
// where the producer and the consumer actually race.
class SpscQueueConcurrencyTest : public testing::TestWithParam<std::size_t> {};

INSTANTIATE_TEST_SUITE_P(
    Capacities, SpscQueueConcurrencyTest, testing::Values(1, 2, 64),
    [](const testing::TestParamInfo<std::size_t>& param_info) {
	    return "Capacity" + std::to_string(param_info.param);
    });

TEST_P(SpscQueueConcurrencyTest, PreservesFifoOrder) {
	spsc_queue<std::uint64_t> queue(GetParam());
	ExpectCompleteTransfer(TransferConcurrently(
	    queue, kTransferCount,
	    [](std::size_t i) { return static_cast<std::uint64_t>(i); },
	    [](std::uint64_t value, std::size_t i) { return value == i; }));
}

// A 64-byte element takes several stores to write. If the consumer can read a
// slot before the producer has finished writing it, the words disagree.
TEST_P(SpscQueueConcurrencyTest, NeverExposesPartiallyWrittenElements) {
	struct Wide {
		std::uint64_t words[8];
	};
	spsc_queue<Wide> queue(GetParam());
	ExpectCompleteTransfer(TransferConcurrently(
	    queue, kTransferCount,
	    [](std::size_t i) {
		    Wide wide{};
		    for (std::uint64_t& word : wide.words) word = i;
		    return wide;
	    },
	    [](const Wide& wide, std::size_t i) {
		    for (const std::uint64_t word : wide.words) {
			    if (word != i) return false;
		    }
		    return true;
	    }));
}

// Every element owns heap memory that the producer allocates and writes and
// the consumer reads and frees. A missing happens-before edge shows up as a
// data race under TSan or as a use-after-free under ASan.
TEST_P(SpscQueueConcurrencyTest, HandsOverHeapOwningElements) {
	const auto make = [](std::size_t i) {
		return std::string(40, 'a') + std::to_string(i);
	};
	spsc_queue<std::string> queue(GetParam());
	ExpectCompleteTransfer(
	    TransferConcurrently(queue, kTransferCount, make,
	                         [&make](const std::string& value, std::size_t i) {
		                         return value == make(i);
	                         }));
}

// `size()`, `empty()` and `capacity()` may be called from any thread.
TEST_P(SpscQueueConcurrencyTest, ObserversStayInRangeDuringTransfer) {
	spsc_queue<std::uint64_t> queue(GetParam());
	std::atomic<bool> done{false};
	std::size_t out_of_range = 0;
	std::thread observer([&queue, &done, &out_of_range] {
		while (!done.load(std::memory_order_relaxed)) {
			if (queue.size() > queue.capacity()) ++out_of_range;
			static_cast<void>(queue.empty());
		}
	});
	ExpectCompleteTransfer(TransferConcurrently(
	    queue, kTransferCount,
	    [](std::size_t i) { return static_cast<std::uint64_t>(i); },
	    [](std::uint64_t value, std::size_t i) { return value == i; }));
	done.store(true, std::memory_order_relaxed);
	observer.join();
	EXPECT_EQ(out_of_range, 0u);
}

}  // namespace
}  // namespace ccc::wait_free

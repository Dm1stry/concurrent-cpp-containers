// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT

#ifndef CCC_DETAIL_CACHE_LINE_HPP
#define CCC_DETAIL_CACHE_LINE_HPP

#include <cstddef>

namespace ccc::detail
{

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
inline constexpr std::size_t kDestructiveInterferenceSize =
	128;  // 64-byte lines, but adjacent-line prefetch pulls pairs
#elif defined(__aarch64__) || defined(_M_ARM64)
inline constexpr std::size_t kDestructiveInterferenceSize =
	128;  // Apple M-series and some server cores use 128-byte lines
#elif defined(__powerpc64__)
inline constexpr std::size_t kDestructiveInterferenceSize = 128;
#elif defined(__s390x__)
inline constexpr std::size_t kDestructiveInterferenceSize = 256;
#else
inline constexpr std::size_t kDestructiveInterferenceSize = 64;
#endif

}  // namespace ccc::detail

#endif  // CCC_DETAIL_CACHE_LINE_HPP

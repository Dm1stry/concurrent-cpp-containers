// Copyright (c) 2026 Dm1stry
// SPDX-License-Identifier: MIT
//
// Compiled with exceptions disabled (-fno-exceptions) and never run: the
// library must stay usable in such builds. Explicit instantiation compiles
// every non-template member function of a class template, so a `try` or
// `throw` hidden in any of them breaks the build.

#include <memory>

#include "ccc/wait_free/spsc_queue.hpp"

template class ccc::wait_free::spsc_queue<int>;
template class ccc::wait_free::spsc_queue<std::unique_ptr<int>>;

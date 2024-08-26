//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Copyright 2018 Ulf Adams
// Copyright (c) Microsoft Corporation. All rights reserved.

// Boost Software License - Version 1.0 - August 17th, 2003

// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:

// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _LIBCPP_SRC_INCLUDE_RYU_COMMON_H
#define _LIBCPP_SRC_INCLUDE_RYU_COMMON_H

// Avoid formatting to keep the changes with the original code minimal.
// clang-format off

#include <__assert>
#include "__config"

_LIBCPP_BEGIN_NAMESPACE_STD

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline uint32_t __decimalLength9(const uint32_t __v) {
  // Function precondition: __v is not a 10-digit number.
  // (f2s: 9 digits are sufficient for round-tripping.)
  // (d2fixed: We print 9-digit blocks.)
  _LIBCPP_ASSERT(__v < 1000000000, "");
  if (__v >= 100000000) { return 9; }
  if (__v >= 10000000) { return 8; }
  if (__v >= 1000000) { return 7; }
  if (__v >= 100000) { return 6; }
  if (__v >= 10000) { return 5; }
  if (__v >= 1000) { return 4; }
  if (__v >= 100) { return 3; }
  if (__v >= 10) { return 2; }
  return 1;
}

// Returns __e == 0 ? 1 : ceil(log_2(5^__e)).
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI  inline int32_t __pow5bits(const int32_t __e) {
  // This approximation works up to the point that the multiplication overflows at __e = 3529.
  // If the multiplication were done in 64 bits, it would fail at 5^4004 which is just greater
  // than 2^9297.
  _LIBCPP_ASSERT(__e >= 0, "");
  _LIBCPP_ASSERT(__e <= 3528, "");
  return static_cast<int32_t>(((static_cast<uint32_t>(__e) * 1217359) >> 19) + 1);
}

// Returns floor(log_10(2^__e)).
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI  inline uint32_t __log10Pow2(const int32_t __e) {
  // The first value this approximation fails for is 2^1651 which is just greater than 10^297.
  _LIBCPP_ASSERT(__e >= 0, "");
  _LIBCPP_ASSERT(__e <= 1650, "");
  return (static_cast<uint32_t>(__e) * 78913) >> 18;
}

// Returns floor(log_10(5^__e)).
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline uint32_t __log10Pow5(const int32_t __e) {
  // The first value this approximation fails for is 5^2621 which is just greater than 10^1832.
  _LIBCPP_ASSERT(__e >= 0, "");
  _LIBCPP_ASSERT(__e <= 2620, "");
  return (static_cast<uint32_t>(__e) * 732923) >> 20;
}

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline uint32_t __float_to_bits(const float __f) {
  uint32_t __bits = 0;
  std::memcpy(&__bits, &__f, sizeof(float));
  return __bits;
}

[[nodiscard]] _LIBCPP_HIDE_FROM_ABI inline uint64_t __double_to_bits(const double __d) {
  uint64_t __bits = 0;
  std::memcpy(&__bits, &__d, sizeof(double));
  return __bits;
}

_LIBCPP_END_NAMESPACE_STD

// clang-format on

#endif // _LIBCPP_SRC_INCLUDE_RYU_COMMON_H
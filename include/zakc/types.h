// File:        types.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     20 Jun 2021
// Version:     0.1.0
// SPDX-License-Identifier: MIT

#pragma once

#include <stddef.h> // for ptrdiff_t, size_t
#include <stdint.h> // for {,u}int{8.16,32,64}_t

// clang-format off
typedef   int8_t   i8;
typedef  uint8_t   u8;
typedef  int16_t  i16;
typedef uint16_t  u16;
typedef  int32_t  i32;
typedef uint32_t  u32;
typedef  int64_t  i64;
typedef uint64_t  u64;

typedef ptrdiff_t  isize;
typedef    size_t  usize;

typedef float         f32;
typedef double        f64;
typedef long double  f128;

typedef char *str;
// clang-format on

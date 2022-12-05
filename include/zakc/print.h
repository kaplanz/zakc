// File:        print.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     26 Aug 2021
// SPDX-License-Identifier: MIT

#pragma once

#include <stdio.h> // for fprintf, stderr, stdout

// clang-format off
#define  print(fmt, ...)   fprintf(stdout, fmt, ##__VA_ARGS__)
#define eprint(fmt, ...)   fprintf(stderr, fmt, ##__VA_ARGS__)
#define  println(fmt, ...)   print(fmt "\n", ##__VA_ARGS__)
#define eprintln(fmt, ...)  eprint(fmt "\n", ##__VA_ARGS__)
// clang-format on

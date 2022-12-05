// File:        panic.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     26 Aug 2021
// SPDX-License-Identifier: MIT

#pragma once

#include <err.h> // for errx

#define panic(msg)                      \
    errx(                               \
        101,                            \
        "panicked at '%s', %s:%u(%s)",  \
        (msg) ? msg : "explicit panic", \
        __FILE__,                       \
        __LINE__,                       \
        __func__                        \
    )

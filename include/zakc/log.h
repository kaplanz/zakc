// File:        log.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     26 Aug 2021
// SPDX-License-Identifier: MIT

#pragma once

#include "zakc/color.h" // for __ansi_*, __ecolorize

enum loglevel {
    Off = 0,
    Error,
    Warn,
    Info,
    Debug,
    Trace,
};

extern enum loglevel loglevel;

#if defined(__APPLE__) || !__has_include(<err.h>)
#include "zakc/print.h" // for eprintln
#define __log(...) eprintln(__VA_ARGS__)
#elif __has_include(<err.h>)
#include <err.h> // for warnx
#define __log(...) warnx(__VA_ARGS__)
#else
#error "could not define macro `__log(...)`"
#endif

// clang-format off
#define __style_reset __ecolorize(__ansi_reset)
#define __style_error __ecolorize(__ansi_reset __ansi_bold   __ansi_red)
#define __style_warn  __ecolorize(__ansi_reset __ansi_bold   __ansi_yellow)
#define __style_info  __ecolorize(__ansi_reset               __ansi_green)
#define __style_debug __ecolorize(__ansi_reset __ansi_italic __ansi_blue)
#define __style_trace __ecolorize(__ansi_reset __ansi_italic __ansi_cyan)
// clang-format on

#define error(fmt, ...)    \
    if (loglevel >= Error) \
    __log("[%serror%s] " fmt, __style_error, __style_reset, ##__VA_ARGS__)
#define warn(fmt, ...)    \
    if (loglevel >= Warn) \
    __log("[%swarn%s] " fmt, __style_warn, __style_reset, ##__VA_ARGS__)
#define info(fmt, ...)    \
    if (loglevel >= Info) \
    __log("[%sinfo%s] " fmt, __style_info, __style_reset, ##__VA_ARGS__)
#define debug(fmt, ...)    \
    if (loglevel >= Debug) \
    __log("[%sdebug%s] " fmt, __style_debug, __style_reset, ##__VA_ARGS__)
#define trace(fmt, ...)    \
    if (loglevel >= Trace) \
    __log("[%strace%s] " fmt, __style_trace, __style_reset, ##__VA_ARGS__)

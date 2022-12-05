// File:        color.h
// Author:      Zakhary Kaplan <https://zakhary.dev>
// Created:     26 Aug 2021
// SPDX-License-Identifier: MIT

#pragma once

#include <stdio.h>  // for fileno, stderr, stdout
#include <unistd.h> // for isatty

#define __colorize(style)  (isatty(fileno(stdout)) ? style : "")
#define __ecolorize(style) (isatty(fileno(stderr)) ? style : "")

#define __ansi_reset     "\033[0m"
#define __ansi_bold      "\033[1m"
#define __ansi_light     "\033[2m"
#define __ansi_italic    "\033[3m"
#define __ansi_underline "\033[4m"
#define __ansi_red       "\033[31m"
#define __ansi_green     "\033[32m"
#define __ansi_yellow    "\033[33m"
#define __ansi_blue      "\033[34m"
#define __ansi_magenta   "\033[34m"
#define __ansi_cyan      "\033[36m"

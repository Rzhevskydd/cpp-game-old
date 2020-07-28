#pragma once

#ifndef NDEBUG

#include <iostream>

template <typename... Args>
void
brd_debug_print(const Args&... args) {
        static_assert(sizeof...(args) > 0, "Useless print call");
        (std::cout << ... << args);
}

#define BRD_DEBUG_PRINT(...) brd_debug_print(__VA_ARGS__)

#else
#define BRD_DEBUG_PRINT(...)
#endif
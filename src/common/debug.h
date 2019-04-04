#pragma once

#include <cstdarg>
#include <iostream>

#ifdef __PIPER_DEBUG__
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 0
#endif
#ifdef __aarch64__
#include "opensslconf-arm64.h"
#elif __arm__
#include "opensslconf-arm32.h"
#elif __i386__
#include "opensslconf-x86.h"
#else
#error "Unsupported architecture!"
#endif

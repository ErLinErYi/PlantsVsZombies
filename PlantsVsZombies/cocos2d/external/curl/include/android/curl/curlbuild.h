#ifdef __aarch64__
#include "curlbuild-arm64.h"
#elif __arm__
#include "curlbuild-arm32.h"
#elif __i386__
#include "curlbuild-x86.h"
#else
#error "Unsupported architecture!"
#endif

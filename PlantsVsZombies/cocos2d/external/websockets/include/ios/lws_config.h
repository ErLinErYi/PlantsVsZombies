#ifdef __arm64__
#include "lws_config-arm64.h"
#elif __arm__
#include "lws_config-arm32.h"
#elif __i386__
#include "lws_config-i386.h"
#elif __x86_64__
#include "lws_config-x86_64.h"
#else
#error "Unsupported architecture!"
#endif

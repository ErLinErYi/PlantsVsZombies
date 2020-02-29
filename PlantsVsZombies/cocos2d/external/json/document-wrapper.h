#pragma once

// Wrapper file for document.h to fix the crash bug (https://github.com/cocos2d/cocos2d-x/issues/16492)

// Bug in gcc 4.9.0 on ARM with ARM instruction set, -O2 causes the bug
// This gcc version cannot use #pragma GCC push_options/pop_options as well. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=59884

#include "json/rapidjson.h"

#if defined(__arm__) && defined(RAPIDJSON_GNUC) && RAPIDJSON_GNUC >= RAPIDJSON_VERSION_CODE(4,9,0) && RAPIDJSON_GNUC < RAPIDJSON_VERSION_CODE(5,0,0)
    #pragma GCC optimize ("O1")
#endif    
#include "document.h"
#if defined(__arm__) && defined(RAPIDJSON_GNUC) && RAPIDJSON_GNUC >= RAPIDJSON_VERSION_CODE(4,9,0) && RAPIDJSON_GNUC < RAPIDJSON_VERSION_CODE(5,0,0)
    #pragma GCC reset_options
#endif

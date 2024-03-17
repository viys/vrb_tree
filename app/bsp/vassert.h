
#pragma once

#include <stdio.h>

#define assert_printf printf

#ifndef assert_printf

#error need define assert_printf

#else

#ifndef NDEBUG

#if __STDC_VERSION__ < 199901L && !defined(__func__)
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

# define vassert(condition, format, ...) do {                           \
        if(!(condition)) {                                              \
            assert_printf("WARNING: %s:%d: %s:"                         \
                    " Assertion \"%s\" failed.\r\n",                    \
                    __FILE__, __LINE__, __func__, #condition);          \
        }else{                                                          \
            assert_printf(format, ##__VA_ARGS__);                       \
        }                                                               \
    } while(0)

#else

#define vassert(condition, format, ...) do {}while(0)

#endif

#endif
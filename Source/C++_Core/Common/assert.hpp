#pragma once

#if defined(RUNTIME_ASSERTS_DISABLED)
    #define RUNTIME_ASSERT(expression)
    #define RUNTIME_ASSERT_MSG(expression)
#else
    void MACRO_RUNTIME_ASSERT(bool expression, const char* function, const char* file, int line);
    void MACRO_RUNTIME_ASSERT_MSG(bool expression, const char* function, const char* file, int line, const char* fmt, ...);
    #define RUNTIME_ASSERT(expression) MACRO_RUNTIME_ASSERT((expression), __func__, __FILE__, __LINE__)
    #define RUNTIME_ASSERT_MSG(expression, message, ...) MACRO_RUNTIME_ASSERT_MSG((expression), __func__, __FILE__, __LINE__, message, ##__VA_ARGS__)
#endif

#define STATIC_ASSERT static_assert



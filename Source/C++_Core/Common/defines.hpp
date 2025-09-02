#pragma once

constexpr float PI = 3.14159265359f;
constexpr float TAU = PI * 2;
constexpr float EPSILON = 0.0001f;

#define global static
#define local_persist static
#define internal static

#define stringify(entry) #entry
#define glue(a, b) a##b

#define KB(value) ((byte_t)(value) * 1024L)
#define MB(value) ((byte_t)KB(value) * 1024L)
#define GB(value) ((byte_t)MB(value) * 1024L)
#define OFFSET_OF(type, member) (byte_t)(&(((type*)0)->member))
#define FIRST_DIGIT(number) ((int)number % 10);
#define GET_BIT(number, bit_to_check) ((number & (1 << bit_to_check)) >> bit_to_check)
#define SET_BIT(number, bit_to_set) number |= (1 << bit_to_set);
#define UNSET_BIT(number, bit_to_unset) number &= (~(1 << bit_to_unset));

#define NEAR_ZERO(x) (fabs(x) <= EPSILON)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(value, min_value, max_value) (MIN(MAX(value, min_value), max_value))
#define SQUARED(a) ((a) * (a))
#define DEGREES_TO_RAD(degrees) ((degrees)*(PI/180))
#define RAD_TO_DEGREES(rad) ((rad)*(180/PI))

#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))

#if defined(_WIN32)
    #define PLATFORM_WINDOWS
    #define CRASH() __debugbreak()
#elif defined(__APPLE__)
    #define PLATFORM_APPLE
    #define CRASH() __builtin_trap()
#elif (defined(__linux__) || defined(__unix__) || defined(__POSIX__))
    #define PLATFORM_LINUX
    #define CRASH() __builtin_trap()
#else
    #error "Unknown Platform???"
#endif

#if defined(_MSC_VER)
    #define UNUSED_FUNCTION
#elif defined(__clang__)
    #define UNUSED_FUNCTION __attribute__((used))
#elif defined(__GNUC__) || defined(__GNUG__)
    #define UNUSED_FUNCTION __attribute__((used))
#endif

#include <stdarg.h>
#include <stdio.h>

#include "common.hpp"
#include "types.hpp"
#include "assert.hpp"

//Regular text
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_PURPLE "\033[0;35m"

//Regular background
#define RED_BACKGROUND "\033[41m"
#define COLOR_RESET "\033[0m"

internal byte_t va_sprint(char* buffer, byte_t buffer_size, const char* fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    byte_t out_size = (byte_t)vsnprintf(nullptr, 0, fmt, args_copy) + 1; // +1 for null terminator
    RUNTIME_ASSERT(out_size < buffer_size);
    va_end(args_copy);

    va_copy(args_copy, args);
    vsnprintf(buffer, out_size, fmt, args_copy);
    va_end(args_copy); 
    
    return out_size;
}

void log_output(LogLevel log_level, const char* message, ...) {
    #define OUT_MESSAGE_BUFFER_LENGTH 2048
    char out_message[OUT_MESSAGE_BUFFER_LENGTH] = {0};

    va_list args_list;
    va_start(args_list, message);
    byte_t out_message_size = va_sprint(out_message, OUT_MESSAGE_BUFFER_LENGTH, message, args_list); // with null term
    u64 out_message_length = out_message_size - 1;
    va_end(args_list);

    const char* log_level_format[LOG_LEVEL_COUNT] = {
        RED_BACKGROUND,
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_RESET
    };

    bool found = out_message[out_message_length - 1] == '\n';
    printf("%s%.*s%s", log_level_format[log_level], (int)(out_message_length - found), out_message, COLOR_RESET);
    if (found) {
        printf("\n");
    }
}
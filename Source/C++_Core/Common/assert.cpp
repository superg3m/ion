#include <stdio.h>
#include <stdarg.h>

#include "types.hpp"
#include "assert.hpp"
#include "logger.hpp"

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


void MACRO_RUNTIME_ASSERT(bool expression, const char* function, const char* file, int line) {
    if (!expression) {                             
        //stack_trace_dump(function, file, line);                               
        char msg[] = "Func: %s, File: %s:%d\n";          
        LOG_FATAL(msg, function, file, line);
        CRASH();
    }                                                         
}

void MACRO_RUNTIME_ASSERT_MSG(bool expression, const char* function, const char* file, int line, const char* msg, ...) {  
    va_list args;
    va_start(args, msg);

    if (!(expression)) {            
        #define BUFFER_LENGTH 128
        char message[BUFFER_LENGTH] = {0};
        byte_t message_size = va_sprint(message, BUFFER_LENGTH, msg, args);
        u64 message_length = message_size - 1;

        // stack_trace_dump(function, file, line);
        LOG_FATAL("%.*s", message_length, message);                                 
        char msg[] = "Func: %s, File: %s:%d\n";          
        LOG_FATAL(msg, function, file, line);
        CRASH();                                                
    }   

    va_end(args);                                                                                             
}
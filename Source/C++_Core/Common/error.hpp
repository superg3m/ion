#pragma once
enum ION_Error {
    ION_ERROR_SUCCESS = 0,
    ION_ERROR_RESOURCE_NOT_FOUND,
    ION_ERROR_RESOURCE_TOO_BIG,
    ION_ERROR_NULL_PARAMETER,
    ION_ERROR_INVALID_PARAMETER,
    ION_ERROR_COUNT
};

/**
 * @brief returns a string literal of the error code
 * 
 * @param error_code 
 * @return const char*
 */
const char* ion_error_str(ION_Error error_code);
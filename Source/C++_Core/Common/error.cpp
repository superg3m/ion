#include "defines.hpp"
#include "error.hpp"
#include "assert.hpp"

internal const char* ION_ERROR_STRINGS[ION_ERROR_COUNT] = {
    stringify(ION_ERROR_SUCCESS),
    stringify(ION_ERROR_RESOURCE_NOT_FOUND),
    stringify(ION_ERROR_RESOURCE_TOO_BIG),
    stringify(ION_ERROR_NULL_PARAMETER),
    stringify(ION_ERROR_INVALID_PARAMETER),
};

const char* ion_error_str(ION_Error error_code) {
    RUNTIME_ASSERT((error_code >= 0) && (error_code < ION_ERROR_COUNT));

    return ION_ERROR_STRINGS[error_code];
}
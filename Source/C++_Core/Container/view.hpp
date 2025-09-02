#pragma once

#include <initializer_list>
#include <type_traits>

#include "../Memory/memory.hpp"
#include "../Common/common.hpp"

namespace ION {
    template <typename T>
    struct View {
        const T* const data;
        u64 length;

        View(const T* const data, u64 length): data(data), length(length) {}
    };
}
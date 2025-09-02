#pragma once

#include "../Common/common.hpp"

namespace ION::Random {
    constexpr u32 STATE_VECTOR_LENGTH  = 624;
    constexpr u32 STATE_VECTOR_M = 397;

    struct Generator {
        u32 mt[STATE_VECTOR_LENGTH];
        s32 index;

		Generator(u32 seed = 1);                   // seed can't be zero
        u32 U32();                                 // 0 to UINT32_MAX
        float F32();                               // 0.0 to FLT_MAX
        double F64();                              // 0.0 to DBL_MAX
        u32 BetweenU32(u32 min, u32 max);          // inclusive min/max
        double BetweenF64(double min, double max); // min/max range
    };
}
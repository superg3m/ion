#include "random.hpp"
#include <float.h>

namespace ION::Random {
    constexpr u32 UPPER_MASK       = 0x80000000;
    constexpr u32 LOWER_MASK       = 0x7fffffff;
    constexpr u32 TEMPERING_MASK_B = 0x9d2c5680;
    constexpr u32 TEMPERING_MASK_C = 0xefc60000;

    internal void _SeedState(Generator* g, u32 seed) {
        g->mt[0] = seed & 0xffffffffU;
        for (g->index = 1; g->index < STATE_VECTOR_LENGTH; g->index++) {
            g->mt[g->index] = (6069U * g->mt[g->index - 1]) & 0xffffffffU;
        }
    }

    Generator::Generator(u32 seed) {
        _SeedState(this, seed);
    }

    u32 Generator::U32() {
        u32 y;
        static u32 mag[2] = { 0x0U, 0x9908b0dfU };

        if (index >= STATE_VECTOR_LENGTH || index < 0) {
            if (index >= STATE_VECTOR_LENGTH + 1 || index < 0) {
                _SeedState(this, 4357);
            }

            for (s32 kk = 0; kk < STATE_VECTOR_LENGTH - STATE_VECTOR_M; kk++) {
                y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                mt[kk] = mt[kk + STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1U];
            }

            for (s32 kk = STATE_VECTOR_LENGTH - STATE_VECTOR_M; kk < STATE_VECTOR_LENGTH - 1; kk++) {
                y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                mt[kk] = mt[kk + (STATE_VECTOR_M - STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1U];
            }

            y = (mt[STATE_VECTOR_LENGTH - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
            mt[STATE_VECTOR_LENGTH - 1] = mt[STATE_VECTOR_M - 1] ^ (y >> 1) ^ mag[y & 0x1U];

            index = 0;
        }

        y = mt[index++];
        y ^= (y >> 11);
        y ^= (y << 7)  & TEMPERING_MASK_B;
        y ^= (y << 15) & TEMPERING_MASK_C;
        y ^= (y >> 18);

        return y;
    }

    float Generator::F32() {
        return static_cast<float>(U32()) / static_cast<float>(0xffffffffU) * FLT_MAX;
    }

    double Generator::F64() {
        return static_cast<double>(U32()) / static_cast<double>(0xffffffffU) * DBL_MAX;
    }

    u32 Generator::BetweenU32(u32 min, u32 max) {
        if (min == max) {
            return min;
        }

        return min + (U32() % (max - min + 1));
    }

    double Generator::BetweenF64(double min, double max) {
        if (min == max) {
            return min;
        }
        
        return min + (static_cast<double>(U32()) / static_cast<double>(0xffffffffU)) * (max - min);
    }

}
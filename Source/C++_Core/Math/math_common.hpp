#pragma once

#include <math.h>
#include "../Common/defines.hpp"

namespace ION::Math {
    float lerp(float a, float b, float t);
    float inverse_lerp(float a, float b, float value);
    float remap(float x, float s_min, float s_max, float e_min, float e_max);
    float move_toward(float current, float target, float delta);
}

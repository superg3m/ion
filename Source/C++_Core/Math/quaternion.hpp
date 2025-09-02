#pragma once

#include "vector.hpp"

namespace ION::Math {
    typedef struct Mat4 Mat4;

    struct Quat {
        float w = 1;
        Vec3  v = Vec3(0);

        Quat();
        Quat(float theta, Vec3 axis);
        Quat(float theta, float x, float y, float z);

        Quat inverse() const;
        Quat normalize() const;
        Quat scale(float scale) const;
        Mat4 to_rotation_matrix() const;
        void to_angle_axis(float &theta, Vec3 &vec) const;

        static Quat identity();
        static Quat literal(float w, Vec3 axis);
        static Quat literal(float w, float x, float y, float z);
        static Quat from_euler(Vec3 euler_angles_degrees);
        static Quat from_angle_axis(float w, Vec3 axis);
        static Quat from_rotation_matrix(Mat4 mat);
        static Quat slerp(Quat q, Quat r, float t);
        static float dot(Quat a, Quat b);

        Quat operator+(const Quat &right);
        Quat& operator+=(const Quat &right);
        
        Quat operator-(const Quat &right);
        Quat& operator-=(const Quat &right);

        Quat operator*(const Quat &right);
        Quat& operator*=(const Quat &right);

        Vec3 operator*(const Vec3 &right);

        bool operator==(const Quat &right);
        bool operator!=(const Quat &right);
    };
}
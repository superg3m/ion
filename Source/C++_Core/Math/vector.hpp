#pragma once

#include "math_common.hpp"

namespace ION::Math {
    typedef struct Vec3 Vec3;
    typedef struct Vec4 Vec4;

    struct Vec2 {
        float x;
        float y;

        Vec2();
        explicit Vec2(float fill);
        explicit Vec2(float x, float y);
        explicit Vec2(Vec3 v);

        float magnitude() const;
        float magnitude_squared() const;
        Vec2 normalize() const;
        Vec2 scale(float scale) const;
        Vec2 scale(float scale_x, float scale_y) const;
        Vec2 scale(Vec2 s) const;

        /**
         * @brief The return value tells you:
         * -1: the vectors are 180 degrees from eachother in other words they vectors are pointing in opposite directions
         *  0: the vectors are perpendicular or orthogonal to eachother
         *  1: the vectors are going the same direction
         * 
         * @param a 
         * @param b 
         * @return float 
         */
        static float dot(Vec2 a, Vec2 b);
        static float distance(Vec2 a, Vec2 b);
        static float distance_squared(Vec2 a, Vec2 b);
        static Vec2 lerp(Vec2 a, Vec2 b, float t);
        static Vec2 from_euler(float yaw, float pitch);

        Vec2 operator+(const Vec2 &right);
        Vec2& operator+=(const Vec2 &right);

        Vec2 operator-(const Vec2 &right);
        Vec2& operator-=(const Vec2 &right);

        Vec2 operator*(const Vec2 &right);
        Vec2& operator*=(const Vec2 &right);

        Vec2 operator/(const Vec2 &right);
        Vec2& operator/=(const Vec2 &right);

        bool operator==(const Vec2 &right);
        bool operator!=(const Vec2 &right);
    };

    struct Vec3 {
        union {
            struct {
                float x;
                float y;
                float z;
            };

            struct {
                float r;
                float g;
                float b;
            };
        };

        Vec3();
        explicit Vec3(float fill);
        explicit Vec3(float x, float y, float z);
        explicit Vec3(Vec2 v, float z);
        explicit Vec3(Vec4 v);

        float magnitude() const;
        float magnitude_squared() const;
        Vec3 normalize() const;
        Vec3 scale(float scale) const;
        Vec3 scale(float scale_x, float scale_y, float scale_z) const;
        Vec3 scale(Vec3 s) const;

        /**
         * @brief The return value tells you:
         * -1: the vectors are 180 degrees from eachother in other words they vectors are pointing in opposite directions
         *  0: the vectors are perpendicular or orthogonal to eachother
         *  1: the vectors are going the same direction
         * 
         * @param a 
         * @param b 
         * @return float 
         */
        static float dot(Vec3 a, Vec3 b);
        static float distance(Vec3 a, Vec3 b);
        static float distance_squared(Vec3 a, Vec3 b);
        static Vec3 lerp(Vec3 a, Vec3 b, float t);
        static Vec3 cross(Vec3 a, Vec3 b);
        static Vec3 euler(float yaw, float pitch);

        Vec3 operator+(const Vec3 &right);
        Vec3& operator+=(const Vec3 &right);

        Vec3 operator-(const Vec3 &right);
        Vec3& operator-=(const Vec3 &right);

        Vec3 operator*(const Vec3 &right);
        Vec3& operator*=(const Vec3 &right);

        Vec3 operator/(const Vec3 &right);
        Vec3& operator/=(const Vec3 &right);

        bool operator==(const Vec3 &right);
        bool operator!=(const Vec3 &right);
    };

    struct Vec4 {
        union {
            struct {
                float x;
                float y;
                float z;
                float w;
            };

            struct {
                float r;
                float g;
                float b;
                float a;
            };
        };

        Vec4();
        explicit Vec4(float fill);
        explicit Vec4(float x, float y, float z, float w);
        explicit Vec4(Vec3 v, float w);

        float magnitude() const;
        float magnitude_squared() const;
        Vec4 normalize() const;
        Vec4 scale(float scale) const;
        Vec4 scale(float scale_x, float scale_y, float scale_z, float scale_w) const;
        Vec4 scale(Vec4 s) const;

        /**
         * @brief The return value tells you:
         * -1: the vectors are 180 degrees from eachother in other words they vectors are pointing in opposite directions
         *  0: the vectors are perpendicular or orthogonal to eachother
         *  1: the vectors are going the same direction
         * 
         * @param a 
         * @param b 
         * @return float 
         */
        static float dot(Vec4 a, Vec4 b);
        static Vec4 lerp(Vec4 a, Vec4 b, float t);
        static float distance(Vec4 a, Vec4 b);
        static float distance_squared(Vec4 a, Vec4 b);

        Vec4 operator+(const Vec4 &right);
        Vec4& operator+=(const Vec4 &right);

        Vec4 operator-(const Vec4 &right);
        Vec4& operator-=(const Vec4 &right);

        Vec4 operator*(const Vec4 &right);
        Vec4& operator*=(const Vec4 &right);

        Vec4 operator/(const Vec4 &right);
        Vec4& operator/=(const Vec4 &right);

        bool operator==(const Vec4 &right);
        bool operator!=(const Vec4 &right);
    };
}
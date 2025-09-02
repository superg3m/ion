#include "vector.hpp"
#include "math_common.hpp"

namespace ION::Math {
    Vec2::Vec2() {
        this->x = 0.0f;
        this->y = 0.0f;
    }

    Vec2::Vec2(float fill) {
        this->x = fill;
        this->y = fill;
    }

    Vec2::Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vec2::Vec2(Vec3 v) {
        this->x = v.x;
        this->y = v.y;
    }

    float Vec2::magnitude() const {
        return sqrtf((this->x * this->x) + (this->y * this->y));
    }

    float Vec2::magnitude_squared() const {
        return (this->x * this->x) + (this->y * this->y);
    }

    Vec2 Vec2::normalize() const {
        Vec2 ret(0, 0);
        const float magnitude = this->magnitude();
        if (magnitude == 0) {
            return Vec2(0,0);
        }

        ret.x = this->x / magnitude;
        ret.y = this->y / magnitude;

        return ret;
    }

    Vec2 Vec2::scale(float scale) const {
        return Vec2(this->x * scale, this->y * scale);
    }

    Vec2 Vec2::scale(Vec2 s) const {
        return Vec2(this->x * s.x, this->y * s.y);
    }

    Vec2 Vec2::scale(float scale_x, float scale_y) const {
        return Vec2(this->x * scale_x, this->y * scale_y);
    }

    float Vec2::dot(Vec2 a, Vec2 b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    float Vec2::distance(Vec2 a, Vec2 b) {
        return sqrtf(SQUARED(b.x - a.x) + SQUARED(b.y - a.y));
    }

    float Vec2::distance_squared(Vec2 a, Vec2 b) {
        return SQUARED(b.x - a.x) + SQUARED(b.y - a.y);
    }

    Vec2 Vec2::lerp(Vec2 a, Vec2 b, float t) {
        Vec2 ab = (b - a);
        return a + (ab.scale(t));
    }

    Vec2 Vec2::from_euler(float yaw, float pitch) {
        Vec2 ret = Vec2(0, 0);
        ret.x = cosf(DEGREES_TO_RAD(yaw)) * cosf(DEGREES_TO_RAD(pitch));
        ret.y = sinf(DEGREES_TO_RAD(pitch));

        return ret;
    }

    Vec2 Vec2::operator+(const Vec2 &right) {
        return Vec2(this->x + right.x, this->y + right.y);
    }
    Vec2& Vec2::operator+=(const Vec2 &right) {
        this->x += right.x;
        this->y += right.y;

        return *this;
    }

    Vec2 Vec2::operator-(const Vec2 &right) {
        return Vec2(this->x - right.x, this->y - right.y);
    }
    Vec2& Vec2::operator-=(const Vec2 &right) {
        this->x -= right.x;
        this->y -= right.y;

        return *this;
    }

    Vec2 Vec2::operator*(const Vec2 &right) {
        return Vec2(this->x * right.x, this->y * right.y);
    }

    Vec2& Vec2::operator*=(const Vec2 &right) {
        this->x *= right.x;
        this->y *= right.y;

        return *this;
    }

    Vec2 Vec2::operator/(const Vec2 &right) {
        return Vec2(this->x / right.x, this->y / right.y);
    }
    Vec2& Vec2::operator/=(const Vec2 &right) {
        this->x /= right.x;
        this->y /= right.y;

        return *this;
    }

    bool Vec2::operator==(const Vec2 &right) {
        return NEAR_ZERO(this->x - right.x) && NEAR_ZERO(this->y - right.y);
    }

    bool Vec2::operator!=(const Vec2 &right) {
        return !(*this == right);
    }

    Vec3::Vec3() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Vec3::Vec3(float fill) {
        this->x = fill;
        this->y = fill;
        this->z = fill;
    }

    Vec3::Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3::Vec3(Vec2 v, float z) {
        this->x = v.x;
        this->y = v.y;
        this->z = z;
    }

    Vec3::Vec3(Vec4 v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    float Vec3::magnitude() const {
        return sqrtf(SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z));
    }

    float Vec3::magnitude_squared() const {
        return SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z);
    }

    Vec3 Vec3::normalize() const {
        Vec3 ret(0, 0, 0);
        const float magnitude = this->magnitude();
        if (magnitude == 0) {
            return Vec3(0, 0, 0);
        }

        ret.x = this->x / magnitude;
        ret.y = this->y / magnitude;
        ret.z = this->z / magnitude;

        return ret;
    }

    Vec3 Vec3::scale(float scale) const {
        return Vec3(this->x * scale, this->y * scale, this->z * scale);
    }

    Vec3 Vec3::scale(float scale_x, float scale_y, float scale_z) const {
        return Vec3(this->x * scale_x, this->y * scale_y, this->z * scale_z);
    }

    Vec3 Vec3::scale(Vec3 s) const {
        return Vec3(this->x * s.x, this->y * s.y, this->z * s.z);
    }

    float Vec3::distance(Vec3 a, Vec3 b) {
        return sqrtf(SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z));
    }

    float Vec3::distance_squared(Vec3 a, Vec3 b) {
        return SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z);
    }

    float Vec3::dot(Vec3 a, Vec3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    Vec3 Vec3::lerp(Vec3 a, Vec3 b, float t) {
        Vec3 ab = (b - a);
        return a + (ab.scale(t));
    }

    Vec3 Vec3::cross(Vec3 A, Vec3 B) {
        Vec3 ret(0, 0, 0);
        ret.x = A.y * B.z - A.z * B.y;
        ret.y = A.z * B.x - A.x * B.z;
        ret.z = A.x * B.y - A.y * B.x;

        return ret;
    }

    Vec3 Vec3::euler(float yaw, float pitch) {
        Vec3 ret = Vec3(0, 0, 0);
        ret.x = cosf(DEGREES_TO_RAD(yaw)) * cosf(DEGREES_TO_RAD(pitch));
        ret.y = sinf(DEGREES_TO_RAD(pitch));
        ret.z = sinf(DEGREES_TO_RAD(yaw)) * cosf(DEGREES_TO_RAD(pitch));

        return ret;
    }

    Vec3 Vec3::operator+(const Vec3 &right) {
        return Vec3(this->x + right.x, this->y + right.y, this->z + right.z);
    }
    Vec3& Vec3::operator+=(const Vec3 &right) {
        this->x += right.x;
        this->y += right.y;
        this->z += right.z;

        return *this;
    }

    Vec3 Vec3::operator-(const Vec3 &right) {
        return Vec3(this->x - right.x, this->y - right.y, this->z - right.z);
    }
    Vec3& Vec3::operator-=(const Vec3 &right) {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z;

        return *this;
    }


    Vec3 Vec3::operator*(const Vec3 &right) {
        return Vec3(this->x * right.x, this->y * right.y, this->z * right.z);
    }
    Vec3& Vec3::operator*=(const Vec3 &right) {
        this->x *= right.x;
        this->y *= right.y;
        this->z *= right.z;

        return *this;
    }


    Vec3 Vec3::operator/(const Vec3 &right) {
        return Vec3(this->x / right.x, this->y / right.y, this->z / right.z);
    }
    Vec3& Vec3::operator/=(const Vec3 &right) {
        this->x /= right.x;
        this->y /= right.y;
        this->z /= right.z;

        return *this;
    }

    bool Vec3::operator==(const Vec3 &right) {
        return NEAR_ZERO(this->x - right.x) && NEAR_ZERO(this->y - right.y) && NEAR_ZERO(this->z - right.z);
    }
    bool Vec3::operator!=(const Vec3 &right) {
        return !(*this == right);
    }

    Vec4::Vec4() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }


    Vec4::Vec4(float fill) {
        this->x = fill;
        this->y = fill;
        this->z = fill;
        this->w = fill;
    }

    Vec4::Vec4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vec4::Vec4(Vec3 v, float w) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    float Vec4::magnitude() const {
        return sqrtf(SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z) + SQUARED(this->w));
    }

    float Vec4::magnitude_squared() const {
        return SQUARED(this->x) + SQUARED(this->y) + SQUARED(this->z) + SQUARED(this->w);
    }

    Vec4 Vec4::normalize() const {
        Vec4 ret(0, 0, 0, 0);
        const float magnitude = this->magnitude();
        if (magnitude == 0) {
            return Vec4(0, 0, 0, 0);
        }

        ret.x = this->x / magnitude;
        ret.y = this->y / magnitude;
        ret.z = this->z / magnitude;
        ret.w = this->w / magnitude;

        return ret;
    }

    Vec4 Vec4::scale(float scale) const {
        return Vec4(this->x * scale, this->y * scale, this->z * scale, this->w * scale);
    }

    Vec4 Vec4::scale(float scale_x, float scale_y, float scale_z, float scale_w) const {
        return Vec4(this->x * scale_x, this->y * scale_y, this->z * scale_z, this->w * scale_w);
    }

    Vec4 Vec4::scale(Vec4 s) const {
        return Vec4(this->x * s.x, this->y * s.y, this->z * s.z, this->w * s.w);
    }

    float Vec4::distance(Vec4 a, Vec4 b) {
        return sqrtf(SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z) + SQUARED(b.w - a.w));
    }

    float Vec4::distance_squared(Vec4 a, Vec4 b) {
        return SQUARED(b.x - a.x) + SQUARED(b.y - a.y) + SQUARED(b.z - a.z) + SQUARED(b.w - a.w);
    }

    float Vec4::dot(Vec4 a, Vec4 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
    }

    Vec4 Vec4::lerp(Vec4 a, Vec4 b, float t) {
        Vec4 ab = (b - a);
        return a + (ab.scale(t));
    }


    Vec4 Vec4::operator+(const Vec4 &right) {
        return Vec4(this->x + right.x, this->y + right.y, this->z + right.z, this->w + right.w);
    }
    Vec4& Vec4::operator+=(const Vec4 &right) {
        this->x += right.x;
        this->y += right.y;
        this->z += right.z;
        this->w += right.w;

        return *this;
    }

    Vec4 Vec4::operator-(const Vec4 &right) {
        return Vec4(this->x - right.x, this->y - right.y, this->z - right.z, this->w - right.w);
    }
    Vec4& Vec4::operator-=(const Vec4 &right) {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z;
        this->w -= right.w;

        return *this;
    }


    Vec4 Vec4::operator*(const Vec4 &right) {
        return Vec4(this->x * right.x, this->y * right.y, this->z * right.z, this->w * right.w);
    }
    Vec4& Vec4::operator*=(const Vec4 &right) {
        this->x *= right.x;
        this->y *= right.y;
        this->z *= right.z;
        this->w *= right.w;

        return *this;
    }


    Vec4 Vec4::operator/(const Vec4 &right) {
        return Vec4(this->x / right.x, this->y / right.y, this->z / right.z, this->w / right.w);
    }
    Vec4& Vec4::operator/=(const Vec4 &right) {
        this->x /= right.x;
        this->y /= right.y;
        this->z /= right.z;
        this->w /= right.w;

        return *this;
    }

    bool Vec4::operator==(const Vec4 &right) {
        return NEAR_ZERO(this->x - right.x) && NEAR_ZERO(this->y - right.y) && NEAR_ZERO(this->z - right.z) && NEAR_ZERO(this->w - right.w);
    }
    bool Vec4::operator!=(const Vec4 &right) {
        return !(*this == right);
    }
}

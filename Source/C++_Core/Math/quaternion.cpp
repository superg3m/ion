#include "quaternion.hpp"
#include "matrix.hpp"

namespace ION::Math {
    Quat::Quat() {
        this->w = 1;
        this->v = Vec3(0, 0, 0);
    }

    Quat::Quat(float theta, Vec3 axis) {
        float radians = DEGREES_TO_RAD(theta);
        this->w = cosf(radians / 2.0f);
        if (NEAR_ZERO(this->w)) {
            this->w = 0.0f;
        }

        axis = axis.normalize();
        this->v = axis.scale(sinf(radians / 2.0f));
    }

    Quat::Quat(float theta, float x, float y, float z) {
        *this = Quat(theta, Vec3(x, y, z));
    }

    Quat Quat::inverse() const {
        Quat ret(1, 0, 0, 0);

        float magnitude_squared = SQUARED(this->w) + Vec3::dot(this->v, this->v);
        if (magnitude_squared == 0.0f) { 
            return Quat::identity();
        }

        ret.w = this->w / magnitude_squared;
        ret.v = this->v.scale(-1.0f / magnitude_squared);

        return ret;
    }

    Quat Quat::normalize() const {
        Vec4 temp = Vec4(this->w, this->v.x, this->v.y, this->v.z).normalize();
        
        Quat ret;
        ret.w = temp.x;
        ret.v.x = temp.y;
        ret.v.y = temp.z;
        ret.v.z = temp.w;

        return ret;
    }

    Quat Quat::scale(float scale) const {
        Quat ret;

        ret.w   = this->w   * scale;
        ret.v.x = this->v.x * scale;
        ret.v.y = this->v.y * scale;
        ret.v.z = this->v.z * scale;

        return ret;
    }

    Mat4 Quat::to_rotation_matrix() const {
        Mat4 result = Mat4::identity();

        float x2 = this->v.x * this->v.x;
        float y2 = this->v.y * this->v.y;
        float z2 = this->v.z * this->v.z;

        float xy = this->v.x * this->v.y;
        float xz = this->v.x * this->v.z;
        float yz = this->v.y * this->v.z;
        float xw = this->v.x * this->w;
        float yw = this->v.y * this->w;
        float zw = this->v.z * this->w;

        result.v[0].x = 1.0f - 2.0f * (y2 + z2);  // m00
        result.v[0].y = 2.0f * (xy - zw);         // m01
        result.v[0].z = 2.0f * (xz + yw);         // m02
        result.v[0].w = 0.0f;                     // m03

        result.v[1].x = 2.0f * (xy + zw);         // m10
        result.v[1].y = 1.0f - 2.0f * (x2 + z2);  // m11
        result.v[1].z = 2.0f * (yz - xw);         // m12
        result.v[1].w = 0.0f;                     // m13

        result.v[2].x  = 2.0f * (xz - yw);        // m20
        result.v[2].y  = 2.0f * (yz + xw);        // m21
        result.v[2].z  = 1.0f - 2.0f * (x2 + y2); // m22
        result.v[2].w  = 0.0f;                    // m23

        result.v[3].x = 0.0f;                     // m30
        result.v[3].y = 0.0f;                     // m31
        result.v[3].z = 0.0f;                     // m32
        result.v[3].w = 1.0f;                     // m33

        return result;
    }

    void Quat::to_angle_axis(float &theta, Vec3 &vec) const {
        Quat quat = this->normalize();
        float sinf_half_theta = quat.v.magnitude();

        if (sinf_half_theta < EPSILON) {
            vec = Vec3(1, 0, 0);
        } else {
            vec = quat.v.scale(1.0f / sinf_half_theta);
        }

        // Clamp w to [-1, 1] to avoid weird floating point stuff
        float w = CLAMP(quat.w, -1.0f, 1.0f);
        theta = 2.0f * acosf(w);
        theta = RAD_TO_DEGREES(theta);
    }

    Quat Quat::identity() {
        return {1, 0, 0, 0};
    }

    Quat Quat::literal(float w, Vec3 axis) {
        Quat ret;
        ret.w = w;
        ret.v = axis;

        return ret;
    }

    Quat Quat::literal(float w, float x, float y, float z) {
        Quat ret;
        ret.w = w;
        ret.v = Vec3(x, y, z);

        return ret;
    }

    Quat Quat::from_euler(Vec3 euler_angles_degrees) {
        float roll_rad_half = DEGREES_TO_RAD(euler_angles_degrees.x) * 0.5f;
        float pitch_rad_half = DEGREES_TO_RAD(euler_angles_degrees.y) * 0.5f;
        float yaw_rad_half = DEGREES_TO_RAD(euler_angles_degrees.z) * 0.5f;

        float cx = cosf(roll_rad_half);
        float sx = sinf(roll_rad_half);
        float cy = cosf(pitch_rad_half);
        float sy = sinf(pitch_rad_half);
        float cz = cosf(yaw_rad_half);
        float sz = sinf(yaw_rad_half);

        Quat q = Quat::identity();
        q.w = cx * cy * cz + sx * sy * sz;
        q.v.x = sx * cy * cz - cx * sy * sz;
        q.v.y = cx * sy * cz + sx * cy * sz;
        q.v.z = cx * cy * sz - sx * sy * cz;

        return q;
    }

    Quat Quat::from_angle_axis(float angle, Vec3 axis) {
        float half_angle = DEGREES_TO_RAD(angle) * 0.5f;
        float sinf_half = sinf(half_angle);
        float cosf_half = cosf(half_angle);

        Quat q = Quat::identity();
        axis = axis.normalize();

        q.w     = cosf_half;
        q.v.x   = axis.x * sinf_half;
        q.v.y   = axis.y * sinf_half;
        q.v.z   = axis.z * sinf_half;

        return q;
    }

    Quat Quat::from_rotation_matrix(Mat4 m) {
        Quat q;

        float m00 = m.v[0].x, m01 = m.v[0].y,  m02 = m.v[0].z;
        float m10 = m.v[1].x, m11 = m.v[1].y,  m12 = m.v[1].z;
        float m20 = m.v[2].x, m21 = m.v[2].y,  m22 = m.v[2].z;
        
        float t;
        if (m22 < 0) {
            if (m00 > m11) {
                t = 1 + m00 - m11 - m22;
                q.v.x = t;
                q.v.y = m01 + m10;
                q.v.z = m02 + m20;
                q.w = m21 - m12;
            } else {
                t = 1 - m00 + m11 - m22;
                q.v.x = m01 + m10;
                q.v.y = t;
                q.v.z = m12 + m21;
                q.w = m02 - m20;
            }
        } else {
            if (m00 < -m11) {
                t = 1 - m00 - m11 + m22;
                q.v.x = m02 + m20;
                q.v.y = m12 + m21;
                q.v.z = t;
                q.w = m10 - m01;
            } else {
                t = 1 + m00 + m11 + m22;
                q.v.x = m21 - m12;
                q.v.y = m02 - m20;
                q.v.z = m10 - m01;
                q.w = t;
            }
        }
        
        float s = 0.5f / sqrtf(t);
        q.v.x *= s;
        q.v.y *= s;
        q.v.z *= s;
        q.w   *= s;

        return q;
    }

    Quat Quat::slerp(Quat q, Quat r, float t) {
        q = q.normalize();
        r = r.normalize();
        float dot = Quat::dot(q, r);

        if (dot < 0.0f) {
            r = r.scale(-1.0f);
            dot = -dot;
        }

        if (dot > 0.9995f) {
            Quat lerp = q + (r - q).scale(t);
            return lerp.normalize();
        }

        float theta_0 = acosf(dot);
        float theta = theta_0 * t;

        Quat q3 = r - q.scale(dot);
        q3 = q3.normalize();

        Quat term1 = q.scale(cosf(theta));
        Quat term2 = q3.scale(sinf(theta));
        return term1 + term2;
    }

    float Quat::dot(Quat a, Quat b) {
        float dot = a.w   * b.w   +
                    a.v.x * b.v.x +
                    a.v.y * b.v.y +
                    a.v.z * b.v.z;

        return dot;
    }

    Quat Quat::operator+(const Quat &right) {
        Quat ret = Quat::identity();

        ret.w = this->w + right.w;
        ret.v = this->v + right.v;

        return ret;
    }
    Quat& Quat::operator+=(const Quat &right) {
        *this = *this + right;
        return *this;
    }

    Quat Quat::operator-(const Quat &right) {
        Quat ret = Quat::identity();

        ret.w = this->w - right.w;
        ret.v = this->v - right.v;

        return ret;
    }
    Quat& Quat::operator-=(const Quat &right) {
        *this = *this - right;
        return *this;
    }

    Quat Quat::operator*(const Quat &right) {
        Quat ret = Quat::identity();
        ret.w = (this->w * right.w) - Vec3::dot(this->v, right.v);
        ret.v = (this->v.scale(right.w) + right.v.scale(this->w)) + Vec3::cross(this->v, right.v);
        
        return ret;
    }
    Quat& Quat::operator*=(const Quat &right) {
        *this = *this * right;
        return *this;
    }

    Vec3 Quat::operator*(const Vec3 &right) {
        Quat q = *this;
        Quat p = Quat::literal(0, right);

        return ((q * p) * q.inverse()).v;
    }

    bool Quat::operator==(const Quat &right) {
        return NEAR_ZERO(this->w - right.w) && (this->v == right.v);
    }
    bool Quat::operator!=(const Quat &right) {
        return !(*this == right);
    }
}
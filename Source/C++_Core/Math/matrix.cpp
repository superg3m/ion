#include "matrix.hpp"
#include "quaternion.hpp"

namespace ION::Math {
    Mat4::Mat4() {
        v[0] = Vec4(0, 0, 0, 0);
        v[1] = Vec4(0, 0, 0, 0);
        v[2] = Vec4(0, 0, 0, 0);
        v[3] = Vec4(0, 0, 0, 0);
    }

    Mat4::Mat4(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3) {
        this->v = {
            r0, 
            r1, 
            r2, 
            r3 
        };
    }

    Mat4::Mat4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) {
        v[0] = Vec4(m00, m01, m02, m03);
        v[1] = Vec4(m10, m11, m12, m13);
        v[2] = Vec4(m20, m21, m22, m23);
        v[3] = Vec4(m30, m31, m32, m33);
    }

    Mat4 Mat4::transpose() {
        Mat4 ret;

        ret.v[0].x = this->v[0].x;
        ret.v[0].y = this->v[1].x;
        ret.v[0].z = this->v[2].x;
        ret.v[0].w = this->v[3].x;

        ret.v[1].x = this->v[0].y;
        ret.v[1].y = this->v[1].y;
        ret.v[1].z = this->v[2].y;
        ret.v[1].w = this->v[3].y;

        ret.v[2].x = this->v[0].z;
        ret.v[2].y = this->v[1].z;
        ret.v[2].z = this->v[2].z;
        ret.v[2].w = this->v[3].z;

        ret.v[3].x = this->v[0].w;
        ret.v[3].y = this->v[1].w;
        ret.v[3].z = this->v[2].w;
        ret.v[3].w = this->v[3].w;      

        return ret;
    }

    Mat4 Mat4::identity() {
        Mat4 ret;
        ret.v = {
            Vec4(1, 0, 0, 0),
            Vec4(0, 1, 0, 0),
            Vec4(0, 0, 1, 0),
            Vec4(0, 0, 0, 1)
        };

        return ret;
    }

    Mat4 Mat4::from_column_major(const float mat[16]) {
        Mat4 ret = {
            Vec4{mat[0], mat[4], mat[8], mat[12]},
            Vec4{mat[1], mat[5], mat[9], mat[13]},
            Vec4{mat[2], mat[6], mat[10], mat[14]},
            Vec4{mat[3], mat[7], mat[11], mat[15]},
        };

        return ret;
    }

    Mat4 Mat4::scale(Mat4 mat, float scale) {
        return Mat4::scale(mat, Vec3(scale, scale, scale));
    }

    Mat4 Mat4::scale(Mat4 mat, Vec3 s) {
        Mat4 scale_matrix;
        scale_matrix.v = {
            Vec4(s.x,  0.0f, 0.0f, 0.0f),
            Vec4(0.0f, s.y,  0.0f, 0.0f),
            Vec4(0.0f, 0.0f, s.z,  0.0f),
            Vec4(0.0f, 0.0f, 0.0f, 1.0f) 
        };

        return scale_matrix * mat;
    }

    Mat4 Mat4::scale(Mat4 mat, float scale_x, float scale_y, float scale_z) {
        return Mat4::scale(mat, Vec3(scale_x, scale_y, scale_z));
    }


    Mat4 Mat4::rotate(Mat4 mat, float theta, Vec3 axis) {
        float rad = DEGREES_TO_RAD(theta);
        float c = cosf(rad);
        float s = sinf(rad);
        float t = 1.0f - c;

        axis = axis.normalize();
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        Mat4 rot;
        rot.v = {
            Vec4(t * x * x + c,     t * x * y - z * s, t * x * z + y * s, 0.0f),
            Vec4(t * x * y + z * s, t * y * y + c,     t * y * z - x * s, 0.0f),
            Vec4(t * x * z - y * s, t * y * z + x * s, t * z * z + c,     0.0f),
            Vec4(0.0f,              0.0f,              0.0f,              1.0f)
        };

        return rot * mat;
    }

    Mat4 Mat4::rotate(Mat4 mat, float theta, float rot_x, float rot_y, float rot_z) {
        return Mat4::rotate(mat, theta, Vec3(rot_x, rot_y, rot_z));
    }

    Mat4 Mat4::rotate(Mat4 mat, Quat quat) {
        float theta;
        Vec3 axis;
        quat.to_angle_axis(theta, axis);
        return Mat4::rotate(mat, theta, axis);
    }

    Mat4 Mat4::translate(Mat4 mat, Vec3 t) {
        Mat4 translate_matrix;
        translate_matrix.v = {
            Vec4(1.0f, 0.0f, 0.0f, t.x),
            Vec4(0.0f, 1.0f, 0.0f, t.y),
            Vec4(0.0f, 0.0f, 1.0f, t.z),
            Vec4(0.0f, 0.0f, 0.0f, 1.0f)
        };

        return translate_matrix * mat;
    }

    Mat4 Mat4::translate(Mat4 mat, float x, float y, float z) {
        return Mat4::translate(mat, Vec3(x, y, z));
    }

    Mat4 Mat4::transform(Vec3 s, float theta, Vec3 axis, Vec3 t) {
        Mat4 scale_matrix = Mat4::scale(Mat4::identity(), s);
        Mat4 rotation_matrix = Mat4::rotate(Mat4::identity(), theta, axis);
        Mat4 translation_matrix = Mat4::translate(Mat4::identity(), t);

        return translation_matrix * rotation_matrix * scale_matrix;
    }

    Mat4 Mat4::inverse_transform(Vec3 s, float theta, Vec3 axis, Vec3 t) {
        Mat4 inverse_scale_matrix = Mat4::scale(Mat4::identity(), s.scale(1 / s.x, 1 / s.y, 1 / s.z));
        Mat4 inverse_rotation_matrix = Mat4::rotate(Mat4::identity(), theta, axis).transpose();
        Mat4 inverse_translation_matrix = Mat4::translate(Mat4::identity(), t.scale(-1));

        return inverse_scale_matrix * inverse_rotation_matrix * inverse_translation_matrix;
    }

    void Mat4::decompose(Mat4 mat, Vec3* out_position, Quat* out_orientation, Vec3* out_scale) {
        Vec3 translation = Vec3(mat.v[0].w, mat.v[1].w, mat.v[2].w);
        Vec3 scale = Vec3(0);
        {
            Vec3 column1 = Vec3(mat.v[0].x, mat.v[1].x, mat.v[2].x);
            Vec3 column2 = Vec3(mat.v[0].y, mat.v[1].y, mat.v[2].y);
            Vec3 column3 = Vec3(mat.v[0].z, mat.v[1].z, mat.v[2].z);
        
            float scale_x = column1.magnitude();
            float scale_y = column2.magnitude();
            float scale_z = column3.magnitude();
            scale = Vec3(scale_x, scale_y, scale_z);
        }
        Quat orientation = Quat::identity();
        {
            Vec3 column1 = Vec3(mat.v[0].x, mat.v[1].x, mat.v[2].x);
            Vec3 column2 = Vec3(mat.v[0].y, mat.v[1].y, mat.v[2].y);
            Vec3 column3 = Vec3(mat.v[0].z, mat.v[1].z, mat.v[2].z);
            
            column1 = column1.scale(1.0f / scale.x);
            column2 = column2.scale(1.0f / scale.y);
            column3 = column3.scale(1.0f / scale.z);
            
            Mat4 rotation_matrix = Mat4(
                Vec4{column1.x, column2.x, column3.x, 0},
                Vec4{column1.y, column2.y, column3.y, 0},
                Vec4{column1.z, column2.z, column3.z, 0},
                Vec4{0,         0,         0,         0}
            );
            orientation = Quat::from_rotation_matrix(rotation_matrix);

            /*
            // TRS = M
            // R = T⁻¹ * M * S⁻¹
            // R = T⁻¹ * (TRS) * S⁻¹
            // R = T⁻¹ * TR * I
            // R = I * R * I
            // R = R
            Vec3 inverse_scale = Vec3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
            Mat4 inverse_scale_matrix = Mat4::scale(Mat4::identity(), inverse_scale);
            Mat4 inverse_translation_matrix = Mat4::translate(Mat4::identity(), translation.scale(-1));
            Mat4 rotation_matrix = inverse_translation_matrix * mat * inverse_scale_matrix;
            orientation = Quat::fromRotationMatrix(rotation_matrix);
            */
        }

        if (out_position) {
            *out_position = translation;
        }

        if (out_scale) {
            *out_scale = scale;
        }

        if (out_orientation) {
            *out_orientation = orientation;
        }
    }

    Mat4 Mat4::perspective(float fov_degrees, float aspect, float near_plane, float far_plane) {
        float fov_radians = DEGREES_TO_RAD(fov_degrees);

        const float t = tanf(fov_radians / 2) * near_plane;
        const float b = -t;
        const float r = t * aspect;
        const float l = -t * aspect;

        const float p = (2.0f * near_plane);

        const float A = p / (r - l);
        const float B = p / (t - b);
        const float C = -((far_plane + near_plane) / (far_plane - near_plane));
        const float D = -((p * far_plane) / (far_plane - near_plane));

        Mat4 ret;
        ret.v = {
            Vec4(A,  0,  0,  0),
            Vec4(0,  B,  0,  0),
            Vec4(0,  0,  C,  D),
            Vec4(0,  0, -1,  0)
        };

        return ret;
    }

    // Found at: https://en.wikipedia.org/wiki/Orthographic_projection
    Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane) {
        const float A = 2.0f / (right - left);
        const float B = 2.0f / (top - bottom);
        const float C = -2.0f / (far_plane - near_plane);
        const float D = -(right + left) / (right - left);
        const float E = -(top + bottom) / (top - bottom);
        const float F = -(far_plane + near_plane) / (far_plane - near_plane);

        Mat4 ret = {
            A,  0,  0,  D,
            0,  B,  0,  E,
            0,  0,  C,  F,
            0,  0,  0,  1
        };

        return ret;
    }

    // Found at: https://www.khronos.org/opengl/wiki/GluLookAt_code
    Mat4 Mat4::lookat(Vec3 position, Vec3 target, Vec3 world_up) {
        Vec3 forward = (position - target).normalize();
        Vec3 right   = Vec3::cross(world_up, forward).normalize();
        Vec3 up      = Vec3::cross(forward, right).normalize();

        Mat4 rotation;
        rotation.v = {
            Vec4(right.x,   right.y,   right.z,   0),
            Vec4(up.x,      up.y,      up.z,      0),
            Vec4(forward.x, forward.y, forward.z, 0),
            Vec4(0,         0,         0,         1)
        };
        
        Mat4 translation = Mat4::translate(Mat4::identity(), -position.x, -position.y, -position.z);

        return rotation * translation;
    }

    float mat3_determinant_helper(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
        return a * (e * i - f * h) -
                b * (d * i - f * g) +
                c * (d * h - e * g);
    }

    Mat4 Mat4::inverse(bool &success) {
        success = false;

        float m00 = this->v[0].x, m01 = this->v[0].y, m02 = this->v[0].z, m03 = this->v[0].w;
        float m10 = this->v[1].x, m11 = this->v[1].y, m12 = this->v[1].z, m13 = this->v[1].w;
        float m20 = this->v[2].x, m21 = this->v[2].y, m22 = this->v[2].z, m23 = this->v[2].w;
        float m30 = this->v[3].x, m31 = this->v[3].y, m32 = this->v[3].z, m33 = this->v[3].w;

        float c00 = mat3_determinant_helper(m11, m12, m13, m21, m22, m23, m31, m32, m33);
        float c01 = mat3_determinant_helper(m10, m12, m13, m20, m22, m23, m30, m32, m33);
        float c02 = mat3_determinant_helper(m10, m11, m13, m20, m21, m23, m30, m31, m33);
        float c03 = mat3_determinant_helper(m10, m11, m12, m20, m21, m22, m30, m31, m32);

        float det = m00 * c00 - m01 * c01 + m02 * c02 - m03 * c03;
        if (NEAR_ZERO(det)) {
            return Mat4::identity();
        }

        float invDet = 1.0f / det;
        Mat4 inv;

        // Row 0
        inv.v[0].x = invDet * c00;
        inv.v[0].y = invDet * (-mat3_determinant_helper(m01, m02, m03, m21, m22, m23, m31, m32, m33));
        inv.v[0].z = invDet * mat3_determinant_helper(m01, m02, m03, m11, m12, m13, m31, m32, m33);
        inv.v[0].w = invDet * (-mat3_determinant_helper(m01, m02, m03, m11, m12, m13, m21, m22, m23));

        // Row 1
        inv.v[1].x = invDet * (-c01);
        inv.v[1].y = invDet * mat3_determinant_helper(m00, m02, m03, m20, m22, m23, m30, m32, m33);
        inv.v[1].z = invDet * (-mat3_determinant_helper(m00, m02, m03, m10, m12, m13, m30, m32, m33));
        inv.v[1].w = invDet * mat3_determinant_helper(m00, m02, m03, m10, m12, m13, m20, m22, m23);

        // Row 2
        inv.v[2].x = invDet * c02;
        inv.v[2].y = invDet * (-mat3_determinant_helper(m00, m01, m03, m20, m21, m23, m30, m31, m33));
        inv.v[2].z = invDet * mat3_determinant_helper(m00, m01, m03, m10, m11, m13, m30, m31, m33);
        inv.v[2].w = invDet * (-mat3_determinant_helper(m00, m01, m03, m10, m11, m13, m20, m21, m23));

        // Row 3
        inv.v[3].x = invDet * (-c03);
        inv.v[3].y = invDet * mat3_determinant_helper(m00, m01, m02, m20, m21, m22, m30, m31, m32);
        inv.v[3].z = invDet * (-mat3_determinant_helper(m00, m01, m02, m10, m11, m12, m30, m31, m32));
        inv.v[3].w = invDet * mat3_determinant_helper(m00, m01, m02, m10, m11, m12, m20, m21, m22);

        success = true;

        return inv;
    }

    Mat4 Mat4::operator*(const Mat4 &right) {
        Mat4 C;

        for (int i = 0; i < 4; i++) {
            C.v[i].x += this->v[i].x * right.v[0].x;
            C.v[i].x += this->v[i].y * right.v[1].x;
            C.v[i].x += this->v[i].z * right.v[2].x;
            C.v[i].x += this->v[i].w * right.v[3].x;

            C.v[i].y += this->v[i].x * right.v[0].y;
            C.v[i].y += this->v[i].y * right.v[1].y;
            C.v[i].y += this->v[i].z * right.v[2].y;
            C.v[i].y += this->v[i].w * right.v[3].y;

            C.v[i].z += this->v[i].x * right.v[0].z;
            C.v[i].z += this->v[i].y * right.v[1].z;
            C.v[i].z += this->v[i].z * right.v[2].z;
            C.v[i].z += this->v[i].w * right.v[3].z;
            
            C.v[i].w += this->v[i].x * right.v[0].w;
            C.v[i].w += this->v[i].y * right.v[1].w;
            C.v[i].w += this->v[i].z * right.v[2].w;
            C.v[i].w += this->v[i].w * right.v[3].w;
        }
            
        return C;
    }

    Vec4 Mat4::operator*(const Vec4 &right) {
        Vec4 ret;
        ret.x += this->v[0].x * right.x;
        ret.x += this->v[0].y * right.y;
        ret.x += this->v[0].z * right.z;
        ret.x += this->v[0].w * right.w;

        ret.y += this->v[1].x * right.x;
        ret.y += this->v[1].y * right.y;
        ret.y += this->v[1].z * right.z;
        ret.y += this->v[1].w * right.w;

        ret.z += this->v[2].x * right.x;
        ret.z += this->v[2].y * right.y;
        ret.z += this->v[2].z * right.z;
        ret.z += this->v[2].w * right.w;
        
        ret.w += this->v[3].x * right.x;
        ret.w += this->v[3].y * right.y;
        ret.w += this->v[3].z * right.z;
        ret.w += this->v[3].w * right.w;
            
        return ret;
    }

    Mat4& Mat4::operator*=(const Mat4 &right) {
        *this = *this * right;
        return *this;
    }

    bool Mat4::operator==(const Mat4 &right) {
        return (this->v[0] == right.v[0]) && (this->v[1] == right.v[1]) && (this->v[2] == right.v[2]) && (this->v[3] == right.v[3]);
    }

    bool Mat4::operator!=(const Mat4 &right) {
        return !(*this == right);
    }
}
#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"
#include "math/Quat.hpp"
namespace ygl {
Mat4::Mat4() { for (int i = 0; i < 16; ++i) data[i] = 0.0f; data[0] = data[5] = data[10] = data[15] = 1.0f; }
Mat4::Mat4(const float* values) { std::memcpy(data, values, 16 * sizeof(float)); }
float Mat4::operator()(int row, int col) const { return data[row * 4 + col]; }
float& Mat4::operator()(int row, int col) { return data[row * 4 + col]; }
Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) {
        result(i, j) = 0.0f;
        for (int k = 0; k < 4; ++k) result(i, j) += (*this)(i, k) * other(k, j);
    }
    return result;
}
Vec3 Mat4::operator*(const Vec3& v) const {
    Vec3 result;
    result.x = (*this)(0,0)*v.x + (*this)(0,1)*v.y + (*this)(0,2)*v.z + (*this)(0,3);
    result.y = (*this)(1,0)*v.x + (*this)(1,1)*v.y + (*this)(1,2)*v.z + (*this)(1,3);
    result.z = (*this)(2,0)*v.x + (*this)(2,1)*v.y + (*this)(2,2)*v.z + (*this)(2,3);
    float w = (*this)(3,0)*v.x + (*this)(3,1)*v.y + (*this)(3,2)*v.z + (*this)(3,3);
    if (w != 0.0f && w != 1.0f) result /= w;
    return result;
}
Vec4 Mat4::operator*(const Vec4& v) const {
    return Vec4(
        (*this)(0,0)*v.x + (*this)(0,1)*v.y + (*this)(0,2)*v.z + (*this)(0,3)*v.w,
        (*this)(1,0)*v.x + (*this)(1,1)*v.y + (*this)(1,2)*v.z + (*this)(1,3)*v.w,
        (*this)(2,0)*v.x + (*this)(2,1)*v.y + (*this)(2,2)*v.z + (*this)(2,3)*v.w,
        (*this)(3,0)*v.x + (*this)(3,1)*v.y + (*this)(3,2)*v.z + (*this)(3,3)*v.w
    );
}
Mat4 Mat4::Identity() { Mat4 m; for (int i = 0; i < 16; ++i) m.data[i] = 0.0f; m(0,0)=m(1,1)=m(2,2)=m(3,3)=1.0f; return m; }
Mat4 Mat4::Translation(const Vec3& vec) { Mat4 m = Identity(); m(0,3)=vec.x; m(1,3)=vec.y; m(2,3)=vec.z; return m; }
Mat4 Mat4::Scale(const Vec3& vec) { Mat4 m = Identity(); m(0,0)=vec.x; m(1,1)=vec.y; m(2,2)=vec.z; return m; }
Mat4 Mat4::RotationX(float angle) { float c=std::cos(angle), s=std::sin(angle); Mat4 m=Identity(); m(1,1)=c; m(1,2)=-s; m(2,1)=s; m(2,2)=c; return m; }
Mat4 Mat4::RotationY(float angle) { float c=std::cos(angle), s=std::sin(angle); Mat4 m=Identity(); m(0,0)=c; m(0,2)=s; m(2,0)=-s; m(2,2)=c; return m; }
Mat4 Mat4::RotationZ(float angle) { float c=std::cos(angle), s=std::sin(angle); Mat4 m=Identity(); m(0,0)=c; m(0,1)=-s; m(1,0)=s; m(1,1)=c; return m; }
Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
    Vec3 z = (eye - target).normalized(); Vec3 x = cross(up, z).normalized(); Vec3 y = cross(z, x);
    Mat4 view; view(0,0)=x.x; view(0,1)=x.y; view(0,2)=x.z; view(0,3)=-dot(x,eye);
    view(1,0)=y.x; view(1,1)=y.y; view(1,2)=y.z; view(1,3)=-dot(y,eye);
    view(2,0)=z.x; view(2,1)=z.y; view(2,2)=z.z; view(2,3)=-dot(z,eye);
    view(3,0)=0.0f; view(3,1)=0.0f; view(3,2)=0.0f; view(3,3)=1.0f; return view;
}
Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
    float tanHalfFov = std::tan(fov/2.0f); Mat4 m;
    m(0,0)=1.0f/(aspect*tanHalfFov); m(1,1)=1.0f/tanHalfFov;
    m(2,2)=(far+near)/(near-far); m(2,3)=(2.0f*far*near)/(near-far); m(3,2)=-1.0f; m(3,3)=0.0f; return m;
}
Mat4 Mat4::orthographic(float l, float r, float b, float t, float n, float f) {
    Mat4 m=Identity(); m(0,0)=2.0f/(r-l); m(1,1)=2.0f/(t-b); m(2,2)=2.0f/(f-n);
    m(0,3)=-(r+l)/(r-l); m(1,3)=-(t+b)/(t-b); m(2,3)=-(f+n)/(f-n); return m;
}
Mat4 Mat4::inverted() const {
    float det = determinant(); if (det == 0.0f) return Identity();
    Mat4 inv; float invDet = 1.0f/det;
    inv(0,0)=(data[5]*data[10]*data[15]-data[5]*data[11]*data[14]-data[9]*data[6]*data[15]+data[9]*data[7]*data[14]+data[13]*data[6]*data[11]-data[13]*data[7]*data[10])*invDet;
    inv(0,1)=(-data[1]*data[10]*data[15]+data[1]*data[11]*data[14]+data[9]*data[2]*data[15]-data[9]*data[3]*data[14]-data[13]*data[2]*data[11]+data[13]*data[3]*data[10])*invDet;
    inv(0,2)=(data[1]*data[6]*data[15]-data[1]*data[7]*data[14]-data[5]*data[2]*data[15]+data[5]*data[3]*data[14]+data[13]*data[2]*data[7]-data[13]*data[3]*data[6])*invDet;
    inv(0,3)=(-data[1]*data[6]*data[11]+data[1]*data[7]*data[10]+data[5]*data[2]*data[11]-data[5]*data[3]*data[10]-data[9]*data[2]*data[7]+data[9]*data[3]*data[6])*invDet;
    inv(1,0)=(-data[4]*data[10]*data[15]+data[4]*data[11]*data[14]+data[8]*data[6]*data[15]-data[8]*data[7]*data[14]-data[12]*data[6]*data[11]+data[12]*data[7]*data[10])*invDet;
    inv(1,1)=(data[0]*data[10]*data[15]-data[0]*data[11]*data[14]-data[8]*data[2]*data[15]+data[8]*data[3]*data[14]+data[12]*data[2]*data[11]-data[12]*data[3]*data[10])*invDet;
    inv(1,2)=(-data[0]*data[6]*data[15]+data[0]*data[7]*data[14]+data[4]*data[2]*data[15]-data[4]*data[3]*data[14]-data[12]*data[2]*data[7]+data[12]*data[3]*data[6])*invDet;
    inv(1,3)=(data[0]*data[6]*data[11]-data[0]*data[7]*data[10]-data[4]*data[2]*data[11]+data[4]*data[3]*data[10]+data[8]*data[2]*data[7]-data[8]*data[3]*data[6])*invDet;
    inv(2,0)=(data[4]*data[9]*data[15]-data[4]*data[11]*data[13]-data[8]*data[5]*data[15]+data[8]*data[7]*data[13]+data[12]*data[5]*data[11]-data[12]*data[7]*data[9])*invDet;
    inv(2,1)=(-data[0]*data[9]*data[15]+data[0]*data[11]*data[13]+data[8]*data[1]*data[15]-data[8]*data[3]*data[13]-data[12]*data[1]*data[11]+data[12]*data[3]*data[9])*invDet;
    inv(2,2)=(data[0]*data[5]*data[15]-data[0]*data[7]*data[13]-data[4]*data[1]*data[15]+data[4]*data[3]*data[13]+data[12]*data[1]*data[7]-data[12]*data[3]*data[5])*invDet;
    inv(2,3)=(-data[0]*data[5]*data[11]+data[0]*data[7]*data[9]+data[4]*data[1]*data[11]-data[4]*data[3]*data[9]-data[8]*data[1]*data[7]+data[8]*data[3]*data[5])*invDet;
    inv(3,0)=(-data[4]*data[9]*data[14]+data[4]*data[10]*data[13]+data[8]*data[5]*data[14]-data[8]*data[6]*data[13]-data[12]*data[5]*data[10]+data[12]*data[6]*data[9])*invDet;
    inv(3,1)=(data[0]*data[9]*data[14]-data[0]*data[10]*data[13]-data[8]*data[1]*data[14]+data[8]*data[2]*data[13]+data[12]*data[1]*data[10]-data[12]*data[2]*data[9])*invDet;
    inv(3,2)=(-data[0]*data[5]*data[14]+data[0]*data[6]*data[13]+data[4]*data[1]*data[14]-data[4]*data[2]*data[13]-data[12]*data[1]*data[6]+data[12]*data[2]*data[5])*invDet;
    inv(3,3)=(data[0]*data[5]*data[10]-data[0]*data[6]*data[9]-data[4]*data[1]*data[10]+data[4]*data[2]*data[9]+data[8]*data[1]*data[6]-data[8]*data[2]*data[5])*invDet;
    return inv;
}
float Mat4::determinant() const {
    return data[0]*(data[5]*(data[10]*data[15]-data[11]*data[14])-data[9]*(data[6]*data[15]-data[7]*data[14])+data[13]*(data[6]*data[11]-data[7]*data[10]))
         - data[1]*(data[4]*(data[10]*data[15]-data[11]*data[14])-data[8]*(data[6]*data[15]-data[7]*data[14])+data[12]*(data[6]*data[11]-data[7]*data[10]))
         + data[2]*(data[4]*(data[9]*data[15]-data[11]*data[13])-data[8]*(data[5]*data[15]-data[7]*data[13])+data[12]*(data[5]*data[11]-data[7]*data[9]))
         - data[3]*(data[4]*(data[9]*data[14]-data[10]*data[13])-data[8]*(data[5]*data[14]-data[6]*data[13])+data[12]*(data[5]*data[10]-data[6]*data[9]));
}
Mat4 Mat4::transposed() const { Mat4 t; for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) t(i,j)=(*this)(j,i); return t; }
} // namespace ygl

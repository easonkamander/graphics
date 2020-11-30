#include <cmath>
#include "Mat4.h"
#include "Vec4.h"


Mat4::Mat4 () {
        p[0] = 0.f;
        p[1] = 0.f;
        p[2] = 0.f;
        p[3] = 0.f;
        p[4] = 0.f;
        p[5] = 0.f;
        p[6] = 0.f;
        p[7] = 0.f;
        p[8] = 0.f;
        p[9] = 0.f;
        p[10] = 0.f;
        p[11] = 0.f;
        p[12] = 0.f;
        p[13] = 0.f;
        p[14] = 0.f;
        p[15] = 0.f;
}

Mat4::Mat4 (Vec4 a, Vec4 b, Vec4 c, Vec4 d) {
        p[0] = a.p[0];
        p[1] = a.p[1];
        p[2] = a.p[2];
        p[3] = a.p[3];
        p[4] = b.p[0];
        p[5] = b.p[1];
        p[6] = b.p[2];
        p[7] = b.p[3];
        p[8] = c.p[0];
        p[9] = c.p[1];
        p[10] = c.p[2];
        p[11] = c.p[3];
        p[12] = d.p[0];
        p[13] = d.p[1];
        p[14] = d.p[2];
        p[15] = d.p[3];
}

Mat4 Mat4::t () const {
        Mat4 out;
        out.p[0] = p[0];
        out.p[1] = p[4];
        out.p[2] = p[8];
        out.p[3] = p[12];
        out.p[4] = p[1];
        out.p[5] = p[5];
        out.p[6] = p[9];
        out.p[7] = p[13];
        out.p[8] = p[2];
        out.p[9] = p[6];
        out.p[10] = p[10];
        out.p[11] = p[14];
        out.p[12] = p[3];
        out.p[13] = p[7];
        out.p[14] = p[11];
        out.p[15] = p[15];
        return out;
}

Vec4 Mat4::slct (int d, int i) const {
        const int a = (4 - 3*d) * i;
        const int b = (3*d + 1);
        return Vec4(p[a], p[a + b], p[a + 2*b], p[a + 3*b]);
}

Vec4 Mat4::operator/ (const Vec4& other) const {
        Vec4 out;
        out.p[0] = slct(0, 0) ^ other;
        out.p[1] = slct(0, 1) ^ other;
        out.p[2] = slct(0, 2) ^ other;
        out.p[3] = slct(0, 3) ^ other;
        return out;
}

Mat4 Mat4::operator/ (const Mat4& other) const {
        return Mat4(slct(0, 0)/other, slct(0, 1)/other, slct(0, 2)/other, slct(0, 3)/other);
}

Mat4 Mat4::inv () const {
        Mat4 out;
        out.p[0] = p[5]*p[10]*p[15] - p[5]*p[11]*p[14] - p[9]*p[6]*p[15] + p[9]*p[7]*p[14] + p[13]*p[6]*p[11] - p[13]*p[7]*p[10];
        out.p[1] = -p[1]*p[10]*p[15] + p[1]*p[11]*p[14] + p[9]*p[2]*p[15] - p[9]*p[3]*p[14] - p[13]*p[2]*p[11] + p[13]*p[3]*p[10];
        out.p[2] = p[1]*p[6]*p[15] - p[1]*p[7]*p[14] - p[5]*p[2]*p[15] + p[5]*p[3]*p[14] + p[13]*p[2]*p[7] - p[13]*p[3]*p[6];
        out.p[3] = -p[1]*p[6]*p[11] + p[1]*p[7]*p[10] + p[5]*p[2]*p[11] - p[5]*p[3]*p[10] - p[9]*p[2]*p[7] + p[9]*p[3]*p[6];
        out.p[4] = -p[4]*p[10]*p[15] + p[4]*p[11]*p[14] + p[8]*p[6]*p[15] - p[8]*p[7]*p[14] - p[12]*p[6]*p[11] + p[12]*p[7]*p[10];
        out.p[5] = p[0]*p[10]*p[15] - p[0]*p[11]*p[14] - p[8]*p[2]*p[15] + p[8]*p[3]*p[14] + p[12]*p[2]*p[11] - p[12]*p[3]*p[10];
        out.p[6] = -p[0]*p[6]*p[15] + p[0]*p[7]*p[14] + p[4]*p[2]*p[15] - p[4]*p[3]*p[14] - p[12]*p[2]*p[7] + p[12]*p[3]*p[6];
        out.p[7] = p[0]*p[6]*p[11] - p[0]*p[7]*p[10] - p[4]*p[2]*p[11] + p[4]*p[3]*p[10] + p[8]*p[2]*p[7] - p[8]*p[3]*p[6];
        out.p[8] = p[4]*p[9]*p[15] - p[4]*p[11]*p[13] - p[8]*p[5]*p[15] + p[8]*p[7]*p[13] + p[12]*p[5]*p[11] - p[12]*p[7]*p[9];
        out.p[9] = -p[0]*p[9]*p[15] + p[0]*p[11]*p[13] + p[8]*p[1]*p[15] - p[8]*p[3]*p[13] - p[12]*p[1]*p[11] + p[12]*p[3]*p[9];
        out.p[10] = p[0]*p[5]*p[15] - p[0]*p[7]*p[13] - p[4]*p[1]*p[15] + p[4]*p[3]*p[13] + p[12]*p[1]*p[7] - p[12]*p[3]*p[5];
        out.p[11] = -p[0]*p[5]*p[11] + p[0]*p[7]*p[9] + p[4]*p[1]*p[11] - p[4]*p[3]*p[9] - p[8]*p[1]*p[7] + p[8]*p[3]*p[5];
        out.p[12] = -p[4]*p[9]*p[14] + p[4]*p[10]*p[13] + p[8]*p[5]*p[14] - p[8]*p[6]*p[13] - p[12]*p[5]*p[10] + p[12]*p[6]*p[9];
        out.p[13] = p[0]*p[9]*p[14] - p[0]*p[10]*p[13] - p[8]*p[1]*p[14] + p[8]*p[2]*p[13] + p[12]*p[1]*p[10] - p[12]*p[2]*p[9];
        out.p[14] = -p[0]*p[5]*p[14] + p[0]*p[6]*p[13] + p[4]*p[1]*p[14] - p[4]*p[2]*p[13] - p[12]*p[1]*p[6] + p[12]*p[2]*p[5];
        out.p[15] = p[0]*p[5]*p[10] - p[0]*p[6]*p[9] - p[4]*p[1]*p[10] + p[4]*p[2]*p[9] + p[8]*p[1]*p[6] - p[8]*p[2]*p[5];
        float det = 1.f/(slct(0, 0) ^ out.slct(1, 0));
        return Mat4(out.slct(0, 0)*det, out.slct(0, 1)*det, out.slct(0, 2)*det, out.slct(0, 3)*det);
}

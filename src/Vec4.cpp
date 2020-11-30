#include <cmath>
#include "Vec4.h"
#include "Mat4.h"




Vec4::Vec4 () {
        p[0] = 0.f;
        p[1] = 0.f;
        p[2] = 0.f;
        p[3] = 0.f;
}

Vec4::Vec4 (float a) {
        p[0] = a;
        p[1] = 0.f;
        p[2] = 0.f;
        p[3] = 0.f;
}

Vec4::Vec4 (float a, float b, float c, float d) {
        p[0] = a;
        p[1] = b;
        p[2] = c;
        p[3] = d;
}

Vec4 Vec4::operator+ (const float& other) const {
        Vec4 out;
        out.p[0] = p[0] + other;
        out.p[1] = p[1] + other;
        out.p[2] = p[2] + other;
        out.p[3] = p[3] + other;
        return out;
}

Vec4 Vec4::operator- (const float& other) const {
        return *this + (other * -1.f);
}

Vec4 Vec4::operator* (const float& other) const {
        Vec4 out;
        out.p[0] = p[0] * other;
        out.p[1] = p[1] * other;
        out.p[2] = p[2] * other;
        out.p[3] = p[3] * other;
        return out;
}

Vec4 Vec4::operator/ (const float& other) const {
        return *this * (1.f / other);
}

Vec4 Vec4::operator+ (const Vec4& other) const {
        Vec4 out;
        out.p[0] = p[0] + other.p[0];
        out.p[1] = p[1] + other.p[1];
        out.p[2] = p[2] + other.p[2];
        out.p[3] = p[3] + other.p[3];
        return out;
}

Vec4 Vec4::operator- (const Vec4& other) const {
        return *this + (other * -1.f);
}

Vec4 Vec4::operator* (const Vec4& other) const {
        Vec4 out;
        out.p[0] = p[0] * other.p[0];
        out.p[1] = p[1] * other.p[1];
        out.p[2] = p[2] * other.p[2];
        out.p[3] = p[3] * other.p[3];
        return out;
}

Vec4 Vec4::inv () const {
        Vec4 out;
        out.p[0] = 1.f/p[0];
        out.p[1] = 1.f/p[1];
        out.p[2] = 1.f/p[2];
        out.p[3] = 1.f/p[3];
        return out;
}

float Vec4::sum () const {
        return p[0] + p[1] + p[2] + p[3];
}

float Vec4::operator^ (const Vec4& other) const {
        return (*this * other).sum();
}

float Vec4::len () const {
        return sqrt(*this ^ *this);
}

Vec4 Vec4::normal () const {
        return *this / len();
}

Mat4 Vec4::operator/ (const Vec4& other) {
        Mat4 out;
        for (int i = 0; i < 16; i++) {
                out.p[i] = p[i/4] * other.p[i%4];
        }
        return out;
}

Vec4 Vec4::operator/ (const Mat4& other) {
        Vec4 out;
        out.p[0] = *this ^ other.slct(1, 0);
        out.p[1] = *this ^ other.slct(1, 1);
        out.p[2] = *this ^ other.slct(1, 2);
        out.p[3] = *this ^ other.slct(1, 3);
        return out;
}

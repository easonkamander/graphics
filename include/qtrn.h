#pragma once

#include <string>
#include <iostream>

struct Vec4;
struct Mat4;
struct Qtrn;

struct Vec4 {
	float p[4] = {0.f};
	Vec4 ();
	Vec4 (float a);
	Vec4 (int i, float a);
	Vec4 (float a, float b, float c, float d);
	float sum () const;
	Vec4 operator+ (const Vec4& other) const;
	Vec4 operator- (const Vec4& other) const;
	Vec4 operator* (const Vec4& other) const;
	Vec4 inv () const;
	float operator^ (const Vec4& other) const;
	float len () const;
	Vec4 normal () const;
	Mat4 operator% (const Vec4& other) const;
	Vec4 operator% (const Mat4& other) const;
	std::string toString () const;
};

struct Mat4 {
	float p[16] = {0.f};
	Mat4 ();
	Mat4 (float a);
	Mat4 (int i, Vec4 a);
	Mat4 (Vec4 a, Vec4 b, Vec4 c, Vec4 d);
	Vec4 slct (int d, int i) const;
	Mat4 operator* (const Mat4& other) const;
	Mat4 trs () const;
	Mat4 inv () const;
	Vec4 operator% (const Vec4& other) const;
	Mat4 operator% (const Mat4& other) const;
	std::string toString () const;
};

struct Qtrn {
	Vec4 v;
	Mat4 m;
};

std::ostream& operator<< (std::ostream& ostr, const Vec4& v);
std::ostream& operator<< (std::ostream& ostr, const Mat4& m);

// abs(i/4 - i%4) + 2*(i/5 == 1)*(i%3 == 0)
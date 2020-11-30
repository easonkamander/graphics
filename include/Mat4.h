#pragma once

struct Vec4;

struct Mat4 {
	float p[16];
	Mat4 ();
	Mat4 (Vec4 a, Vec4 b, Vec4 c, Vec4 d);
	Mat4 t () const;
	Vec4 slct (int d, int i) const;
	Vec4 operator/ (const Vec4& other) const;
	Mat4 operator/ (const Mat4& other) const;
	Mat4 inv () const;
};
#pragma once

struct Mat4;

struct Vec4 {
	float p[4];
	Vec4 ();
	Vec4 (float a);
	Vec4 (float a, float b, float c, float d);
	Vec4 operator+ (const float& other) const;
	Vec4 operator- (const float& other) const;
	Vec4 operator* (const float& other) const;
	Vec4 operator/ (const float& other) const;
	Vec4 operator+ (const Vec4& other) const;
	Vec4 operator- (const Vec4& other) const;
	Vec4 operator* (const Vec4& other) const;
	Vec4 inv () const;
	float sum () const;
	float operator^ (const Vec4& other) const;
	float len () const;
	Vec4 normal () const;
	Mat4 operator/ (const Vec4& other);
	Vec4 operator/ (const Mat4& other);
};
#include <cmath>
#include <qtrn.h>

Vec4::Vec4 () {}

Vec4::Vec4 (float a) {
	p[0] = a;
	p[1] = a;
	p[2] = a;
	p[3] = a;
}

Vec4::Vec4 (int i, float a) {
	p[i] = a;
}

Vec4::Vec4 (float a, float b, float c, float d) {
	p[0] = a;
	p[1] = b;
	p[2] = c;
	p[3] = d;
}

float Vec4::sum () const {
	return p[0] + p[1] + p[2] + p[3];
}

Vec4 Vec4::operator+ (const Vec4& other) const {
	return Vec4(
		p[0] + other.p[0],
		p[1] + other.p[1],
		p[2] + other.p[2],
		p[3] + other.p[3]
	);
}

Vec4 Vec4::operator- (const Vec4& other) const {
	return *this + other * Vec4(-1.f);
}

Vec4 Vec4::operator* (const Vec4& other) const {
	return Vec4(
		p[0] * other.p[0],
		p[1] * other.p[1],
		p[2] * other.p[2],
		p[3] * other.p[3]
	);
}

Vec4 Vec4::inv () const {
	return Vec4(
		1.f/p[0],
		1.f/p[1],
		1.f/p[2],
		1.f/p[3]
	);
}

float Vec4::operator^ (const Vec4& other) const {
	return (*this * other).sum();
}

float Vec4::len () const {
	return std::sqrt(*this ^ *this);
}

Vec4 Vec4::normal () const {
	return *this * Vec4(1.f/len());
}

Mat4 Vec4::operator% (const Vec4& other) const {
	return Mat4(
		Vec4(
			p[0] * other.p[0],
			p[0] * other.p[1],
			p[0] * other.p[2],
			p[0] * other.p[3]
		), Vec4(
			p[1] * other.p[0],
			p[1] * other.p[1],
			p[1] * other.p[2],
			p[1] * other.p[3]
		), Vec4(
			p[2] * other.p[0],
			p[2] * other.p[1],
			p[2] * other.p[2],
			p[2] * other.p[3]
		), Vec4(
			p[3] * other.p[0],
			p[3] * other.p[1],
			p[3] * other.p[2],
			p[3] * other.p[3]
		)
	);
}

Vec4 Vec4::operator% (const Mat4& other) const {
	Vec4 out;
	out.p[0] = *this ^ other.slct(1, 0);
	out.p[1] = *this ^ other.slct(1, 1);
	out.p[2] = *this ^ other.slct(1, 2);
	out.p[3] = *this ^ other.slct(1, 3);
	return out;
}
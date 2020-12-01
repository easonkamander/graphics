#include <qtrn.h>

Mat4::Mat4 () {}

Mat4::Mat4 (float a) {
	p[0] = a;
	p[1] = a;
	p[2] = a;
	p[3] = a;
	p[4] = a;
	p[5] = a;
	p[6] = a;
	p[7] = a;
	p[8] = a;
	p[9] = a;
	p[10] = a;
	p[11] = a;
	p[12] = a;
	p[13] = a;
	p[14] = a;
	p[15] = a;
}

Mat4::Mat4 (int i, Vec4 a) {
	p[4*i + 0] = a.p[0];
	p[4*i + 1] = a.p[1];
	p[4*i + 2] = a.p[2];
	p[4*i + 3] = a.p[3];
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

Vec4 Mat4::slct (int d, int i) const {
	const int a = (4 - 3*d) * i;
	const int b = 3*d + 1;
	return Vec4(p[a], p[a + b], p[a + 2*b], p[a + 3*b]);
}

Mat4 Mat4::operator* (const Mat4& other) const {
	return Mat4(
		slct(0, 0) * other.slct(0, 0),
		slct(0, 1) * other.slct(0, 1),
		slct(0, 2) * other.slct(0, 2),
		slct(0, 3) * other.slct(0, 3)
	);
}

Mat4 Mat4::trs () const {
	return Mat4(
		slct(1, 0),
		slct(1, 1),
		slct(1, 2),
		slct(1, 3)
	);
}

Mat4 Mat4::inv () const {
	Mat4 out = Mat4(
		Vec4(
			p[5]*p[10]*p[15] - p[5]*p[11]*p[14] - p[9]*p[6]*p[15] + p[9]*p[7]*p[14] + p[13]*p[6]*p[11] - p[13]*p[7]*p[10],
			-p[1]*p[10]*p[15] + p[1]*p[11]*p[14] + p[9]*p[2]*p[15] - p[9]*p[3]*p[14] - p[13]*p[2]*p[11] + p[13]*p[3]*p[10],
			p[1]*p[6]*p[15] - p[1]*p[7]*p[14] - p[5]*p[2]*p[15] + p[5]*p[3]*p[14] + p[13]*p[2]*p[7] - p[13]*p[3]*p[6],
			-p[1]*p[6]*p[11] + p[1]*p[7]*p[10] + p[5]*p[2]*p[11] - p[5]*p[3]*p[10] - p[9]*p[2]*p[7] + p[9]*p[3]*p[6]
		), Vec4(
			-p[4]*p[10]*p[15] + p[4]*p[11]*p[14] + p[8]*p[6]*p[15] - p[8]*p[7]*p[14] - p[12]*p[6]*p[11] + p[12]*p[7]*p[10],
			p[0]*p[10]*p[15] - p[0]*p[11]*p[14] - p[8]*p[2]*p[15] + p[8]*p[3]*p[14] + p[12]*p[2]*p[11] - p[12]*p[3]*p[10],
			-p[0]*p[6]*p[15] + p[0]*p[7]*p[14] + p[4]*p[2]*p[15] - p[4]*p[3]*p[14] - p[12]*p[2]*p[7] + p[12]*p[3]*p[6],
			p[0]*p[6]*p[11] - p[0]*p[7]*p[10] - p[4]*p[2]*p[11] + p[4]*p[3]*p[10] + p[8]*p[2]*p[7] - p[8]*p[3]*p[6]
		), Vec4(
			p[4]*p[9]*p[15] - p[4]*p[11]*p[13] - p[8]*p[5]*p[15] + p[8]*p[7]*p[13] + p[12]*p[5]*p[11] - p[12]*p[7]*p[9],
			-p[0]*p[9]*p[15] + p[0]*p[11]*p[13] + p[8]*p[1]*p[15] - p[8]*p[3]*p[13] - p[12]*p[1]*p[11] + p[12]*p[3]*p[9],
			p[0]*p[5]*p[15] - p[0]*p[7]*p[13] - p[4]*p[1]*p[15] + p[4]*p[3]*p[13] + p[12]*p[1]*p[7] - p[12]*p[3]*p[5],
			-p[0]*p[5]*p[11] + p[0]*p[7]*p[9] + p[4]*p[1]*p[11] - p[4]*p[3]*p[9] - p[8]*p[1]*p[7] + p[8]*p[3]*p[5]
		), Vec4(
			-p[4]*p[9]*p[14] + p[4]*p[10]*p[13] + p[8]*p[5]*p[14] - p[8]*p[6]*p[13] - p[12]*p[5]*p[10] + p[12]*p[6]*p[9],
			p[0]*p[9]*p[14] - p[0]*p[10]*p[13] - p[8]*p[1]*p[14] + p[8]*p[2]*p[13] + p[12]*p[1]*p[10] - p[12]*p[2]*p[9],
			-p[0]*p[5]*p[14] + p[0]*p[6]*p[13] + p[4]*p[1]*p[14] - p[4]*p[2]*p[13] - p[12]*p[1]*p[6] + p[12]*p[2]*p[5],
			p[0]*p[5]*p[10] - p[0]*p[6]*p[9] - p[4]*p[1]*p[10] + p[4]*p[2]*p[9] + p[8]*p[1]*p[6] - p[8]*p[2]*p[5]
		)
	);
	float determinant = slct(0, 0) ^ out.slct(1, 0);
	if (determinant == 0) {
		throw std::domain_error("Determinant Is Zero");
	}
	return out * Mat4(1.f/determinant);
}

Vec4 Mat4::operator% (const Vec4& other) const {
	return Vec4(
		slct(0, 0) ^ other,
		slct(0, 1) ^ other,
		slct(0, 2) ^ other,
		slct(0, 3) ^ other
	);
}

Mat4 Mat4::operator% (const Mat4& other) const {
	return Mat4(
		slct(0, 0) % other,
		slct(0, 1) % other,
		slct(0, 2) % other,
		slct(0, 3) % other
	);
}
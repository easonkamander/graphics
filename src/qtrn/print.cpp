#include <qtrn.h>
#include <sstream>
#include <iomanip>

std::string Vec4::toString () const {
	std::stringstream streams[4];
	for (int i = 0; i < 4; i++) {
		streams[i] << std::setfill(' ') << std::setw(12) << p[i];
	}
    std::string out = "[" + streams[0].str() + "," + streams[1].str() + "," + streams[2].str() + "," + streams[3].str() + "]";
	return out;
}

std::string Mat4::toString () const {
	return "[" + slct(0, 0).toString() + ",\n " + slct(0, 1).toString() + ",\n " + slct(0, 2).toString() + ",\n " + slct(0, 3).toString() + "]";
}

std::ostream& operator<< (std::ostream& out, const Vec4& v) {
	return out << v.toString();
}

std::ostream& operator<< (std::ostream& out, const Mat4& m) {
	return out << m.toString();
}
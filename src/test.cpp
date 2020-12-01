#include <qtrn.h>

int main () {
	Mat4 t = Mat4(Vec4(0, 1, 2, 4), Vec4(10, -21, 30, 40), Vec4(100, 32, 300, 400), Vec4(1500, 2000, 3000, 4000));
	Mat4 i = Mat4(Vec4(0, 1), Vec4(1, 1), Vec4(2, 1), Vec4(3, 1));
	std::cout << t.inv() << std::endl;
	return 0;
}
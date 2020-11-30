main:
	g++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -Iinclude src/* -o main.out

test:
	g++ -Iinclude src/Vec4.cpp src/Mat4.cpp src/test.cpp src/test.cpp -o quaternions.out
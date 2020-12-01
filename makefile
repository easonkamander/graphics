main:
	g++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -Iinclude src/* -o main.out

test:
	g++ -Iinclude src/qtrn/* src/test.cpp -o main.out
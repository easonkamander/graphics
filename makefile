main:
	g++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -Iinclude src/* -o main.out
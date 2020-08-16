#version 460
in vec2 pixel;
out vec2 point;
void main() {
	point = pixel*0.5f + 0.5f;
	gl_Position = vec4(pixel, 0.0, 1.0);
}
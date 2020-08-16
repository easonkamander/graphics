#version 460
uniform sampler2D tex;
in vec2 point;
out vec4 color;
void main() {
	color = texture(tex, point);
}
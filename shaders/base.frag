#version 460
uniform sampler2D srcTex;
in vec2 point;
out vec4 color;
void main() {
	color = texture(srcTex, point);
}
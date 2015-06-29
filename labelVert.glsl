#version 330

uniform vec2 offSet;

in  vec2 posAttrib;
in  vec4 colorAttrib;

out vec4 colorInterp;

void main() {
	colorInterp = colorAttrib;
	gl_Position = vec4(posAttrib+offSet, 0.0, 1.0);
}

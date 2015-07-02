#version 330

uniform vec2 screenSize;
uniform vec2 glyphSize;
uniform vec2 offSet;

in  vec3  pos;

in  vec3  color;
out vec4  color_terp;

in  vec2  glyphUV;
out vec2  glyphUV_terp;

in  float wordU;
out float wordU_terp;

in  float glyphI;
out float glyphI_terp;

void main() {
	color_terp   = vec4(color, 1.0);
	glyphUV_terp = glyphUV;
	wordU_terp   = wordU;
	glyphI_terp  = glyphI;
	gl_Position  = vec4(
		(pos.x*(glyphSize.x/(screenSize.x/2))-1.0) + offSet.x,
		(0-(pos.y*(glyphSize.y/(screenSize.y/2))))+1.0 + offSet.y,
		pos.z,
		1.0
	);
}

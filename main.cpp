//A program only uses one glyphmap,
//though perhaps multiple glyphmaps could be
//combined into one if a program needs it.
//A root struct holds a label,
//the label has a character count,
//the label's width is it's character count
//times the character width of the glyphmap.

#include <iostream>
#include <SDL2/SDL.h>
#define   GLEW_STATIC
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
using namespace std;
typedef unsigned int uint;

enum labelVertAttribs {
	x, y, z, glyphY, glyphX, wordX, glyphIndex, objIndex,
	labelVertAttribCount
};

#include "foundation.hpp"
#include "someroots.hpp"


int main(int argc, char *argv[]) {
	cout << sizeof(string) << endl;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	const float    videoWidth  = 1280;
	const float    videoHeight =  720;
	//const uint32_t videoSize   = videoWidth * videoHeight;
	SDL_Window    *window    = NULL;
	SDL_GLContext  GLcontext = NULL;
	window = SDL_CreateWindow(
		__FILE__,                  //const char* title,
		SDL_WINDOWPOS_UNDEFINED,   //int         x,
		SDL_WINDOWPOS_UNDEFINED,   //int         y,
		videoWidth,                //int         w,
		videoHeight,               //int         h,
		SDL_WINDOW_OPENGL          //Uint32      flags
	);
	GLcontext = SDL_GL_CreateContext(window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	float vertices[] = {
		00.0f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //Vertex 1: Red
		00.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, //Vertex 2: Green
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f  //Vertex 3: Blue
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	int  shaderLogSize = 1024;
	char shaderLogBuffer[shaderLogSize];
	
	const char *vertexSource = 
		"#version 330\n"
		"uniform vec2 offSet;\n"
		"in      vec2 posAttrib;\n"
		"in      vec4 colorAttrib;\n"
		"out     vec4 colorInterp;\n"
		"void main() {\n"
			"colorInterp = colorAttrib;\n"
			"gl_Position = vec4(posAttrib+offSet, 0.0, 1.0);\n"
	"}";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, shaderLogBuffer);
	cout << "compiling vertex shader...\n" << shaderLogBuffer << endl;
	
	const char *fragmentSource = 
		"#version 330\n"
		"in  vec4 colorInterp;\n"
		"out vec4 outColor;\n"
		"void main() { outColor = colorInterp; }";
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderInfoLog(fragmentShader, shaderLogSize, NULL, shaderLogBuffer);
	cout << "compiling fragment shader...\n" << shaderLogBuffer << endl;
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	
	GLint posAttrib = glGetAttribLocation(shaderProgram, "posAttrib");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
	
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "colorAttrib");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));
	
	
	
	
	
	GLint offSet = glGetUniformLocation(shaderProgram, "offSet");
	glUniform2f(offSet, 0.5f, 0.0f);
	
	
	bool  running  = true;
	//float curFrame = 1;
	//int   runTime  = 3000;//ms
	//Uint32 timeout = SDL_GetTicks() + runTime;
	while (/*!SDL_TICKS_PASSED(SDL_GetTicks(), timeout) && */running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (
				event.type == SDL_QUIT ||
				(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			) {
				running = false;
			}
		}
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		SDL_GL_SwapWindow(window);
		//curFrame++;
		SDL_Delay(10);
	}
	//cout << "ended on frame " << curFrame << ", about " 
	//<< curFrame/(runTime/1000) << " FPS" << endl;
	
	
	SDL_GL_DeleteContext(GLcontext);
	SDL_Quit();
	return 0;
}



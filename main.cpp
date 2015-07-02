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

#include "foundation.hpp"
#include "someroots.hpp"


#include <string>
#include <sstream>
#include <fstream>
const uint shaderLogSize = 1024;
char shaderLogBuffer[shaderLogSize];
void initGLshader(GLint shader, const char *filename) {
	ifstream sourceFile(filename);
	stringstream sourceStream;
	sourceStream << sourceFile.rdbuf();
	string sourceString = sourceStream.str();
	const char *source = sourceString.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderInfoLog(shader, shaderLogSize, NULL, shaderLogBuffer);
	cout << "compiling " << filename << "...\n" << shaderLogBuffer << endl;
}


int main(int argc, char *argv[]) {
	
	activeGlyphSheet.glyphW = 16;
	activeGlyphSheet.glyphH = 24;
	buildsomeroots();
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	const float videoW = 1280;
	const float videoH =  720;
	//const uint32_t videoSize   = videoW * videoH;
	SDL_Window    *window    = NULL;
	SDL_GLContext  GLcontext = NULL;
	window = SDL_CreateWindow(
		__FILE__,                  //const char* title,
		SDL_WINDOWPOS_UNDEFINED,   //int         x,
		SDL_WINDOWPOS_UNDEFINED,   //int         y,
		videoW,                    //int         w,
		videoH,                    //int         h,
		SDL_WINDOW_OPENGL          //Uint32      flags
	);
	GLcontext = SDL_GL_CreateContext(window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	
	const char *labelVertSrcFilename = "labelVert.glsl";
	GLuint labelVertShader = glCreateShader(GL_VERTEX_SHADER);
	initGLshader(labelVertShader, labelVertSrcFilename);
	const char *labelFragSrcFilename = "labelFrag.glsl";
	GLuint labelFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	initGLshader(labelFragShader, labelFragSrcFilename);
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, labelVertShader);
	glAttachShader(shaderProgram, labelFragShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	
	
	
	
	vector<float> rootVerts;
	buildVerts(rootVerts);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float)*rootVerts.size(), 
		rootVerts.data(), 
		GL_STATIC_DRAW
	);
	
	GLint posAttrib     = glGetAttribLocation(shaderProgram, "pos");
	GLint colorAttrib   = glGetAttribLocation(shaderProgram, "color");
	GLint glyphUVAttrib = glGetAttribLocation(shaderProgram, "glyphUV");
	GLint wordUAttrib   = glGetAttribLocation(shaderProgram, "wordU");
	GLint glyphIAttrib  = glGetAttribLocation(shaderProgram, "glyphI");
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colorAttrib);
	glEnableVertexAttribArray(glyphUVAttrib);
	glEnableVertexAttribArray(wordUAttrib);
	glEnableVertexAttribArray(glyphIAttrib);
	glVertexAttribPointer(posAttrib,     3, GL_FLOAT, GL_FALSE, labelVertAttribCount*sizeof(float), NULL);
	glVertexAttribPointer(colorAttrib,   3, GL_FLOAT, GL_FALSE, labelVertAttribCount*sizeof(float), (void*)(sizeof(float)*lva_r));
	glVertexAttribPointer(glyphUVAttrib, 2, GL_FLOAT, GL_FALSE, labelVertAttribCount*sizeof(float), (void*)(sizeof(float)*lva_glyphU));
	glVertexAttribPointer(wordUAttrib,   1, GL_FLOAT, GL_FALSE, labelVertAttribCount*sizeof(float), (void*)(sizeof(float)*lva_wordU ));
	glVertexAttribPointer(glyphIAttrib,  1, GL_FLOAT, GL_FALSE, labelVertAttribCount*sizeof(float), (void*)(sizeof(float)*lva_glyphI));
	
	
	GLint screenSize = glGetUniformLocation(shaderProgram, "screenSize");
	GLint glyphSize  = glGetUniformLocation(shaderProgram, "glyphSize");
	GLint offSet     = glGetUniformLocation(shaderProgram, "offSet");
	glUniform2f(screenSize, videoW, videoH);
	glUniform2f(glyphSize, activeGlyphSheet.glyphW, activeGlyphSheet.glyphH);
	glUniform2f(offSet, 0.1, -0.1);
	
	
	
	
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
		
		glDrawArrays(GL_TRIANGLES, 0, rootVerts.size());
		
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




cc=g++
cflags= -Wall
OpenGL= -lGL -lGLEW
OpenCL= -lOpenCL -I/opt/AMDAPPSDK-3.0-0-Beta/include -L/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64/
SDL2  = `sdl2-config --cflags --libs`

ShaderPunk: main.cpp foundation.hpp someroots.hpp
	$(cc) -o $@ $< $(cflags) $(OpenGL) $(SDL2)

clean:
	rm  ShaderPunk


cc=g++
cflags= -std=c++11 -Wall -g
SDL2  = `sdl2-config --cflags --libs`
OpenCL= -lOpenCL -I/opt/AMDAPPSDK-3.0-0-Beta/include -L/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64/

ShaderPunk: main.cpp init.hpp foundation.hpp testRoots.hpp error.hpp makefile
	$(cc) -o $@ $< $(cflags) $(SDL2) $(OpenCL)

clean:
	rm  ShaderPunk

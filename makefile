
cc=g++
cflags= -Wall -std=c++11
SDL2  = `sdl2-config --cflags --libs`
OpenCL= -lOpenCL -I/opt/AMDAPPSDK-3.0-0-Beta/include -L/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64/

ShaderPunk: main.cpp init.hpp foundation.hpp someroots.hpp
	$(cc) -o $@ $< $(cflags) $(SDL2) $(OpenCL)

clean:
	rm  ShaderPunk


cc=g++
cflags= -std=c++11 -Wall -g
SDL2  = `sdl2-config --cflags --libs`
OpenCL= -lOpenCL -I/opt/AMDAPPSDK-3.0-0-Beta/include -L/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64/

ShaderPunk: main.cpp 01_error.hpp 02_CLtypes.hpp 03_sharedWithCL.h 04_initCL.hpp 05_foundation.hpp 06_testRoots.hpp 07_scrollable.hpp
	$(cc) -o $@ $< $(cflags) $(SDL2) $(OpenCL)

clean:
	rm  ShaderPunk

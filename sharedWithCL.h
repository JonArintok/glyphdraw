// This header is included in both the main program, and the OpenCL kernel

#define kernelInspectArgIndex 0//5

typedef struct {
	uint2 glyphSize;
	uint2 glyphCount;
	uint unicodeFirst;
	uint unicodeLast;
} glyphSheetInfo;

// This header is included in both the main program, and the OpenCL kernel

#define kernelInspectArgIndex 0//6

typedef struct {
	uint glyphW;
	uint glyphH;
	uint colCount;
	uint rowCount;
	uint unicodeFirst;
	uint unicodeLast;
} glyphSheetInfo;

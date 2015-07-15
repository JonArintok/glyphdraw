// This header is included in both the main program, and the OpenCL kernel

typedef struct {
	uint glyphW;
	uint glyphH;
	uint colCount;
	uint rowCount;
	uint unicodeFirst;
	uint unicodeLast;
} glyphSheetInfo;

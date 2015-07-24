// This header is included in both the main program, and the OpenCL kernel

#define kernelInspectArgIndex 0

typedef struct {
  int2 glyphSize;
  int2 glyphCount;
  //int  unicodeFirst;
  //int  unicodeLast;
} glyphSheetInfo;

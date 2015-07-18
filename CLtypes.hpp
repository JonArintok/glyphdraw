
typedef uint8_t  uchar;
typedef uint32_t uint;

#define __pair(dt, st)\
struct dt {\
	st x;\
	st y;\
	dt (st xIn, st yIn) : x(xIn), y(yIn) {}\
	dt operator + (dt b) {return dt(x + b.x, y + b.y);}\
	dt operator - (dt b) {return dt(x - b.x, y - b.y);}\
	dt operator * (dt b) {return dt(x * b.x, y * b.y);}\
	dt operator / (dt b) {return dt(x / b.x, y / b.y);}\
	dt operator % (dt b) {return dt(x % b.x, y % b.y);}\
	dt operator ^ (dt b) {return dt(x ^ b.x, y ^ b.y);}\
	bool operator == (dt b) {return x == b.x  &&  y == b.y;}\
	bool operator != (dt b) {return x != b.x  ||  y != b.y;}\
};

__pair(int2, int)
__pair(uint2, uint)

/*
struct uint2 {
	uint x;
	uint y;
	uint2 (uint xIn, uint yIn) : x(xIn), y(yIn) {}
	uint2 operator + (uint2 b) {return uint2(x + b.x, y + b.y);}
	uint2 operator - (uint2 b) {return uint2(x - b.x, y - b.y);}
	uint2 operator * (uint2 b) {return uint2(x * b.x, y * b.y);}
	uint2 operator / (uint2 b) {return uint2(x / b.x, y / b.y);}
	uint2 operator % (uint2 b) {return uint2(x % b.x, y % b.y);}
	uint2 operator ^ (uint2 b) {return uint2(x ^ b.x, y ^ b.y);}
	bool operator == (uint2 b) {return x == b.x  &&  y == b.y;}
	bool operator != (uint2 b) {return x != b.x  ||  y != b.y;}
};

struct int2 {
	int x;
	int y;
	int2 (int xIn, int yIn) : x(xIn), y(yIn) {}
	int2 operator + (int2 b) {return int2(x + b.x, y + b.y);}
	int2 operator - (int2 b) {return int2(x - b.x, y - b.y);}
	int2 operator * (int2 b) {return int2(x * b.x, y * b.y);}
	int2 operator / (int2 b) {return int2(x / b.x, y / b.y);}
	int2 operator % (int2 b) {return int2(x % b.x, y % b.y);}
	int2 operator ^ (int2 b) {return int2(x ^ b.x, y ^ b.y);}
	bool operator == (int2 b) {return x == b.x  &&  y == b.y;}
	bool operator != (int2 b) {return x != b.x  ||  y != b.y;}
};
*/
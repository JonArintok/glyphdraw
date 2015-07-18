
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

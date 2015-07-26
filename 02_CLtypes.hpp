
typedef uint8_t  uchar;

#define __pair(dt, st)\
struct dt {\
  st x;\
  st y;\
  dt () : x(0), y(0) {}\
  dt (st xIn, st yIn) : x(xIn), y(yIn) {}\
  dt   operator +  (dt b) {return dt(x + b.x, y + b.y);}\
  dt   operator -  (dt b) {return dt(x - b.x, y - b.y);}\
  dt   operator *  (dt b) {return dt(x * b.x, y * b.y);}\
  dt   operator /  (dt b) {return dt(x / b.x, y / b.y);}\
  dt   operator %  (dt b) {return dt(x % b.x, y % b.y);}\
  dt   operator ^  (dt b) {return dt(x ^ b.x, y ^ b.y);}\
  bool operator == (dt b) {return x == b.x  &&  y == b.y;}\
  bool operator != (dt b) {return x != b.x  ||  y != b.y;}\
  dt   operator +  (st b) {return dt(x + b, y + b);}\
  dt   operator -  (st b) {return dt(x - b, y - b);}\
  dt   operator *  (st b) {return dt(x * b, y * b);}\
  dt   operator /  (st b) {return dt(x / b, y / b);}\
  dt   operator %  (st b) {return dt(x % b, y % b);}\
  dt   operator ^  (st b) {return dt(x ^ b, y ^ b);}\
  bool operator == (st b) {return x == b  &&  y == b;}\
  bool operator != (st b) {return x != b  ||  y != b;}\
  void operator += (dt b) {x += b.x; y += b.y;}\
  void operator -= (dt b) {x -= b.x; y -= b.y;}\
  void operator *= (dt b) {x *= b.x; y *= b.y;}\
  void operator /= (dt b) {x /= b.x; y /= b.y;}\
  void operator += (st b) {x += b; y += b;}\
  void operator -= (st b) {x -= b; y -= b;}\
  void operator *= (st b) {x *= b; y *= b;}\
  void operator /= (st b) {x /= b; y /= b;}\
  void operator  = (st b) {x  = b; y  = b;}\
  st pro() {return x*y;}\
};

__pair(int2, int)

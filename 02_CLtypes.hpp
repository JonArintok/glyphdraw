
typedef uint8_t  uchar;


#define __pair(dt, st)\
struct dt {\
  st x;\
  st y;\
  dt () : x(0), y(0) {}\
  dt (const st xIn, const st yIn) : x(xIn), y(yIn) {}\
  dt operator +  (const dt b) {return dt(x + b.x, y + b.y);}\
  dt operator -  (const dt b) {return dt(x - b.x, y - b.y);}\
  dt operator *  (const dt b) {return dt(x * b.x, y * b.y);}\
  dt operator /  (const dt b) {return dt(x / b.x, y / b.y);}\
  bool   operator == (const dt b) {return x == b.x  &&  y == b.y;}\
  bool   operator != (const dt b) {return x != b.x  ||  y != b.y;}\
  void   operator += (const dt b) {x += b.x; y += b.y;}\
  void   operator -= (const dt b) {x -= b.x; y -= b.y;}\
  void   operator *= (const dt b) {x *= b.x; y *= b.y;}\
  void   operator /= (const dt b) {x /= b.x; y /= b.y;}\
  dt operator +  (const st  b) {return dt(x + b, y + b);}\
  dt operator -  (const st  b) {return dt(x - b, y - b);}\
  dt operator *  (const st  b) {return dt(x * b, y * b);}\
  dt operator /  (const st  b) {return dt(x / b, y / b);}\
  bool   operator == (const st  b) {return x == b  &&  y == b;}\
  bool   operator != (const st  b) {return x != b  ||  y != b;}\
  void   operator += (const st  b) {x += b; y += b;}\
  void   operator -= (const st  b) {x -= b; y -= b;}\
  void   operator *= (const st  b) {x *= b; y *= b;}\
  void   operator /= (const st  b) {x /= b; y /= b;}\
  void   operator  = (const st  b) {x  = b; y  = b;}\
  bool   operator !  () {return !x && !y;}\
  st pro() {return x*y;}\
  st sel(uint8_t axis) {\
    switch(axis) {\
      case  0: return x;\
      case  1: return y;\
      default: return 0;\
    }\
  }\
};

__pair(int2, int);

__pair(float2, float);

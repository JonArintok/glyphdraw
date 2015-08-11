
float bigger(const float a, const float b) {return a > b ? a : b;}
float ternaryReduc(const float in) {return in ? (in > 0 ? 1 : -1) : 0;}
bool passedZero(const float prev, const float cur) {
  return (prev > 0 && cur <= 0) || (prev < 0 && cur >= 0);
}
class scrollable {
  float  accel;
  float  bumper;
  float2 size;
  float2 boundary;
  float2 vel;
  float2 pos;
  float2 pPos;
  float2 overBounds;
  float2 pOverBounds;
  float2 winSize;
  float2 posBR() {return pos+boundary;}
  float2 pPosBR() {return pPos+boundary;}
  float  genPos();
public:
  scrollable(
    const float  accelIn,
    const float2 sizeIn,
    const float2 winSizeIn
  ) :
    accel(accelIn), size(sizeIn), winSize(winSizeIn)
  {
    for (int i = 0; i < 2; i++) {
      *boundary.pSel(i) = bigger(size.sel(i), winSize.sel(i));
    }
    bumper = winSize.y/8;
  }
  void advance(float cursPress,float pCursPress,float2 cursPos,float2 pCursPos);
  float2 getPos() {return pos;}
  bool hasMoved() {return pos != pPos;}
};
void scrollable::advance(
  float  cursPress,
  float  pCursPress,
  float2 cursPos,
  float2 pCursPos
) {
  for (int i = 0; i < 2; i++) {
    *pOverBounds.pSel(i) = overBounds.sel(i);
    *overBounds.pSel(i) = pos.sel(i) > 0 ?
      pos.sel(i) :
      (posBR().sel(i) < winSize.sel(i) ? posBR().sel(i) - winSize.sel(i) : 0)
    ;
    *pPos.pSel(i) = pos.sel(i);
    *pos.pSel(i) = passedZero(pOverBounds.sel(i), overBounds.sel(i)) ?
      (pOverBounds.sel(i) > 0 ? 0 : winSize.sel(i) - boundary.sel(i)) :
      (pos.sel(i) > winSize.sel(i)-bumper ?
        winSize.sel(i) - bumper :
        (posBR().sel(i) < bumper ?
          bumper - boundary.sel(i) :
          pos.sel(i) + vel.sel(i)
        )
      )
    ;
    *vel.pSel(i) =
      pos.sel(i) > winSize.sel(i)-bumper ||
      posBR().sel(i) < bumper ||
      passedZero(pOverBounds.sel(i), overBounds.sel(i)) ?
      0 :
      (cursPress ?
        (pCursPress ? cursPos.sel(i) - pCursPos.sel(i) : 0) :
        vel.sel(i) + accel*ternaryReduc(overBounds.sel(i))*-1
      )
    ;
  }
}

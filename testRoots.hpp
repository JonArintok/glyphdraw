
int uintVecFromString(vector<int> &dest, const char *source) {
  int i = 0;
  while (source[i]) dest.push_back(source[i++]);
  return i;
}

void printTextBlock() {
  int i = 0;
  for (uint32_t row = 0; row < (uint32_t)UItextBlock.size.y; row++) {
    for (uint32_t col = 0; col < (uint32_t)UItextBlock.size.x; col++, i++) {
      uchar c = (uchar)UItextBlock.text[i];
      cout << c;
    }
    cout << endl;
  }
}

void buildsomeroots() {
  roots.resize(180);
  UItextBlock.size.x = 180;
  UItextBlock.size.y = roots.size();
  UItextBlock.text.resize(UItextBlock.size.pro());
  uint32_t i = 0;
  for (uint32_t row = 0; row < (uint32_t)UItextBlock.size.y; row++) {
    uint32_t col = 0;
    for (; col < (uint32_t)UItextBlock.size.x; col++, i++) {
      UItextBlock.text[i] = 32 + i%95;
    }
  }
  //printTextBlock();
}


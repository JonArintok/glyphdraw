

struct Function {
};

struct Buffer {
  vector<Buffer> *dependents;
  bool needReeval;
};
struct Variable : Buffer {
};
struct Array : Buffer {
  bool isShaded;
};

union RootUnion {
  Variable v;
  Array    a;
  Function f;
};
enum RootType {
  variable,
  array,
  function,
  rootTypeCount
};

struct intMap {
  void insert(int key, int value) {
    keys.push_back(key);
    values.push_back(value);
  }
  int find(int key) {
    for (uint32_t i = 0; i < keys.size(); i++) {
      if (key == keys[i]) return values[i];
    }
    return 0;
  }
  int size() {return keys.size();}
private:
  vector<int> keys;
  vector<int> values;
};

cl_mem glyphSheet;
SDL_Surface *gss;
glyphSheetInfo gsi;
intMap glyphSheetMap;

void initGlyphSheet(const char *name) {
  cout << "building glyphSheet from " << name << endl;
  string mapPath = name;
  string bmpPath = name;
  mapPath.append(".txt");
  bmpPath.append(".bmp");
  ifstream mapInfoFile(mapPath.c_str());
  stringstream mapInfoStream;
  mapInfoStream << mapInfoFile.rdbuf();
  gsi.glyphSize.x =
    (mapInfoStream.str()[0]-'0')*100 +
    (mapInfoStream.str()[1]-'0')*10 +
    (mapInfoStream.str()[2]-'0')
  ;
  gsi.glyphSize.y =
    (mapInfoStream.str()[4]-'0')*100 +
    (mapInfoStream.str()[5]-'0')*10 +
    (mapInfoStream.str()[6]-'0')
  ;
  gsi.glyphCount.x =
    (mapInfoStream.str()[ 8]-'0')*100 +
    (mapInfoStream.str()[ 9]-'0')*10 +
    (mapInfoStream.str()[10]-'0')
  ;
  gsi.glyphCount.y =
    (mapInfoStream.str()[12]-'0')*100 +
    (mapInfoStream.str()[13]-'0')*10 +
    (mapInfoStream.str()[14]-'0')
  ;
  int val = 0;
  for (int i = 16; mapInfoStream.str()[i] != EOF; i++) {
    if (mapInfoStream.str()[i] == '\n') continue;
    glyphSheetMap.insert(mapInfoStream.str()[i], val++);
  }
  gss = SDL_LoadBMP(bmpPath.c_str());
  checkSDLerror(__LINE__, __FILE__);
}

int intVecFromString(vector<int> &dest, const string &source) {
  uint32_t i = 0;
  for (; i < source.length(); i++) {
    dest.push_back(glyphSheetMap.find(source[i]));
  }
  return i;
}

struct textBlock {
  vector<int> text;
  int2 size;
};
textBlock UItextBlock;


struct Root {
  vector<int> text;
  int         color;
  RootType    rootType;
  RootUnion   root;
  Root() {}
  Root(const string &str) {intVecFromString(text, str);}
};
vector<Root> roots;

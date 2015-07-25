

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

int intVecFromString(vector<int> &dest, const char *source) {
  int i = 0;
  while (source[i]) dest.push_back(source[i++]-32);
  return i;
}
struct Root {
  vector<int> text;
  int         color;
  RootType    rootType;
  RootUnion   root;
  Root() {}
  Root(const char *str) {
    intVecFromString(text, str);
  }
};
vector<Root> roots;


cl_mem glyphSheet;
SDL_Surface *gss;
glyphSheetInfo gsi;

struct textBlock {
  vector<int> text;
  int2 size;
};
textBlock UItextBlock;




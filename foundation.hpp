

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
struct Root {
	string    text;
	uint32_t  color;
	RootType  rootType;
	RootUnion root;
};

vector<Root> roots;



cl_mem       glyphSheet;
SDL_Surface *gs_surface;
uint32_t gs_glyphW;
uint32_t gs_glyphH;
uint32_t gs_glyphColCount;
uint32_t gs_glyphRowCount;
uint32_t gs_unicodefirst;
uint32_t gs_unicodeLast;






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

struct Label {
	string   text;
	uint32_t color;//rgba
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
	Label     label;
	RootType  rootType;
	RootUnion root;
};

vector<Root> roots;



struct GlyphSheet {
	uint32_t charHeight;
	uint32_t charWidth;
	uint32_t unicodeBegin;
	uint32_t unicodeRange;
	GLuint   texture;
};
GlyphSheet activeGlyphSheet;


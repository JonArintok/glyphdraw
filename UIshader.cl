
typedef struct {
	uint glyphW;
	uint glyphH;
	uint colCount;
	uint rowCount;
	uint unicodeFirst;
	uint unicodeLast;
} glyphSheetInfo;


__constant sampler_t glyphSheetSampler = 
	CLK_NORMALIZED_COORDS_FALSE | 
	CLK_ADDRESS_CLAMP           | 
	CLK_FILTER_NEAREST
;

__kernel void helloPixel(
	uint textW,                               //0
	uint textH,                               //1
	__constant   uint           *text,        //2
	__constant   glyphSheetInfo *gsi,         //3
	__read_only  image2d_t       glyphSheet,  //4
	__write_only image2d_t       out          //5
) {
	const int2 dim = {get_global_size(0), get_global_size(1)};
	const int2 pos = {get_global_id(0), get_global_id(1)};
	const int2 textPos = {pos.x/(*gsi).glyphW, pos.y/(*gsi).glyphH};
	uint glyphIndex;
	if (textPos.x > textW  &&  textPos.y > textH) {
		glyphIndex = text[textPos.y*textW + textPos.x];
	}
	float4 color = read_imagef(glyphSheet, glyphSheetSampler, pos);//xyzw : abgr
	write_imagef(out, pos, color);
}

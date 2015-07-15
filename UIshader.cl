
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
	__write_only image2d_t       out,         //5
	__global     int            *inspect      //6
) {
	const int2 dim = {get_global_size(0), get_global_size(1)};
	const int2 pos = {get_global_id(0), get_global_id(1)};
	const int2 textPos = {pos.x/gsi->glyphW, pos.y/gsi->glyphH};
	const int textPosInBounds = textPos.x < textW && textPos.y < textH;
	int glyphIndex = 0;
	if (textPosInBounds) {
		glyphIndex = text[(textPos.y * textW + textPos.x)] - gsi->unicodeFirst;
	}
	
	const int glyphIndexInBounds = glyphIndex < gsi->colCount * gsi->rowCount;
	const int2 glyphSheetPos = {
		(glyphIndex % gsi->colCount) * glyphIndexInBounds, 
		(glyphIndex / gsi->colCount) * glyphIndexInBounds
	};
	
	//const int linearPos = pos.y * dim.x + pos.x;
	//inspect[linearPos] = glyphSheetPos.y;
	
	const int2 glyphSheetPixPos = {
		(glyphSheetPos.x * gsi->glyphW) + (pos.x % gsi->glyphW),
		(glyphSheetPos.y * gsi->glyphH) + (pos.y % gsi->glyphH)
	};
	float4 color = read_imagef(glyphSheet, glyphSheetSampler, glyphSheetPixPos);
	write_imagef(out, pos, color);
}

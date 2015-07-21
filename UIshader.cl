
#define backColor (float4)(0.5, 0, 0, 1)

__constant sampler_t glyphSheetSampler = 
	CLK_NORMALIZED_COORDS_FALSE | 
	CLK_ADDRESS_CLAMP           | 
	CLK_FILTER_NEAREST
;

__kernel void UIshader(
	private    int2            offset,      //0
	private    int2            textSize,    //1
	constant   int            *text,        //2
	constant   glyphSheetInfo *gsi,         //3
	read_only  image2d_t       glyphSheet,  //4
	write_only image2d_t       out          //5
#if kernelInspectArgIndex
	, global     int            *inspect      //6
#endif
) {
	const int2 pos = {get_global_id(0), get_global_id(1)};
	const int2 offsetPos = pos-offset;
	const int2 textPos = offsetPos/gsi->glyphSize;
	if (
		offsetPos.x < 0  ||  offsetPos.y < 0  ||
		textPos.x >= textSize.x  ||  textPos.y >= textSize.y
	) {
		write_imagef(out, pos, backColor);
		return;
	}
	const int glyphIndex = text[(textPos.y * textSize.x + textPos.x)] - gsi->unicodeFirst;
	const int glyphIndexInBounds = glyphIndex < gsi->glyphCount.x * gsi->glyphCount.y;
	const int2 glyphSheetPos = glyphIndexInBounds * (int2)(
		glyphIndex % gsi->glyphCount.x, 
		glyphIndex / gsi->glyphCount.x
	);
	const int2 glyphSheetPixPos = (glyphSheetPos * gsi->glyphSize) + (offsetPos % gsi->glyphSize);
	float4 color = read_imagef(glyphSheet, glyphSheetSampler, glyphSheetPixPos);
	write_imagef(out, pos, color);
#if kernelInspectArgIndex
	const int2 dim = {get_global_size(0), get_global_size(1)};
	const int linearPos = pos.y * dim.x + pos.x;
	inspect[linearPos] = glyphSheetPos.y;
#endif
}

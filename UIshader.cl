
#define backColor (float4)(0.5, 0, 0, 1)

__constant sampler_t glyphSheetSampler = 
	CLK_NORMALIZED_COORDS_FALSE | 
	CLK_ADDRESS_CLAMP           | 
	CLK_FILTER_NEAREST
;

__kernel void UIshader(
	private    uint2           textSize,    //0
	constant   uint           *text,        //1
	constant   glyphSheetInfo *gsi,         //2
	read_only  image2d_t       glyphSheet,  //3
	write_only image2d_t       out          //4
#if kernelInspectArgIndex
	, global     int            *inspect      //5
#endif
) {
	const int2 dim = {get_global_size(0), get_global_size(1)};
	const int2 pos = {get_global_id(0), get_global_id(1)};
	const int2 textPos = {pos.x/gsi->glyphSize.x, pos.y/gsi->glyphSize.y};
	if (textPos.x >= textSize.x  ||  textPos.y >= textSize.y) {
		write_imagef(out, pos, backColor);
		return;
	}
	const int glyphIndex = text[(textPos.y * textSize.x + textPos.x)] - gsi->unicodeFirst;
	
	const int glyphIndexInBounds = glyphIndex < gsi->glyphCount.x * gsi->glyphCount.y;
	const int2 glyphSheetPos = {
		(glyphIndex % gsi->glyphCount.x) * glyphIndexInBounds, 
		(glyphIndex / gsi->glyphCount.x) * glyphIndexInBounds
	};
	
#if kernelInspectArgIndex
	const int linearPos = pos.y * dim.x + pos.x;
	inspect[linearPos] = glyphSheetPos.y;
#endif
	
	const int2 glyphSheetPixPos = {
		(glyphSheetPos.x * gsi->glyphSize.x) + (pos.x % gsi->glyphSize.x),
		(glyphSheetPos.y * gsi->glyphSize.y) + (pos.y % gsi->glyphSize.y)
	};
	float4 color = read_imagef(glyphSheet, glyphSheetSampler, glyphSheetPixPos);
	write_imagef(out, pos, color);
}

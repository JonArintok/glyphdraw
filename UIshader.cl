
__constant sampler_t imageSampler = 
	CLK_NORMALIZED_COORDS_FALSE | 
	CLK_ADDRESS_CLAMP           | 
	CLK_FILTER_NEAREST
;

__kernel void helloPixel(
	float curFrame,
	__read_only  image2d_t glyphSheet,
	__write_only image2d_t out
) {
	const int2 pos = {get_global_id(0), get_global_id(1)};
	const int2 dim = {get_global_size(0), get_global_size(1)};
	//xyzw : abgr
	float4 color = read_imagef(glyphSheet, imageSampler, pos);
	write_imagef(out, pos, color);
}

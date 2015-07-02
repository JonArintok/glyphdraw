

void buildsomeroots() {
	roots.resize(3);
	roots[0].text = "foo";
	roots[1].text = "bar baz";
	roots[2].text = "let me drive my van into your heart";
	roots[0].color = 0xff0000ff;
	roots[1].color = 0x00ff00ff;
	roots[2].color = 0x0000ffff;
}


const uint vertsPerGlyph = 6;
int buildVerts(vector<float> &v) {
	uint vsize = 0;
	for (uint ri = 0; ri < roots.size(); ri++) {
		vsize += roots[ri].text.length()*vertsPerGlyph*labelVertAttribCount;
	}
	v.resize(vsize);
	
	uint vi = 0;
	for (uint ri = 0; ri < roots.size(); ri++) {
		uint wordLength = roots[ri].text.length();
		for (uint ci = 0; ci < wordLength; ci++) {
			for (
				uint gvi = 0;
				gvi < vertsPerGlyph;
				gvi++, vi += labelVertAttribCount
			) {
				float glyphU = gvi==0||gvi==1||gvi==4 ? 0 : 1;
				float glyphV = gvi==0||gvi==2||gvi==3 ? 0 : 1;
				v[vi+lva_x] = ci + glyphU;
				v[vi+lva_y] = ri + glyphV;
				v[vi+lva_z] = uil_rootLabels;
				v[vi+lva_r] = ((roots[ri].color>>24)&0x000000ff)/255.0;
				v[vi+lva_g] = ((roots[ri].color>>16)&0x000000ff)/255.0;
				v[vi+lva_b] = ((roots[ri].color>> 8)&0x000000ff)/255.0;
				v[vi+lva_glyphU] = glyphU;
				v[vi+lva_glyphV] = glyphV;
				v[vi+lva_wordU ] = (gvi + glyphU) / wordLength;
				v[vi+lva_glyphI] = roots[ri].text[ci];
			}
		}
	}
	
	return vsize;
}


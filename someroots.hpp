

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
int buildVerts(vector<float> v) {
	{
		uint vsize = 0;
		for (uint ri = 0; ri < roots.size(); ri++) {
			vsize += roots[ri].text.length()*vertsPerGlyph*labelVertAttribCount;
		}
		v.resize(vsize);
	}
	uint vi = 0;
	for (uint ri = 0; ri < roots.size(); ri++) {
		uint wordLength = roots[ri].text.length();
		for (uint ci = 0; ci < wordLength; ci++) {
			for (
				uint gvi = 0;
				gvi < vertsPerGlyph;
				gvi++, vi += labelVertAttribCount
			) {
				float glyphX = gvi % 2;
				float glyphY = gvi==2||gvi==4||gvi==5 ? 1 : 0;
				v[vi+lva_x         ] = gvi + glyphX;
				v[vi+lva_y         ] = ri  + glyphY;
				v[vi+lva_z         ] = uil_rootLabels;
				v[vi+lva_glyphX    ] = glyphX;
				v[vi+lva_glyphY    ] = glyphY;
				v[vi+lva_wordX     ] = (gvi + glyphX) / wordLength;
				v[vi+lva_glyphIndex] = roots[ri].text[ci];
				v[vi+lva_objIndex  ] = ri;
			}
		}
	}
	return v.size();
}


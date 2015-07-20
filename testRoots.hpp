
int uintVecFromString(vector<uint> &dest, const char *source) {
	uint i = 0;
	while (source[i]) dest.push_back(source[i++]);
	return i;
}
/*
void printTextBlock() {
	uint i = 0;
	for (uint row = 0; row < UItextBlock.size.y; row++) {
		for (uint col = 0; col < UItextBlock.size.x; col++, i++) {
			uchar c = (uchar)UItextBlock.text[i];
			cout << c;
		}
		cout << endl;
	}
}
*/
void buildsomeroots() {
	roots.resize(3);
	uintVecFromString(roots[0].text, "foo bar baz");
	uintVecFromString(
		roots[1].text, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"
	);
	uintVecFromString(
		roots[2].text, "Let me drive my van into your heart! <3 <3 <3"
	);
	roots[0].color = 0xff0000ff;
	roots[1].color = 0x00ff00ff;
	roots[2].color = 0x0000ffff;
	
	UItextBlock.size.x = 0;
	UItextBlock.size.y = roots.size();
	for (uint i = 0; i < roots.size(); i++) {
		if (roots[i].text.size() > UItextBlock.size.x) {
			UItextBlock.size.x = roots[i].text.size();
		}
	}
	UItextBlock.text.resize(UItextBlock.size.x * UItextBlock.size.y);
	uint i = 0;
	for (uint row = 0; row < roots.size(); row++) {
		uint col = 0;
		for (; col < roots[row].text.size(); col++, i++) {
			if (
				roots[row].text[col] < gsi.unicodeFirst || 
				roots[row].text[col] > gsi.unicodeLast
			) {
				cout << "warning: the character '" << roots[row].text[col]
				<< "' is not on the glyphSheet" << endl;
			}
			UItextBlock.text[i] = roots[row].text[col];
		}
		for (; col < UItextBlock.size.x; col++, i++) {
			UItextBlock.text[i] = ' ';
		}
	}
	//printTextBlock();
}


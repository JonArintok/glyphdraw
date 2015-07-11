

void buildsomeroots() {
	roots.resize(3);
	roots[0].text = "foo bar baz";
	roots[1].text = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	roots[2].text = "Let me drive my van into your heart! <3 <3 <3";
	roots[0].color = 0xff0000ff;
	roots[1].color = 0x00ff00ff;
	roots[2].color = 0x0000ffff;
	
	memset(&UItextBlock, 0, sizeof(textBlock));
	UItextBlock.h = roots.size();
	for (uint i = 0; i < roots.size(); i++) {
		if (roots[i].text.size() > UItextBlock.w) {
			UItextBlock.w = roots[i].text.size();
		}
	}
	//this doesn't actually support unicode yet...
	UItextBlock.text = new char[UItextBlock.w * UItextBlock.h];
	for (uint row = 0; row < roots.size(); row++) {
		uint col = 0;
		uint UItc = row*UItextBlock.w + col;
		for (; col < roots[row].text.size(); col++) {
			if (
				(uint)roots[row].text[col] < gsi.unicodeFirst || 
				(uint)roots[row].text[col] < gsi.unicodeLast
			) {
				cout << "warning: the character '" << roots[row].text[col]
				<< "' is not on the glyphSheet" << endl;
			}
			UItextBlock.text[UItc] = roots[row].text[col];
		}
		for (; col < UItextBlock.w; col++) {
			UItextBlock.text[UItc] = ' ';
		}
	}
}




void buildsomeroots() {
	roots.resize(3);
	roots[0].label.text = "foo";
	roots[1].label.text = "bar baz";
	roots[2].label.text = "let me drive my van into your heart";
	roots[0].label.color = 0xff0000ff;
	roots[1].label.color = 0x00ff00ff;
	roots[2].label.color = 0x0000ffff;
}



int buildVertices(vector<float> vertices) {
	for (uint i = 0; i < roots.size(); i += labelVertAttribCount) {
		
	}
	return vertices.size();
}


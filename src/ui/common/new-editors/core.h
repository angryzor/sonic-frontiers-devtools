#pragma once

struct Test {
	int a;
	int b;
};

bool Inspector(Test& t) {
	f(t.a, true);
	f(t.a, false);
}

bool Editor(Test& t) {

}

void Viewer(const Test& t) {
	Inspector(t);
}
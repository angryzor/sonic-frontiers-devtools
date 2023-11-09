#pragma once

struct Theme {
	const char* name;
	void (*Load)();

	static Theme themes[3];
};

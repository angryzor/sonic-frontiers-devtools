#pragma once

struct Theme {
	const char* name;
	void (*Load)();

	static constexpr size_t themeCount{ 5 };
	static Theme themes[themeCount];
};

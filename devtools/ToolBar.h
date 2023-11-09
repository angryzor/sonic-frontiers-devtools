#pragma once

class Desktop;
class ToolBar
{
public:
	static bool IsPaused();
	static void Render(Desktop* desktop);
};


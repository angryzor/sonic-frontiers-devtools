#pragma once

class Context
{
public:
    static bool visible;
    static bool passThroughMouse;
    static bool inited;
    static bool alreadyRendering;

    static void update();
    static void install_hooks();
    static void init();
};

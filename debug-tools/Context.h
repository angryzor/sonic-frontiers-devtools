#pragma once
#pragma once

class Context
{
public:
    static bool visible;
    static bool passThroughMouse;
    static bool inited;

    static void update();
    static void install_hooks();
    static void init();
};

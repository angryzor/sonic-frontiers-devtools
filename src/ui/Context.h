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
    static void deinit();

    static void init_modules();
    static void deinit_modules();

    static void init_imgui();
    static void deinit_imgui();
};

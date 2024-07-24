#pragma once

class Context
{
    static bool enableViewports;
    static bool reinitImGuiNextFrame;
public:
    static bool visible;
    static bool passThroughMouse;
    static bool inited;
    static bool imguiInited;

    static void update();
    static void install_hooks();
    static void init();
    static void deinit();

    static void init_modules();
    static void deinit_modules();

    static void init_imgui();
    static void deinit_imgui();

    static void set_enable_viewports(bool enable);
};

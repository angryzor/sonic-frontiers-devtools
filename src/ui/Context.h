#pragma once

class Context
{
    static bool enableViewports;
    static bool reinitImGuiNextFrame;
    static bool rebuildFontsNextFrame;
    static float fontSize;
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

    static void reinit_imgui();
    static void rebuild_fonts();

    static void set_font_size(float size);
    static void set_enable_viewports(bool enable);
    static bool should_enable_viewports_considering_fullscreen(hh::needle::DisplaySwapDevice* swapDevice);
};

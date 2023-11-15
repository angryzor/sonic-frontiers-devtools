#pragma once
#include "imgui/imgui_internal.h"
#include "common/Translations.h"
#include "common/Theme.h"

class SettingsManager
{
    struct Settings {
        unsigned int theme;
        Translations::Language language;
        float fontSize;
        ImGuiConfigFlags configFlags;

        bool operator==(const Settings& other) const;
        bool operator!=(const Settings& other) const;
    };

    static ImGuiSettingsHandler settingsHandler;

    static bool showConfigDialog;

    Settings SerializeSettings();
    void LoadSerializedSettings(const Settings& settings);
    static void ClearAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    static void ReadInitFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    static void* ReadOpenFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name);
    static void ReadLineFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line);
    static void ApplyAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler);
    static void WriteAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf);

    static void ApplySettings();

public:
    static Settings settings;
    static void Init();
    static void Render();
    static void OpenConfigDialog();
};


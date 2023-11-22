#pragma once
#include "imgui/imgui_internal.h"
#include "common/Translations.h"
#include "common/Theme.h"

class SettingsManager
{
    struct Settings {
        unsigned int theme{ 0 };
        Translations::Language language{ Translations::Language::VAR_NAMES };
        float fontSize{ 14 };
        ImGuiConfigFlags configFlags{ ImGuiConfigFlags_NavEnableKeyboard };
        float physicsPickerRayLength{ 2000.0f };
        unsigned int physicsPickerSelectionMask{ 0xFDFFFFFF };
        float debugCameraMouseSensitivityX{ 0.05f };
        float debugCameraMouseSensitivityY{ 0.05f };

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


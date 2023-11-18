#include "Pch.h"
#include "SettingsManager.h"

ImGuiSettingsHandler SettingsManager::settingsHandler{};
SettingsManager::Settings SettingsManager::settings{ 0, Translations::Language::VAR_NAMES, 14, ImGuiConfigFlags_NavEnableKeyboard, 0xFDFFFFFF };
bool SettingsManager::showConfigDialog{ false };

bool SettingsManager::Settings::operator==(const SettingsManager::Settings& other) const {
	return theme == other.theme && language == other.language && fontSize == other.fontSize && configFlags == other.configFlags && physicsPickerSelectionMask == other.physicsPickerSelectionMask;
}

bool SettingsManager::Settings::operator!=(const SettingsManager::Settings& other) const {
	return !operator==(other);
}

void SettingsManager::Init() {
	settingsHandler.TypeName = "DevTools";
	settingsHandler.TypeHash = ImHashStr(settingsHandler.TypeName);
	settingsHandler.ClearAllFn = ClearAllFn;
	settingsHandler.ReadInitFn = ReadInitFn;
	settingsHandler.ReadOpenFn = ReadOpenFn;
	settingsHandler.ReadLineFn = ReadLineFn;
	settingsHandler.ApplyAllFn = ApplyAllFn;
	settingsHandler.WriteAllFn = WriteAllFn;
	settingsHandler.UserData = &settings;
	ImGui::AddSettingsHandler(&settingsHandler);

	// We apply the defaults on startup in case the user doesn't have a config yet.
	ApplySettings();
}

void SettingsManager::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
	if (showConfigDialog) {
		if (ImGui::Begin("Settings", &showConfigDialog, windowFlags)) {
			Settings tempSettings{ settings };

			if (ImGui::BeginTabBar("SettingsTabs")) {
				if (ImGui::BeginTabItem("UI")) {
					const char* themeComboStr = Theme::themes[tempSettings.theme].name;
					if (ImGui::BeginCombo("Theme", themeComboStr)) {
						for (unsigned int i = 0; i < 3; i++) {
							bool is_selected = tempSettings.theme == i;

							if (ImGui::Selectable(Theme::themes[i].name, is_selected)) {
								tempSettings.theme = i;
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					//ImGui::DragFloat("Font size", &tempSettings.fontSize, 1.0f, 6.0f, 30.0f);

					const char* languageComboStr = Translations::GetLanguageName(Translations::GetCurrentLanguage());
					if (ImGui::BeginCombo("Reflections description language", languageComboStr)) {
						for (size_t i = 0; i < 3; i++) {
							Translations::Language language = static_cast<Translations::Language>(i);
							bool is_selected = tempSettings.language == language;

							if (ImGui::Selectable(Translations::GetLanguageName(language), is_selected)) {
								tempSettings.language = language;
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					ImGui::CheckboxFlags("Allow keyboard navigation", &tempSettings.configFlags, ImGuiConfigFlags_NavEnableKeyboard);
					ImGui::CheckboxFlags("Allow gamepad navigation", &tempSettings.configFlags, ImGuiConfigFlags_NavEnableGamepad);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Selection")) {
					ImGui::SeparatorText("Physics ray cast collision mask");

					for (unsigned int i = 0; i < 32; i++) {
						char label[20];
						snprintf(label, sizeof(label), "Bit %d", i);
						ImGui::CheckboxFlags(label, &tempSettings.physicsPickerSelectionMask, 1 << i);
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			if (tempSettings != settings) {
				settings = tempSettings;
				ApplySettings();
				ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);
			}
		}
		ImGui::End();
	}
}

void SettingsManager::OpenConfigDialog() {
	showConfigDialog = true;
}

void SettingsManager::ApplySettings() {
	Theme::themes[settings.theme].Load();
	Translations::SetCurrentLanguage(settings.language);
	ImGui::GetIO().ConfigFlags = settings.configFlags;
	*rangerssdk::GetAddress(&hh::physics::PhysicsMousePickingViewer::selectionMask) = settings.physicsPickerSelectionMask;
}

void SettingsManager::ClearAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
	settings.theme = 0;
	settings.language = Translations::Language::VAR_NAMES;
	//settings.fontSize = 14;
	settings.configFlags = ImGuiConfigFlags_NavEnableKeyboard;
	settings.physicsPickerSelectionMask = 0xFDFFFFFF;
}

void SettingsManager::ReadInitFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
}

void* SettingsManager::ReadOpenFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name)
{
	return &settings;
}

void SettingsManager::ReadLineFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line)
{
	unsigned int i;
	//float f;
	if (sscanf_s(line, "Theme=%i", &i) == 1) { settings.theme = i; }
	else if (sscanf_s(line, "Translations=%i", &i) == 1) { settings.language = static_cast<Translations::Language>(i); }
	//else if (sscanf_s(line, "FontSize=%f", &f) == 1) { settings.fontSize = f; }
	else if (sscanf_s(line, "ConfigFlags=%i", &i) == 1) { settings.configFlags = i; }
	else if (sscanf_s(line, "PhysicsPickerSelectionMask=%i", &i) == 1) { settings.physicsPickerSelectionMask = i; }
}

void SettingsManager::ApplyAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
	ApplySettings();
}

void SettingsManager::WriteAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf)
{
	//buf->reserve(buf->size() + sizeof(Settings) * 6);
	out_buf->appendf("[DevTools][]\n");
	out_buf->appendf("Theme=%i\n", settings.theme);
	out_buf->appendf("Translations=%i\n", settings.language);
	//out_buf->appendf("FontSize=%f\n", settings.fontSize);
	out_buf->appendf("ConfigFlags=%i\n", settings.configFlags);
	out_buf->appendf("PhysicsPickerSelectionMask=%i\n", settings.physicsPickerSelectionMask);
}

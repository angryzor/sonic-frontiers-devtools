#include "SettingsManager.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/operation-modes/LevelEditor/LevelEditor.h>
#include <ui/Desktop.h>
#include <ui/input/Input.h>
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>

ImGuiSettingsHandler SettingsManager::settingsHandler{};
SettingsManager::Settings SettingsManager::settings{};
bool SettingsManager::showConfigDialog{ false };

bool SettingsManager::Settings::operator==(const SettingsManager::Settings& other) const {
	bool equal = theme == other.theme
		&& language == other.language
		&& fontSize == other.fontSize
		&& configFlags == other.configFlags
		&& debugCameraMouseSensitivityX == other.debugCameraMouseSensitivityX
		&& debugCameraMouseSensitivityY == other.debugCameraMouseSensitivityY
		&& rflDefaultFloatStep == other.rflDefaultFloatStep
		&& rflMinFloatStep == other.rflMinFloatStep
		&& rflSliderCutOffRange == other.rflSliderCutOffRange
		&& debugRenderingRenderGOCVisualDebugDraw == other.debugRenderingRenderGOCVisualDebugDraw
		&& debugRenderingRenderColliders == other.debugRenderingRenderColliders
		&& debugRenderingRenderOcclusionCapsules == other.debugRenderingRenderOcclusionCapsules
		&& debugRenderingGOCVisualDebugDrawOpacity == other.debugRenderingGOCVisualDebugDrawOpacity
		&& debugRenderingLevelEditorDebugBoxScale == other.debugRenderingLevelEditorDebugBoxScale;

	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			equal = equal && debugRenderingColliderFilters[i][j] == other.debugRenderingColliderFilters[i][j];

	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			equal = equal && selectionColliderFilters[i][j] == other.selectionColliderFilters[i][j];

	return equal;
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

	ImGui::SetNextWindowSize(ImVec2(1400, 1000), ImGuiCond_Once);
	if (showConfigDialog) {
		if (ImGui::Begin("Settings", &showConfigDialog, windowFlags)) {
			Settings tempSettings{ settings };

			if (ImGui::BeginTabBar("SettingsTabs")) {
				if (ImGui::BeginTabItem("UI")) {
					const char* themeComboStr = Theme::themes[tempSettings.theme].name;
					if (ImGui::BeginCombo("Theme", themeComboStr)) {
						for (unsigned int i = 0; i < Theme::themeCount; i++) {
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
						for (unsigned int i = 0; i < 3; i++) {
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
					ImGui::SeparatorText("Reflection editor");
					ImGui::DragFloat("Default step size for float values", &tempSettings.rflDefaultFloatStep, 0.001f);
					ImGui::SetItemTooltip("Default precision for float values if the game does not specify a precision.");
					ImGui::DragFloat("Minimum step size for float values", &tempSettings.rflMinFloatStep, 0.01f);
					ImGui::SetItemTooltip("The drag control's precision for float values will be capped to this value if the game specifies a smaller precision. Intended to correct for the game often specifying the excessively precise value 0.001.");
					ImGui::DragScalar("Slider cut-off range", ImGuiDataType_U32, &tempSettings.rflSliderCutOffRange, 1.0f);
					ImGui::SetItemTooltip("Range between minimum and maximum value at which the editor will choose to use drag controls instead of sliders.");
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Camera")) {
					ImGui::DragFloat("Debug camera mouse horizontal sensitivity", &tempSettings.debugCameraMouseSensitivityX, 0.001f);
					ImGui::DragFloat("Debug camera mouse vertical sensitivity", &tempSettings.debugCameraMouseSensitivityY, 0.001f);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Selection")) {
					if (ImGui::BeginTabBar("Selection tabs")) {
						if (ImGui::BeginTabItem("Selection collider filters")) {
							if (ImGui::BeginTable("Filters", 33))
							{
								ImGui::TableSetupColumn("Filter categories", ImGuiTableColumnFlags_WidthStretch);
								for (size_t j = 0; j < 32; j++) {
									char header[10];
									snprintf(header, 10, "%zd", j);
									ImGui::TableSetupColumn(header, ImGuiTableColumnFlags_WidthFixed, 24.0f);
								}

								ImGui::TableHeadersRow();

								for (size_t i = 0; i < 32; i++)
								{
									ImGui::PushID(i);
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::Text("Layer %zd", i);

									for (size_t j = 0; j < 32; j++) {
										ImGui::PushID(j);
										ImGui::TableNextColumn();
										ImGui::Checkbox("", &tempSettings.selectionColliderFilters[i][j]);
										ImGui::PopID();
									}

									ImGui::PopID();
								}
								ImGui::EndTable();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Debug rendering")) {
					if (ImGui::BeginTabBar("Debug rendering tabs")) {
						if (ImGui::BeginTabItem("General options")) {
							constexpr uint8_t minAlpha{ 0 };
							constexpr uint8_t maxAlpha{ 255 };
							ImGui::Checkbox("Render GOCVisualDebugDraw visuals", &tempSettings.debugRenderingRenderGOCVisualDebugDraw);
							ImGui::Checkbox("Render colliders", &tempSettings.debugRenderingRenderColliders);
							ImGui::Checkbox("Render occlusion capsules", &tempSettings.debugRenderingRenderOcclusionCapsules);
							ImGui::SliderScalar("GOCVisualDebugDraw opacity (requires stage restart)", ImGuiDataType_U8, &tempSettings.debugRenderingGOCVisualDebugDrawOpacity, &minAlpha, &maxAlpha);
							ImGui::DragFloat("Level editor debug box scale", &tempSettings.debugRenderingLevelEditorDebugBoxScale, 0.05f);
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Collider rendering filters")) {
							if (ImGui::BeginTable("Filters", 33))
							{
								ImGui::TableSetupColumn("Filter categories", ImGuiTableColumnFlags_WidthStretch);
								for (size_t j = 0; j < 32; j++) {
									char header[10];
									snprintf(header, 10, "%zd", j);
									ImGui::TableSetupColumn(header, ImGuiTableColumnFlags_WidthFixed, 24.0f);
								}

								ImGui::TableHeadersRow();

								for (size_t i = 0; i < 32; i++)
								{
									ImGui::PushID(i);
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::Text("Layer %zd", i);

									for (size_t j = 0; j < 32; j++) {
										ImGui::PushID(j);
										ImGui::TableNextColumn();
										ImGui::Checkbox("", &tempSettings.debugRenderingColliderFilters[i][j]);
										ImGui::PopID();
									}

									ImGui::PopID();
								}
								ImGui::EndTable();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
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
	ImGui::GetIO().ConfigFlags = settings.configFlags | ImGuiConfigFlags_DockingEnable;
	defaultFloatStep = settings.rflDefaultFloatStep;
	rflMinFloatStep = settings.rflMinFloatStep;
	rflSliderCutOff = settings.rflSliderCutOffRange;
	GOCVisualDebugDrawRenderer::renderGOCVisualDebugDraw = settings.debugRenderingRenderGOCVisualDebugDraw;
	GOCVisualDebugDrawRenderer::renderColliders = settings.debugRenderingRenderColliders;
	GOCVisualDebugDrawRenderer::renderOcclusionCapsules = settings.debugRenderingRenderOcclusionCapsules;
	GOCVisualDebugDrawRenderer::gocVisualDebugDrawOpacity = settings.debugRenderingGOCVisualDebugDrawOpacity;
	LevelEditor::SetDebugBoxScale(settings.debugRenderingLevelEditorDebugBoxScale);

	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			Desktop::selectionColliderFilters[i][j] = settings.selectionColliderFilters[i][j];
	
	for (size_t i = 0; i < 32; i++)
		for (size_t j = 0; j < 32; j++)
			GOCVisualDebugDrawRenderer::colliderFilters[i][j] = settings.debugRenderingColliderFilters[i][j];

	//ReloadInputSettings();
}

void SettingsManager::ClearAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
	settings = SettingsManager::Settings{};
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
	unsigned int u;
	float f;
	char s[300];
	char s2[10];
	if (sscanf_s(line, "Theme=%u", &u) == 1) { settings.theme = u; return; }
	if (sscanf_s(line, "Translations=%u", &u) == 1) { settings.language = static_cast<Translations::Language>(u); return; }
	// if (sscanf_s(line, "FontSize=%f", &f) == 1) { settings.fontSize = f; return; }
	if (sscanf_s(line, "ConfigFlags=%u", &u) == 1) { settings.configFlags = u; return; }
	if (sscanf_s(line, "DebugCameraMouseSensitivityX=%f", &f) == 1) { settings.debugCameraMouseSensitivityX = f; return; }
	if (sscanf_s(line, "DebugCameraMouseSensitivityY=%f", &f) == 1) { settings.debugCameraMouseSensitivityY = f; return; }
	if (sscanf_s(line, "DefaultFloatStep=%f", &f) == 1) { settings.rflDefaultFloatStep = f; return; }
	if (sscanf_s(line, "MinFloatStep=%f", &f) == 1) { settings.rflMinFloatStep = f; return; }
	if (sscanf_s(line, "SliderCutOffRange=%u", &u) == 1) { settings.rflSliderCutOffRange = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderGOCVisualDebugDraw=%u", &u) == 1) { settings.debugRenderingRenderGOCVisualDebugDraw = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderColliders=%u", &u) == 1) { settings.debugRenderingRenderColliders = u; return; }
	if (sscanf_s(line, "DebugRenderingRenderOcclusionCapsules=%u", &u) == 1) { settings.debugRenderingRenderOcclusionCapsules = u; return; }
	if (sscanf_s(line, "DebugRenderingGOCVisualDebugDrawOpacity=%u", &u) == 1) { settings.debugRenderingGOCVisualDebugDrawOpacity = u; return; }
	if (sscanf_s(line, "DebugRenderingLevelEditorDebugBoxScale=%f", &f) == 1) { settings.debugRenderingLevelEditorDebugBoxScale = f; return; }
	if (sscanf_s(line, "SelectionColliderFilters=%256s", s, 300) == 1 && strlen(s) == 256) {
		for (size_t i = 0; i < 32; i++) {
			sscanf_s(s + i * 8, "%8x", &u);
			for (size_t j = 0; j < 32; j++) {
				settings.selectionColliderFilters[i][j] = u & (1 << j);
			}
		}
		return;
	}
	if (sscanf_s(line, "DebugRenderingColliderFilters=%256s", s, 300) == 1 && strlen(s) == 256) {
		for (size_t i = 0; i < 32; i++) {
			sscanf_s(s + i * 8, "%8x", &u);
			for (size_t j = 0; j < 32; j++) {
				settings.debugRenderingColliderFilters[i][j] = u & (1 << j);
			}
		}
		return;
	}
}

void SettingsManager::ApplyAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler)
{
	ApplySettings();
}

void SettingsManager::WriteAllFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf)
{
	//buf->reserve(buf->size() + sizeof(Settings) * 6);
	out_buf->appendf("[DevTools][]\n");
	out_buf->appendf("Theme=%u\n", settings.theme);
	out_buf->appendf("Translations=%u\n", settings.language);
	//out_buf->appendf("FontSize=%f\n", settings.fontSize);
	out_buf->appendf("ConfigFlags=%u\n", settings.configFlags);
	out_buf->appendf("DebugCameraMouseSensitivityX=%f\n", settings.debugCameraMouseSensitivityX);
	out_buf->appendf("DebugCameraMouseSensitivityY=%f\n", settings.debugCameraMouseSensitivityY);
	out_buf->appendf("DefaultFloatStep=%f\n", settings.rflDefaultFloatStep);
	out_buf->appendf("MinFloatStep=%f\n", settings.rflMinFloatStep);
	out_buf->appendf("SliderCutOffRange=%u\n", settings.rflSliderCutOffRange);
	out_buf->appendf("DebugRenderingRenderGOCVisualDebugDraw=%u\n", settings.debugRenderingRenderGOCVisualDebugDraw);
	out_buf->appendf("DebugRenderingRenderColliders=%u\n", settings.debugRenderingRenderColliders);
	out_buf->appendf("DebugRenderingRenderOcclusionCapsules=%u\n", settings.debugRenderingRenderOcclusionCapsules);
	out_buf->appendf("DebugRenderingGOCVisualDebugDrawOpacity=%u\n", settings.debugRenderingGOCVisualDebugDrawOpacity);
	out_buf->appendf("DebugRenderingLevelEditorDebugBoxScale=%f\n", settings.debugRenderingLevelEditorDebugBoxScale);

	out_buf->appendf("SelectionColliderFilters=");
	for (size_t i = 0; i < 32; i++) {
		unsigned int u{ 0 };

		for (size_t j = 0; j < 32; j++)
			if (settings.selectionColliderFilters[i][j])
				u |= 1 << j;

		out_buf->appendf("%08x", u);
	}
	out_buf->appendf("\n");

	out_buf->appendf("DebugRenderingColliderFilters=");
	for (size_t i = 0; i < 32; i++) {
		unsigned int u{ 0 };

		for (size_t j = 0; j < 32; j++)
			if (settings.debugRenderingColliderFilters[i][j])
				u |= 1 << j;

		out_buf->appendf("%08x", u);
	}
	out_buf->appendf("\n");
}

#include "GameModeInspector.h"
#include <imgui_internal.h>
#include <ui/Desktop.h>
#include <ui/resources/editors/ResObjectWorldEditor.h>
#include <ui/common/ObjectDataEditor.h>
#include <ui/common/SimpleWidgets.h>

using namespace hh::game;

GameModeInspector::GameModeInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator } {
	SetTitle("GameMode inspector");
}

void GameModeInspector::RenderContents() {
	auto* seqExt = static_cast<app::MyApplication*>(app::MyApplication::GetInstance())->GetExtension<app::game::ApplicationSequenceExtension>();
	if (!seqExt)
		return;

	auto* gameMode = seqExt->GetCurrentGameMode();
	if (!gameMode)
		return;

	ImGui::SeparatorText("Extensions");
	for (auto* extension : gameMode->extensions) {
		if (extension->GetNameHash() == 0x42983F51) {
			auto* ext = static_cast<app::game::GameModeLayerStatusExtension*>(extension);

			if (ImGui::CollapsingHeader("Layer Status")) {
				size_t i;
				ImGui::SeparatorText("Active layers during normal operation override stack");
				i = 0;
				for (auto& d : ext->layersActiveDuringNormalOperation) {
					if (ImGui::TreeNodeEx(&d, ImGuiTreeNodeFlags_None, "Stack item %d", i++)) {
						for (size_t j = 0; j < 32; j++) {
							char name[20];
							snprintf(name, sizeof(name), "Layer %zd", j);
							
							ImGui::CheckboxFlags(name, &d.unk3, 1 << j);
						}
						ImGui::TreePop();
					}
				}

				ImGui::SeparatorText("Active layers during ingame pause override stack");
				i = 0;
				for (auto& d : ext->layersActiveDuringIngamePause) {
					if (ImGui::TreeNodeEx(&d, ImGuiTreeNodeFlags_None, "Stack item %d", i++)) {
						for (size_t j = 0; j < 32; j++) {
							char name[20];
							snprintf(name, sizeof(name), "Layer %zd", j);

							ImGui::CheckboxFlags(name, &d.unk3, 1 << j);
						}
						ImGui::TreePop();
					}
				}

				ImGui::SeparatorText("Global timescale interpolator stack");
				ImGui::DragFloat("Timescale", &ext->globalInterpolator.timeScale);
				i = 0;
				for (auto& d : ext->globalInterpolator.unk1) {
					if (ImGui::TreeNodeEx(&d, ImGuiTreeNodeFlags_None, "Interpolator %d", i++)) {
						ImGui::DragFloat("Timescale", &d.timeScale, 0.01f);
						ImGui::DragFloat("Current delta", &d.currentDelta, 0.01f);
						ImGui::DragFloat("Increment per second", &d.incrementPerSecond, 0.01f);
						ImGui::Text("Delegate count: %zd", d.unk3.functors.size());
						ImGui::TreePop();
					}
				}

				ImGui::SeparatorText("Per-layer timescale interpolator stacks");
				i = 0;
				for (auto& interpolator : ext->timeScaleInterpolators) {
					if (ImGui::TreeNodeEx(&interpolator, ImGuiTreeNodeFlags_None, "Layer %d", i++)) {
						ImGui::DragFloat("Timescale", &interpolator.timeScale);

						size_t j = 0;
						for (auto& d : interpolator.unk1) {
							if (ImGui::TreeNodeEx(&d, ImGuiTreeNodeFlags_None, "Interpolator %d", j++)) {
								ImGui::DragFloat("Timescale", &d.timeScale, 0.01f);
								ImGui::DragFloat("Current delta", &d.currentDelta, 0.01f);
								ImGui::DragFloat("Increment per second", &d.incrementPerSecond, 0.01f);
								ImGui::Text("Delegate count: %zd", d.unk3.functors.size());
								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}
			}
		}
	}
}

#include "Paths.h"

namespace devtools::debug_rendering::renderables {
	void Paths::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::path::PathComponent::GetClass()) {
					hh::path::PathEvaluator pathEvaluator{ static_cast<hh::path::PathComponent*>(goc) };
					hh::gfnd::DrawVertex segments[2 * 100];
					unsigned short indices[2 * 100];

					float len = pathEvaluator.GetLength();

					for (size_t i = 0; i < 100; i++) {
						float time = len * (float)i / 100.0f;
						float nextTime = len * (float)(i + 1) / 100.0f;

						csl::math::Vector3 pos = pathEvaluator.GetWorldPosition(time);

						segments[i * 2] = { pos.x(), pos.y(), pos.z(), 0xFF00FF00 };

						pos = pathEvaluator.GetWorldPosition(nextTime);

						segments[i * 2 + 1] = { pos.x(), pos.y(), pos.z(), 0xFF00FF00 };
					}

					for (unsigned short i = 0; i < 200; i++)
						indices[i] = i;

					ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, segments, indices, 200);
				}
			}
		}
	}

	void Paths::RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx) {
		if (!enabled)
			return;

		for (auto* gameObject : hh::game::GameManager::GetInstance()->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::path::PathComponent::GetClass()) {
					auto* pathGoc = static_cast<hh::path::PathComponent*>(goc);
					auto maybePos = ctx.WorldCoordsToImGui(hh::path::PathEvaluator{ pathGoc }.GetWorldPosition(0.0f));

					if (!maybePos.has_value())
						continue;

					auto pos = maybePos.value();

					auto textSize = ImGui::CalcTextSize(pathGoc->GetName());
					auto& style = ImGui::GetStyle();
					auto* dl = ImGui::GetWindowDrawList();

					dl->AddRectFilled(pos - textSize / 2 - style.FramePadding, pos + textSize / 2 + style.FramePadding, 0x60000000);

					ImGui::SetCursorPos(pos - textSize / 2);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::Text("%s", pathGoc->GetName());
					ImGui::PopStyleColor();
				}
			}
		}
	}
}

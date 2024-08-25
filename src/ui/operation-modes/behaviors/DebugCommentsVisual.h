#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include <utilities/math/MathUtils.h>

using namespace hh::game;

class DebugCommentsVisualBehavior : public OperationModeBehavior {
	bool renderDebugComments{ false };

public:
	static constexpr unsigned int id = 1;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	virtual void Render() override {
		if (ImGui::Begin("Main menu")) {
			ImGui::SameLine();
			ImGui::Checkbox("Render debug comments", &renderDebugComments);
		}
		ImGui::End();

		if (!renderDebugComments)
			return;

		auto* gameManager = GameManager::GetInstance();
		auto* camera = gameManager->GetService<CameraManager>()->GetTopComponent(0);

		if (!camera)
			return;

		Eigen::Projective3f cameraMatrix{ camera->viewportData.projMatrix * camera->viewportData.viewMatrix };

		auto* ivp = ImGui::GetMainViewport();

		if (operationMode.BeginOverlayWindow()) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });

			for (auto* obj : gameManager->objects) {
				if (auto* gocTransform = obj->GetComponent<GOCTransform>()) {
					hh::dbg::MsgGetDebugCommentInEditor msgGetDbgCmt{};

					obj->SendMessageImm(msgGetDbgCmt);
					if (msgGetDbgCmt.comment[0] == 0)
						continue;

					Eigen::Projective3f fullMatrix{ cameraMatrix * TransformToAffine3f(gocTransform->GetFrame().fullTransform).matrix() };
					Eigen::Vector4f clipSpaceOrigin{ fullMatrix * Eigen::Vector4f{ 0.0f, 0.0f, 0.0f, 1.0f } };

					// Cull
					if (clipSpaceOrigin.x() < -clipSpaceOrigin.w() || clipSpaceOrigin.x() > clipSpaceOrigin.w()
						|| clipSpaceOrigin.y() < -clipSpaceOrigin.w() || clipSpaceOrigin.y() > clipSpaceOrigin.w()
						|| clipSpaceOrigin.z() <= 0 || clipSpaceOrigin.z() > clipSpaceOrigin.w())
						continue;

					Eigen::Vector4f transformedOrigin{ clipSpaceOrigin * 0.5 / clipSpaceOrigin.w() };

					auto originInImGuiSpace = ImVec2{ (transformedOrigin.x() + 0.5f) * ivp->Size.x, (0.5f - transformedOrigin.y()) * ivp->Size.y };

					auto textSize = ImGui::CalcTextSize(msgGetDbgCmt.comment);

					ImGui::SetCursorPos(originInImGuiSpace - textSize / 2);
					ImGui::Text(msgGetDbgCmt.comment);
				}
			}

			ImGui::PopStyleColor();
		}
		operationMode.EndOverlayWindow();
	}
};

#include "ElementInspector.h"
#include "SurfRideEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

ElementInspector::ElementInspector(csl::fnd::IAllocator* allocator, SurfRideEditor& editor) : BaseObject{ allocator }, editor{ editor }
{
}

void ElementInspector::Render()
{
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImGuiCond_FirstUseEver, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Element inspector", NULL, windowFlags)) {
		if (editor.focusedElements.size() == 0) {
			ImGui::Text("Select an item in the left pane.");
		}
		else if (editor.focusedElements.size() > 1) {
			ImGui::Text("Multiple items selected.");
		}
		else {
			auto& focusedElement = editor.focusedElements[0];

			switch (focusedElement.type) {
			case SurfRideEditor::Selection::Type::SCENE:
				RenderSceneInspector(*focusedElement.scene);
				break;
			case SurfRideEditor::Selection::Type::CAMERA_DATA:
				RenderCameraDataInspector(*focusedElement.cameraData);
				break;
			case SurfRideEditor::Selection::Type::LAYER:
				RenderLayerInspector(*focusedElement.layer);
				break;
			case SurfRideEditor::Selection::Type::CAST:
				RenderCastInspector(*focusedElement.cast);
				break;
			}
		}
	}
	ImGui::End();
}

void ElementInspector::RenderSceneInspector(SurfRide::Scene& scene)
{
	ImGui::Text("ID: %d", scene.sceneData->ID);
	ImGui::Text("Name: %s", scene.sceneData->pName);
	ImGui::DragFloat2("Resolution", scene.sceneData->Resolution.data());
	Editor("Background color", *reinterpret_cast<csl::ut::Color8*>(&scene.sceneData->BackgroundColor));
	if (ImGui::CollapsingHeader("Active camera")) {
		ImGui::Text("ID: %d", scene.camera.camera.ID);
		ImGui::Text("Name: %s", scene.camera.camera.pName);
		ImGui::SeparatorText("View matrix:");
		Viewer("View matrix", scene.camera.viewMatrix);
		ImGui::SeparatorText("Projection matrix:");
		Viewer("Projection matrix", scene.camera.projectionMatrix);
	}
}

void ElementInspector::RenderLayerInspector(SurfRide::Layer& layer)
{
	ImGui::Text("ID: %d", layer.layerData->ID);
	ImGui::Text("Name: %s", layer.layerData->pName);

	if (ImGui::BeginCombo("Current animation", layer.layerData->pAnimations[layer.currentAnimationIndex].pName)) {
		for (size_t i = 0; i < layer.layerData->AnimationCount; i++) {
			if (ImGui::Selectable(layer.layerData->pAnimations[i].pName, layer.currentAnimationIndex == i))
				layer.StartAnimation(i, 0, false);
			if (layer.currentAnimationIndex == i)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::SliderFloat("currentFrame", &layer.currentFrame, layer.startFrame, layer.endFrame);
	ImGui::SliderFloat("currentFrame2", &layer.currentFrame2, layer.startFrame, layer.endFrame);
	ImGui::DragFloat("startFrame", &layer.startFrame, 0.01f);
	ImGui::DragFloat("endFrame", &layer.endFrame, 0.01f);
	ImGui::DragFloat("unk11a", &layer.unk11a, 0.01f);
	ImGui::SliderFloat("currentFrame3", &layer.currentFrame3, layer.startFrame, layer.endFrame);
	ImGui::Text("unk13: %x", layer.unk13);
	ImGui::Text("flags: %x", layer.flags);
	ImGui::Checkbox("atAnimationStart", &layer.atAnimationStart);
	ImGui::Checkbox("unk14b", &layer.unk14b);
	ImGui::Checkbox("isLooping", &layer.isLooping);
	ImGui::Checkbox("dontLoop", &layer.dontLoop);
	ImGui::Checkbox("atAnimationEnd", &layer.atAnimationEnd);
	ImGui::Checkbox("playInReverse", &layer.playInReverse);
	ImGui::Checkbox("unk16", &layer.unk16);
	ImGui::Text("unk17: %x", layer.unk17);
	ImGui::Text("unk18: %x", layer.unk18);
	ImGui::Text("unk19: %x", layer.unk19);

	Editor("Translation", layer.unk20.translation);
	ImGui::DragFloat3("Rotation", &layer.unk20.rotationX); // and Y and Z
	Editor("Scale", layer.unk20.scale);
	Editor("Material color", layer.unk20.materialColor);
	Editor("Illumination color", layer.unk20.illuminationColor);
	ImGui::DragInt("unk7", &layer.unk20.unk7);
	ImGui::DragInt("unk8", &layer.unk20.unk8);
	ImGui::DragScalar("unk9", ImGuiDataType_U16, &layer.unk20.unk9);
	//uint32_t unk10[13];
	//Layer* layer;
	//uint64_t unk16;
	//uint64_t unk17;
	Viewer("unk20.unk18", layer.unk20.unk18);
}

void ElementInspector::RenderCastInspector(SurfRide::Cast& cast)
{
}

void ElementInspector::RenderCameraDataInspector(SurfRide::SRS_CAMERA& camera)
{
	ImGui::Text("ID: %d", camera.ID);
	ImGui::Text("Name: %s", camera.pName);
	Editor("Position", camera.Position);
	Editor("Target", camera.Target);
	ImGui::Text("Flags: %x", camera.Flags);
	ImGui::DragInt("FOV", &camera.Fov);
	ImGui::DragFloat("Near clipping plane", &camera.NearPlane);
	ImGui::DragFloat("Far clipping plane", &camera.FarPlane);
	ImGui::Text("Unk: %zx", camera.unk);
}

#include "ElementInspector.h"
#include "SurfRideElement.h"
#include "Behaviors.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/SurfRide.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace SurfRide;

	void ElementInspector::RenderPanel()
	{
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0) {
			ImGui::Text("Select an item in the left pane.");
		}
		else if (selection.size() > 1) {
			ImGui::Text("Multiple items selected.");
		}
		else {
			auto& focusedElement = selection[0];

			switch (focusedElement.type) {
			case SurfRideElement::Type::SCENE:
				RenderSceneInspector(*focusedElement.scene);
				break;
			case SurfRideElement::Type::CAMERA_DATA:
				RenderCameraDataInspector(*focusedElement.cameraData);
				break;
			case SurfRideElement::Type::LAYER:
				RenderLayerInspector(*focusedElement.layer);
				break;
			case SurfRideElement::Type::CAST:
				RenderCastInspector(*focusedElement.cast);
				break;
			}
		}
	}

	PanelTraits ElementInspector::GetPanelTraits() const
	{
		return { "Element inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}

	void ElementInspector::RenderSceneInspector(Scene& scene)
	{
		ImGui::SeparatorText("Scene");
		ImGui::Text("ID: %d", scene.sceneData->id);
		ImGui::Text("Name: %s", scene.sceneData->name);
		ImGui::DragFloat2("Resolution", scene.sceneData->resolution.data());
		Editor("Background color", *reinterpret_cast<csl::ut::Color8*>(&scene.sceneData->backgroundColor));
		if (ImGui::CollapsingHeader("Active camera")) {
			ImGui::Text("ID: %d", scene.camera.camera.id);
			ImGui::Text("Name: %s", scene.camera.camera.name);
			ImGui::SeparatorText("View matrix:");
			Viewer("View matrix", scene.camera.viewMatrix);
			ImGui::SeparatorText("Projection matrix:");
			Viewer("Projection matrix", scene.camera.projectionMatrix);
		}
	}

	void ElementInspector::RenderLayerInspector(Layer& layer)
	{
		ImGui::SeparatorText("Layer");
		ImGui::Text("ID: %d", layer.layerData->id);
		ImGui::Text("Name: %s", layer.layerData->name);

		if (ImGui::BeginCombo("Current animation", layer.layerData->animations[layer.currentAnimationIndex].name)) {
			for (size_t i = 0; i < layer.layerData->animationCount; i++) {
				if (ImGui::Selectable(layer.layerData->animations[i].name, layer.currentAnimationIndex == i))
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

		Editor("Transform", layer.rootTransform);
	}

	void ElementInspector::RenderCastInspector(Cast& cast)
	{
		switch (static_cast<SurfRide::SRS_CASTNODE::Type>(cast.flags & 0xF)) {
		case SurfRide::SRS_CASTNODE::Type::NORMAL: RenderNormalCastInspector(static_cast<ImageCast&>(cast)); break;
		case SurfRide::SRS_CASTNODE::Type::IMAGE: RenderImageCastInspector(static_cast<ImageCast&>(cast)); break;
		case SurfRide::SRS_CASTNODE::Type::REFERENCE: RenderReferenceCastInspector(static_cast<ReferenceCast&>(cast)); break;
		case SurfRide::SRS_CASTNODE::Type::SLICE: RenderSliceCastInspector(static_cast<SliceCast&>(cast)); break;
		default: break;
		}
	}

	void ElementInspector::RenderBaseCastInspector(Cast& cast)
	{
		Viewer("ID", cast.castData->id);
		Viewer("Name", cast.castData->name);

		if (cast.castData->userData)
			Editor("User data", *cast.castData->userData);

		if (Editor("Transform", *cast.transform))
			cast.transform->dirtyFlag.flags.m_dummy |= cast.transform->dirtyFlag.transformAny.m_dummy;
	}

	void ElementInspector::RenderNormalCastInspector(Cast& cast)
	{
		ImGui::SeparatorText("Cast");

		RenderBaseCastInspector(cast);
	}

	void ElementInspector::RenderImageCastInspector(ImageCast& cast)
	{
		ImGui::SeparatorText("Image cast");

		RenderBaseCastInspector(cast);

		if (Editor("Image cast properties", *cast.imageCastData)) {
			cast.size = cast.imageCastData->size;
			cast.vertexColorTopLeft = cast.imageCastData->vertexColorTopLeft;
			cast.vertexColorBottomLeft = cast.imageCastData->vertexColorBottomLeft;
			cast.vertexColorTopRight = cast.imageCastData->vertexColorTopRight;
			cast.vertexColorBottomRight = cast.imageCastData->vertexColorBottomRight;
			cast.cropIndex[0] = cast.imageCastData->cropIndex0;
			cast.cropIndex[1] = cast.imageCastData->cropIndex1;
			cast.transform->dirtyFlag.flags.m_dummy |= cast.transform->dirtyFlag.cellAny.m_dummy;
		}
	}

	void ElementInspector::RenderReferenceCastInspector(SurfRide::ReferenceCast& cast)
	{
		ImGui::SeparatorText("Reference cast");

		RenderBaseCastInspector(cast);

		if (Editor("Reference cast properties", *cast.referenceCastData)) {

		}
	}

	void ElementInspector::RenderSliceCastInspector(SurfRide::SliceCast& cast)
	{
		ImGui::SeparatorText("Slice cast");

		RenderBaseCastInspector(cast);

		if (Editor("Slice cast properties", *cast.sliceCastData)) {
			cast.size = cast.sliceCastData->size;
			cast.vertexColorTopLeft = cast.sliceCastData->vertexColorTopLeft;
			cast.vertexColorBottomLeft = cast.sliceCastData->vertexColorBottomLeft;
			cast.vertexColorTopRight = cast.sliceCastData->vertexColorTopRight;
			cast.vertexColorBottomRight = cast.sliceCastData->vertexColorBottomRight;
			cast.transform->dirtyFlag.flags.m_dummy |= cast.transform->dirtyFlag.cellAny.m_dummy;
		}
	}

	void ElementInspector::RenderCameraDataInspector(SRS_CAMERA& camera)
	{
		Viewer("ID", camera.id);
		Viewer("Name", camera.name);
		Editor("Position", camera.position);
		Editor("Target", camera.target);
		Viewer("Flags", camera.flags);
		Editor("FOV", camera.fov);
		Editor("Near clipping plane", camera.nearPlane);
		Editor("Far clipping plane", camera.farPlane);
		Viewer("Unk", camera.unk);
	}
}

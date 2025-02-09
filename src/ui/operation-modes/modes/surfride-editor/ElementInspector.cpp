#include "ElementInspector.h"
#include "SurfRideElement.h"
#include "Behaviors.h"
#include <resources/ManagedMemoryRegistry.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/SurfRide.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::v6;

	void ElementInspector::RenderPanel()
	{
		auto& context = GetContext();
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0)
			ImGui::Text("Select an item in the left pane.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple items selected.");
		else {
			auto& focusedElement = selection[0];

			switch (focusedElement.type) {
			case SurfRideElement::Type::SCENE: RenderSceneInspector(*context.FindScene(focusedElement.id), context.FindRuntimeScene(focusedElement.id)); break;
			case SurfRideElement::Type::CAMERA: RenderCameraInspector(*context.FindCamera(focusedElement.id)); break;
			case SurfRideElement::Type::LAYER: RenderLayerInspector(*context.FindLayer(focusedElement.id), context.FindRuntimeLayer(focusedElement.id)); break;
			case SurfRideElement::Type::CAST: RenderCastInspector(*context.FindCast(focusedElement.id), context.FindRuntimeCast(focusedElement.id)); break;
			}
		}
	}

	PanelTraits ElementInspector::GetPanelTraits() const
	{
		return { "Element inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}

	void ElementInspector::RenderSceneInspector(SRS_SCENE& scene, SurfRide::Scene* runtimeScene)
	{
		ImGui::SeparatorText("Scene");
		ImGui::Text("ID: %d", scene.id);
		ImGui::Text("Name: %s", scene.name);
		ImGui::DragFloat2("Resolution", scene.resolution.data());
		Editor("Background color", *reinterpret_cast<csl::ut::Color8*>(&scene.backgroundColor));
		
		if (ImGui::BeginCombo("Camera", scene.cameras[scene.currentCameraIndex].name)) {
			for (unsigned short i = 0; i < scene.cameraCount; i++) {
				auto& camera = scene.cameras[i];

				if (ImGui::Selectable(camera.name, scene.currentCameraIndex == i))
					scene.currentCameraIndex = i;

				if (scene.currentCameraIndex == i)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		bool hides = scene.GetHideFlag();
		if (Editor("Hide", hides))
			scene.SetHideFlag(hides);

		ImGui::Separator();
		Editor("User data", GetContext().projectResource, scene.userData);
	}

	void ElementInspector::RenderLayerInspector(SRS_LAYER& layer, SurfRide::Layer* runtimeLayer)
	{
		ImGui::SeparatorText("Layer");
		ImGui::Text("ID: %d", layer.id);
		ImGui::Text("Name: %s", layer.name);

		if (ImGui::BeginCombo("Current animation", layer.animations[layer.currentAnimationIndex].name)) {
			for (int i = 0; i < layer.animationCount; i++) {
				if (ImGui::Selectable(layer.animations[i].name, layer.currentAnimationIndex == i))
					GetContext().StartAnimationByIndex(layer, i);
				if (layer.currentAnimationIndex == i)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();
		Editor("User data", GetContext().projectResource, layer.userData);

//		ImGui::Separator();
//		ImGui::SliderFloat("currentFrame", &runtimeLayer.currentFrame, runtimeLayer.startFrame, runtimeLayer.endFrame);
//		ImGui::SliderFloat("currentFrame2", &runtimeLayer.currentFrame2, runtimeLayer.startFrame, runtimeLayer.endFrame);
//		ImGui::DragFloat("startFrame", &runtimeLayer.startFrame, 0.01f);
//		ImGui::DragFloat("endFrame", &runtimeLayer.endFrame, 0.01f);
//		ImGui::DragFloat("unk11a", &runtimeLayer.unk11a, 0.01f);
//		ImGui::SliderFloat("currentFrame3", &runtimeLayer.currentFrame3, runtimeLayer.startFrame, runtimeLayer.endFrame);
//		ImGui::Text("unk13: %x", runtimeLayer.unk13);
//		ImGui::Text("flags: %x", runtimeLayer.flags);
//		ImGui::Checkbox("atAnimationStart", &runtimeLayer.atAnimationStart);
//		ImGui::Checkbox("unk14b", &runtimeLayer.unk14b);
//		ImGui::Checkbox("isLooping", &runtimeLayer.isLooping);
//		ImGui::Checkbox("dontLoop", &runtimeLayer.dontLoop);
//		ImGui::Checkbox("atAnimationEnd", &runtimeLayer.atAnimationEnd);
//		ImGui::Checkbox("playInReverse", &runtimeLayer.playInReverse);
//		ImGui::Checkbox("unk16", &runtimeLayer.unk16);
//#ifndef DEVTOOLS_TARGET_SDK_wars
//		ImGui::Text("unk17: %x", runtimeLayer.unk17);
//		ImGui::Text("unk18: %x", runtimeLayer.unk18);
//		ImGui::Text("unk19: %x", runtimeLayer.unk19);
//
//		//Editor("Transform", layer.rootTransform);
//#endif
	}

	void ElementInspector::RenderCastInspector(SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast)
	{
		switch (static_cast<SRS_CASTNODE::Type>(cast.flags & 0xF)) {
		case SRS_CASTNODE::Type::NORMAL: RenderNormalCastInspector(cast, runtimeCast); break;
		case SRS_CASTNODE::Type::IMAGE: RenderImageCastInspector(cast, static_cast<SurfRide::ImageCast*>(runtimeCast)); break;
		case SRS_CASTNODE::Type::REFERENCE: RenderReferenceCastInspector(cast, static_cast<SurfRide::ReferenceCast*>(runtimeCast)); break;
		case SRS_CASTNODE::Type::SLICE: RenderSliceCastInspector(cast, static_cast<SurfRide::SliceCast*>(runtimeCast)); break;
		default: break;
		}
	}

	void ElementInspector::RenderBaseCastInspector(SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast)
	{
		auto context = GetContext();

		Viewer("ID", cast.id);
		InputText("Name", cast.name, context.projectResource);

		bool transformEdited{};
		transformEdited |= Editor("Cast", cast);

		Editor("User data", context.projectResource, cast.userData);

		auto& layer = *context.FindCastLayer(cast.id);
		size_t castIndex = &cast - layer.casts;

		if (layer.Is3D())
			transformEdited |= Editor("Transform", layer.transforms.transforms3d[castIndex]);
		else
			transformEdited |= Editor("Transform", layer.transforms.transforms2d[castIndex]);

		if (transformEdited)
			context.ApplyTransformChange(cast);
	}

	void ElementInspector::RenderNormalCastInspector(SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast)
	{
		ImGui::SeparatorText("Cast");

		RenderBaseCastInspector(cast, runtimeCast);
	}

	void ElementInspector::RenderImageCastInspector(SRS_CASTNODE& cast, SurfRide::ImageCast* runtimeCast)
	{
		ImGui::SeparatorText("Image cast");

		RenderBaseCastInspector(cast, runtimeCast);

		auto& imageCast = *cast.data.image;

		if (Editor("Image cast properties", GetContext().projectResource, imageCast))
			GetContext().ApplyImageCastChange(cast);
	}

	void ElementInspector::RenderReferenceCastInspector(SRS_CASTNODE& cast, SurfRide::ReferenceCast* runtimeCast)
	{
		ImGui::SeparatorText("Reference cast");

		RenderBaseCastInspector(cast, runtimeCast);

		if (Editor("Reference cast properties", *cast.data.reference))
			GetContext().ApplyReferenceCastChange(cast);
	}

	void ElementInspector::RenderSliceCastInspector(SRS_CASTNODE& cast, SurfRide::SliceCast* runtimeCast)
	{
		ImGui::SeparatorText("Slice cast");

		RenderBaseCastInspector(cast, runtimeCast);

		if (Editor("Slice cast properties", GetContext().projectResource, *cast.data.slice))
			GetContext().ApplySliceCastChange(cast);
	}

	void ElementInspector::RenderCameraInspector(SRS_CAMERA& camera)
	{
		Viewer("ID", camera.id);
		Viewer("Name", camera.name);
		Editor("Position", camera.position);
		Editor("Target", camera.target);
		Editor("Orthogonal", camera.isOrthogonal);
		Editor("FOV", camera.fov);
		Editor("Near clipping plane", camera.nearPlane);
		Editor("Far clipping plane", camera.farPlane);
		Viewer("Unk1", camera.unk1);
		Viewer("Unk2", camera.unk2);
	}
}

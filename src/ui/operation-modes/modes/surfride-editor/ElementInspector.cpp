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
			ImGui::Text("ID: %d", scene.camera.GetCameraData().id);
			ImGui::Text("Name: %s", scene.camera.GetCameraData().name);
			ImGui::SeparatorText("View matrix:");
			Viewer("View matrix", scene.camera.viewMatrix);
			ImGui::SeparatorText("Projection matrix:");
#ifndef DEVTOOLS_TARGET_SDK_wars
			Viewer("Projection matrix", scene.camera.projectionMatrix);
#endif
		}
	}

	void ElementInspector::RenderLayerInspector(Layer& layer)
	{
		ImGui::SeparatorText("Layer");
		ImGui::Text("ID: %d", layer.layerData->id);
		ImGui::Text("Name: %s", layer.layerData->name);

		if (ImGui::BeginCombo("Current animation", layer.layerData->animations[layer.currentAnimationIndex].name)) {
			for (int i = 0; i < layer.layerData->animationCount; i++) {
				if (ImGui::Selectable(layer.layerData->animations[i].name, layer.currentAnimationIndex == i))
					layer.StartAnimation(i);
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
#ifndef DEVTOOLS_TARGET_SDK_wars
		ImGui::Text("unk17: %x", layer.unk17);
		ImGui::Text("unk18: %x", layer.unk18);
		ImGui::Text("unk19: %x", layer.unk19);

		//Editor("Transform", layer.rootTransform);
#endif
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
		InputText("Name", const_cast<char*&>(cast.castData->name), GetContext().gocSprite->projectResource);

		if (cast.castData->userData)
			Editor("User data", *cast.castData->userData);

#ifdef DEVTOOLS_TARGET_SDK_wars
		size_t castIndex = cast.index;
#else
		size_t castIndex = (reinterpret_cast<size_t>(cast.castData) - reinterpret_cast<size_t>(cast.layer->layerData->casts)) / sizeof(SRS_CASTNODE);
#endif

		bool transformEdited{};

#ifdef DEVTOOLS_TARGET_SDK_wars
		transformEdited |= CheckboxFlags("Disable rotation", cast.castData->flags, 0x200u);
		transformEdited |= CheckboxFlags("Disable scale", cast.castData->flags, 0x400u);
#endif

		//transformEdited |= CheckboxFlags("TF flag 1", cast.castData->flags, 0x10000u);
		//transformEdited |= CheckboxFlags("TF flag 2", cast.castData->flags, 0x20000u);
		//transformEdited |= CheckboxFlags("TF flag 3", cast.castData->flags, 0x40000u);

		transformEdited |= CheckboxFlags("Transform material color", cast.castData->flags, 0x20u);
		transformEdited |= CheckboxFlags("Transform illumination color", cast.castData->flags, 0x80u);
		transformEdited |= CheckboxFlags("Transform display flag", cast.castData->flags, 0x40u);

		if (cast.layer->flags.test(SurfRide::Layer::Flag::IS_3D)) {
			auto& transform = cast.layer->layerData->transforms.transforms3d[castIndex];

			transformEdited |= Editor("Transform", transform);

			if (transformEdited) {
#ifdef DEVTOOLS_TARGET_SDK_wars
				auto* castTransform = static_cast<SRS_TRS3D*>(cast.transformData);
				castTransform->position = transform.position;
				castTransform->rotation = transform.rotation;
				castTransform->scale = transform.scale;
				static_cast<Cast3D&>(cast).position = transform.position;
#else
				cast.transform->position = transform.position;
				cast.transform->rotation = transform.rotation;
				cast.transform->scale = transform.scale;
#endif
				cast.flags = cast.castData->flags;
				cast.transform->materialColor = transform.materialColor;
				cast.transform->illuminationColor = transform.illuminationColor;
				cast.transform->display = transform.display;

				cast.transform->dirtyFlag.SetTransformAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
				cast.UpdateParentsAndThisTransformRecursively();
#endif
			}
		}
		else {
			auto& transform = cast.layer->layerData->transforms.transforms2d[castIndex];

			transformEdited |= Editor("Transform", transform);

			if (transformEdited) {
#ifdef DEVTOOLS_TARGET_SDK_wars
				auto* castTransform = static_cast<SRS_TRS3D*>(cast.transformData);
				castTransform->position = { transform.position.x(), transform.position.y(), 0.0f };
				castTransform->rotation = { 0, 0, transform.rotation };
				castTransform->scale = { transform.scale.x(), transform.scale.y(), 0.0f };
				static_cast<Cast3D&>(cast).position = { transform.position.x(), transform.position.y(), 0.0f };
				cast.flags = cast.castData->flags;
#else
				cast.transform->position = { transform.position.x(), transform.position.y(), 0.0f };
				cast.transform->rotation = { 0, 0, transform.rotation };
				cast.transform->scale = { transform.scale.x(), transform.scale.y(), 0.0f };
#endif
				cast.transform->materialColor = transform.materialColor;
				cast.transform->illuminationColor = transform.illuminationColor;
				cast.transform->display = transform.display;

				cast.transform->dirtyFlag.SetTransformAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
				cast.UpdateParentsAndThisTransformRecursively();
#endif
			}
		}

		if (transformEdited) {
			if (cast.layer->flags.test(SurfRide::Layer::Flag::IS_3D)) {
			}
		}
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

		if (Editor("Image cast properties", GetContext().gocSprite->projectResource, *cast.imageCastData)) {
			cast.size = cast.imageCastData->size;
#ifndef DEVTOOLS_TARGET_SDK_wars
			cast.vertexColorTopLeft = cast.imageCastData->vertexColorTopLeft;
			cast.vertexColorBottomLeft = cast.imageCastData->vertexColorBottomLeft;
			cast.vertexColorTopRight = cast.imageCastData->vertexColorTopRight;
			cast.vertexColorBottomRight = cast.imageCastData->vertexColorBottomRight;
#endif
			cast.cropIndex[0] = cast.imageCastData->cropIndex0;
			cast.cropIndex[1] = cast.imageCastData->cropIndex1;
			//cast.cropRectDirty[0] = true;
			//cast.cropRectDirty[1] = true;

#ifndef DEVTOOLS_TARGET_SDK_wars
			cast.blurEffect = nullptr;
			cast.reflectEffect = nullptr;
			cast.CreateEffectCast(cast.imageCastData->effectData);
#endif

			cast.transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
			cast.UpdateParentsAndThisTransformRecursively();
#endif
		}

		if (cast.text && cast.text->textData) {
			if (Editor("Live text", *cast.text->textData)) {
				cast.transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
				cast.UpdateParentsAndThisTransformRecursively();
#endif
			}
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

		if (Editor("Slice cast properties", GetContext().gocSprite->projectResource, *cast.sliceCastData)) {
			cast.size = cast.sliceCastData->size;
#ifndef DEVTOOLS_TARGET_SDK_wars
			cast.vertexColorTopLeft = cast.sliceCastData->vertexColorTopLeft;
			cast.vertexColorBottomLeft = cast.sliceCastData->vertexColorBottomLeft;
			cast.vertexColorTopRight = cast.sliceCastData->vertexColorTopRight;
			cast.vertexColorBottomRight = cast.sliceCastData->vertexColorBottomRight;
#endif
#ifndef DEVTOOLS_TARGET_SDK_wars
			cast.blurEffect = nullptr;
			cast.reflectEffect = nullptr;
			cast.CreateEffectCast(cast.sliceCastData->effectData);
#endif
			cast.transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
			cast.UpdateParentsAndThisTransformRecursively();
#endif
		}
	}

	void ElementInspector::RenderCameraDataInspector(SRS_CAMERA& camera)
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

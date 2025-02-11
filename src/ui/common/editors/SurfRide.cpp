bool Editor(const char* label, ucsl::resources::swif::v6::SRS_CROPREF& cropRef);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_DATA& data);

#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <resources/ManagedMemoryRegistry.h>
#include <ui/operation-modes/modes/surfride-editor/texture-editor/References.h>
#include "SurfRide.h"

using namespace ucsl::resources::swif::v6;

const char* blendModeNames[]{ "Mix", "Add", "Subtract", "Multiply", "Mix - Premultiplied Alpha", "Override" };
const char* pivotTypeNames[]{ "Top Left", "Top Center", "Top Right", "Center Left", "Center Center", "Center Right", "Bottom Left", "Bottom Center", "Bottom Right", "Custom" };
const char* orientationNames[]{ "Up", "Left", "Down", "Right" };
const char* effectTypeNames[]{ "None", "Blur", "Reflect" };
const char* sliceConstraintNames[]{ "Unknown 0", "Unknown 1", "Unknown 2", "Unknown 3", "Unknown 4", "Unknown 5", "Unknown 6", "Unknown 7", "Unknown 8", "Unknown 9" };
const char* cropBlendModeNames[]{ "Crop 0", "Crop 1", "Modulate", "Alpha blend" };
const char* renderModeNames[]{ "Unknown 0", "Override", "Unknown 2", "Unknown 3" };
const char* userDataTypes[]{ "Boolean", "Signed integer", "Unsigned integer", "Float", "Unknown", "String"};

bool Editor(const char* label, Color& color) {
	float colorAsFloat[]{
		static_cast<float>(color.r) / 255,
		static_cast<float>(color.g) / 255,
		static_cast<float>(color.b) / 255,
		static_cast<float>(color.a) / 255,
	};
	float editableColor[4]{ colorAsFloat[0], colorAsFloat[1], colorAsFloat[2], colorAsFloat[3] };

	bool edited = ImGui::ColorEdit4(label, editableColor, ImGuiColorEditFlags_Uint8);

	if (edited) {
		color.r = static_cast<uint8_t>(editableColor[0] * 255);
		color.g = static_cast<uint8_t>(editableColor[1] * 255);
		color.b = static_cast<uint8_t>(editableColor[2] * 255);
		color.a = static_cast<uint8_t>(editableColor[3] * 255);
	}

	return edited;
}

bool Editor(const char* label, Colorf& color) {
	return ImGui::ColorEdit4(label, reinterpret_cast<float*>(&color), ImGuiColorEditFlags_Float);
}

bool Editor(const char* label, SRS_TRS2D& transformData) {
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Position", transformData.position);
	edited |= Editor("Rotation", transformData.rotation);
	edited |= Editor("Scale", transformData.scale);
	edited |= Editor("UnkX", transformData.unkX);
	edited |= Editor("UnkY", transformData.unkY);
	edited |= Editor("Material color", transformData.materialColor);
	edited |= Editor("Illumination color", transformData.illuminationColor);
	edited |= Editor("Display", transformData.display);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, SRS_TRS3D& transformData) {
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Position", transformData.position);
	edited |= Editor("Rotation", transformData.rotation);
	edited |= Editor("Scale", transformData.scale);
	edited |= Editor("Material color", transformData.materialColor);
	edited |= Editor("Illumination color", transformData.illuminationColor);
	edited |= Editor("Display", transformData.display);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ucsl::resources::swif::v6::SRS_CASTNODE& castData) {
	bool edited{};
	edited |= CheckboxFlags("Disable position", castData.flags, 0x100u);
	edited |= CheckboxFlags("Disable rotation", castData.flags, 0x200u);
	edited |= CheckboxFlags("Disable scale", castData.flags, 0x400u);

	edited |= CheckboxFlags("Hide", castData.flags, 0x1000u);

	edited |= CheckboxFlags("Transform position", castData.flags, 0x10000u);
	edited |= CheckboxFlags("Transform rotation", castData.flags, 0x20000u);
	edited |= CheckboxFlags("Transform scale", castData.flags, 0x40000u);

	edited |= CheckboxFlags("Transform material color", castData.flags, 0x20u);
	edited |= CheckboxFlags("Transform illumination color", castData.flags, 0x80u);

	edited |= CheckboxFlags("Transform display flag", castData.flags, 0x40u);

	edited |= CheckboxFlags("Transform crop index 0", castData.flags, 0x4000u);
	edited |= CheckboxFlags("Transform crop index 1", castData.flags, 0x8000u);

	auto sliceConstraint = castData.GetSliceConstraint();
	if (ComboEnum("Slice constraint", sliceConstraint, sliceConstraintNames)) {
		castData.SetSliceConstraint(sliceConstraint);
		edited = true;
	}

	return edited;
}

//bool Editor(const char* label, Transform& transform) {
//	bool edited{};
//	ImGui::PushID(label);
//	ImGui::SeparatorText(label);
//	edited |= Editor("Position", transform.position);
//	edited |= Editor("Rotation", transform.rotation);
//	edited |= Editor("Scale", transform.scale);
//	edited |= Editor("Material color", transform.materialColor);
//	edited |= Editor("Illumination color", transform.illuminationColor);
//	edited |= Editor("Display", transform.display);
//	ImGui::PopID();
//	return edited;
//}

bool Editor(const char* label, SRS_BLUR3D& effect) {
	bool edited{};
	edited |= Editor("Unk1", effect.unk1);
	edited |= Editor("Unk2", effect.unk2);
	edited |= Editor("Crop count", effect.cropCount);
	edited |= Editor("Steps", effect.steps);
	edited |= Editor("Duration", effect.duration);
	edited |= Editor("Color", effect.color);
	edited |= CheckboxFlags("Hide", effect.flags, 0x1000u);

	auto blendMode = effect.GetBlendMode();
	if (ComboEnum("Blend mode", blendMode, blendModeNames)) {
		effect.SetBlendMode(blendMode);
		edited = true;
	}

	return edited;
}

bool Editor(const char* label, SRS_REFLECT3D& effect) {
	bool edited{};
	edited |= Editor("Unk1", effect.unk1);
	edited |= Editor("Unk2", effect.unk2);
	edited |= Editor("Unk3", effect.unk3);
	edited |= Editor("Unk4", effect.unk4);
	edited |= Editor("Unk5", effect.unk5);
	edited |= Editor("Unk6", effect.unk6);
	edited |= Editor("Unk7", effect.unk7);
	edited |= Editor("Color", effect.color);
	edited |= CheckboxFlags("Hide", effect.flags, 0x1000u);

	auto blendMode = effect.GetBlendMode();
	if (ComboEnum("Blend mode", blendMode, blendModeNames)) {
		effect.SetBlendMode(blendMode);
		edited = true;
	}

	return edited;
}

const char* verticalAlignmentNames[]{ "Top", "Center", "Bottom" };
bool Editor(const char* label, SRS_TEXTDATA& textData) {
	bool edited{};

	auto verticalAlignment = textData.GetVerticalAlignment();
	if (ComboEnum("Pivot type", verticalAlignment, verticalAlignmentNames)) {
		textData.SetVerticalAlignment(verticalAlignment);
		edited = true;
	}
	Viewer("Text", textData.text);
	edited |= Editor("Font index", textData.fontIndex);
	edited |= Editor("Scale", textData.scale);
	edited |= Editor("Padding left", textData.paddingLeft);
	edited |= Editor("Padding right", textData.paddingRight);
	edited |= Editor("Padding top", textData.paddingTop);
	edited |= Editor("Padding bottom", textData.paddingBottom);
	edited |= Editor("Tracking", textData.tracking);
	edited |= Editor("Line height", textData.lineHeight);

	return edited;
}

bool Editor(const char* label, SRS_CROPREF& cropRef) {
	bool edited{};

	ImGui::BeginGroup();
	edited |= Editor("Texture List index", cropRef.textureListIndex);
	edited |= Editor("Texture index", cropRef.textureIndex);
	edited |= Editor("Crop index", cropRef.cropIndex);
	ImGui::EndGroup();

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("surfride:TextureRef")) {
			auto textureRef = *static_cast<ui::operation_modes::modes::surfride_editor::texture_editor::TextureRef*>(payload->Data);

			cropRef.textureListIndex = static_cast<short>(textureRef.textureListIndex);
			cropRef.textureIndex = static_cast<short>(textureRef.textureIndex);
			cropRef.cropIndex = -1;

			edited = true;
		}
		else if (auto* payload = ImGui::AcceptDragDropPayload("surfride:CropRef")) {
			auto sourceCropRef = *static_cast<ui::operation_modes::modes::surfride_editor::texture_editor::CropRef*>(payload->Data);

			cropRef.textureListIndex = static_cast<short>(sourceCropRef.textureListIndex);
			cropRef.textureIndex = static_cast<short>(sourceCropRef.textureIndex);
			cropRef.cropIndex = static_cast<short>(sourceCropRef.cropIndex);

			edited = true;
		}
		ImGui::EndDragDropTarget();
	}

	return edited;
}

bool Editor(const char* label, hh::fnd::ManagedResource* resource, SRS_IMAGECAST& imageCastData) {
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Size", imageCastData.size);

	auto pivotType = imageCastData.GetPivotType();
	if (ComboEnum("Pivot type", pivotType, pivotTypeNames)) {
		imageCastData.SetPivotType(pivotType);
		edited = true;
	}

	if (pivotType == EPivotType::CUSTOM)
		edited |= Editor("Pivot", imageCastData.pivot);

	auto orientation = imageCastData.GetOrientation();
	if (ComboEnum("Orientation", orientation, orientationNames)) {
		imageCastData.SetOrientation(orientation);
		edited = true;
	}

	edited |= CheckboxFlags("Flip horizontally", imageCastData.flags, 0x10u);
	edited |= CheckboxFlags("Flip vertically", imageCastData.flags, 0x20u);

	ImGui::Separator();
	auto renderMode = imageCastData.GetRenderMode();
	if (ComboEnum("Render mode", renderMode, renderModeNames)) {
		imageCastData.SetRenderMode(renderMode);
		edited = true;
	}

	auto blendMode = imageCastData.GetBlendMode();
	if (ComboEnum("Blend mode", blendMode, blendModeNames)) {
		imageCastData.SetBlendMode(blendMode);
		edited = true;
	}

	edited |= Editor("Vertex Color Top Left", imageCastData.vertexColorTopLeft);
	edited |= Editor("Vertex Color Bottom Left", imageCastData.vertexColorBottomLeft);
	edited |= Editor("Vertex Color Top Right", imageCastData.vertexColorTopRight);
	edited |= Editor("Vertex Color Bottom Right", imageCastData.vertexColorBottomRight);

	ImGui::Separator();
	auto cropBlendMode = imageCastData.GetCropBlendMode();
	if (ComboEnum("Crop blend mode", cropBlendMode, cropBlendModeNames)) {
		imageCastData.SetCropBlendMode(cropBlendMode);
		edited = true;
	}
	edited |= CheckboxFlags("Use crop 0 alpha", imageCastData.flags, 0x20000000u);

	edited |= Editor("Crop Index 0", imageCastData.cropIndex0);
	edited |= Editor("Crop Index 1", imageCastData.cropIndex1);

	resources::ManagedCArray cropRefs0{ resource, imageCastData.cropRefs0, imageCastData.cropRef0Count };
	resources::ManagedCArray cropRefs1{ resource, imageCastData.cropRefs1, imageCastData.cropRef1Count };

	edited |= Editor("Crop References 0", cropRefs0);
	edited |= Editor("Crop References 1", cropRefs1);

	edited |= CheckboxFlags("Unknown 1", imageCastData.flags, 0x2000u);

	ImGui::Separator();
	edited |= CheckboxFlags("Enable text rendering", imageCastData.flags, 0x100u);
	if (imageCastData.textData)
		edited |= Editor("Text Data", *imageCastData.textData);

#ifdef DEVTOOLS_TARGET_SDK_rangers
	ImGui::Separator();
	auto effectType = imageCastData.GetEffectType();
	if (ComboEnum("Effect type", effectType, effectTypeNames)) {
		edited = true;
		imageCastData.SetEffectType(effectType);

		auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		if (imageCastData.effectData.blur)
			allocator.Free(imageCastData.effectData.blur);
	
		switch (effectType) {
		case EEffectType::BLUR: imageCastData.effectData.blur = new (&allocator) SRS_BLUR3D{}; break;
		case EEffectType::REFLECT: imageCastData.effectData.reflect = new (&allocator) SRS_REFLECT3D{}; break;
		default: imageCastData.effectData.blur = nullptr; break;
		}
	}

	if (imageCastData.effectData.blur) {
		switch (effectType) {
		case EEffectType::BLUR: edited |= Editor("Effect parameters", *imageCastData.effectData.blur); break;
		case EEffectType::REFLECT: edited |= Editor("Effect parameters", *imageCastData.effectData.reflect); break;
		}
	}
#endif

	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, SRS_REFERENCECAST& referenceCastData)
{
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	if (referenceCastData.layer) {
		Viewer("Layer ID", referenceCastData.layer->id);
		Viewer("Layer name", referenceCastData.layer->name);

		size_t castAnimIdx{};

		for (size_t i = 0; i < referenceCastData.layer->animationCount; i++)
			if (referenceCastData.layer->animations[i].id == referenceCastData.animationId)
				castAnimIdx = i;

		if (ImGui::BeginCombo("Animation", referenceCastData.layer->animations[castAnimIdx].name)) {
			for (int i = 0; i < referenceCastData.layer->animationCount; i++) {
				auto& anim = referenceCastData.layer->animations[i];

				if (ImGui::Selectable(anim.name, anim.id == referenceCastData.animationId)) {
					referenceCastData.animationId = i;
					edited = true;
				}

				if (anim.id == referenceCastData.animationId)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		unsigned int zero{};
		edited |= SliderScalar("Animation frame", referenceCastData.animationFrame, &zero, &referenceCastData.layer->animations[castAnimIdx].frameCount);
		edited |= Editor("unk2", referenceCastData.unk2);
		ImGui::Separator();
		edited |= CheckboxFlags("Enable blending", referenceCastData.flags, 0x2u);
		auto blendMode = referenceCastData.GetBlendMode();
		if (ComboEnum("Blend mode", blendMode, blendModeNames)) {
			referenceCastData.SetBlendMode(blendMode);
			edited = true;
		}
	}
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::fnd::ManagedResource* resource, SRS_SLICECAST& sliceCastData)
{
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Size", sliceCastData.size);

	auto pivotType = sliceCastData.GetPivotType();
	if (ComboEnum("Pivot type", pivotType, pivotTypeNames)) {
		sliceCastData.SetPivotType(pivotType);
		edited = true;
	}

	if (pivotType == SurfRide::EPivotType::CUSTOM)
		edited |= Editor("Pivot", sliceCastData.pivot);

	auto orientation = sliceCastData.GetOrientation();
	if (ComboEnum("Orientation", orientation, orientationNames)) {
		sliceCastData.SetOrientation(orientation);
		edited = true;
	}

	edited |= CheckboxFlags("Flip horizontally", sliceCastData.flags, 0x10u);
	edited |= CheckboxFlags("Flip vertically", sliceCastData.flags, 0x20u);

	ImGui::Separator();
	auto renderMode = sliceCastData.GetRenderMode();
	if (ComboEnum("Render mode", renderMode, renderModeNames)) {
		sliceCastData.SetRenderMode(renderMode);
		edited = true;
	}

	auto blendMode = sliceCastData.GetBlendMode();
	if (ComboEnum("Blend mode", blendMode, blendModeNames)) {
		sliceCastData.SetBlendMode(blendMode);
		edited = true;
	}

	edited |= Editor("Fixed size", sliceCastData.fixedSize);
	edited |= Editor("Vertex Color Top Left", sliceCastData.vertexColorTopLeft);
	edited |= Editor("Vertex Color Bottom Left", sliceCastData.vertexColorBottomLeft);
	edited |= Editor("Vertex Color Top Right", sliceCastData.vertexColorTopRight);
	edited |= Editor("Vertex Color Bottom Right", sliceCastData.vertexColorBottomRight);

	ImGui::Separator();
	auto cropBlendMode = sliceCastData.GetCropBlendMode();
	if (ComboEnum("Crop blend mode", cropBlendMode, cropBlendModeNames)) {
		sliceCastData.SetCropBlendMode(cropBlendMode);
		edited = true;
	}

	edited |= CheckboxFlags("Use crop 0 alpha", sliceCastData.flags, 0x20000000u);
	
	//edited |= Editor("Crop Index 0", sliceCastData.cropIndex0);

	resources::ManagedCArray cropRefs0{ resource, sliceCastData.cropRefs0, sliceCastData.cropRef0Count };

	edited |= Editor("Crop References 0", cropRefs0);

	edited |= CheckboxFlags("Unknown 1", sliceCastData.flags, 0x2000u);

#ifdef DEVTOOLS_TARGET_SDK_rangers
	ImGui::Separator();
	auto effectType = sliceCastData.GetEffectType();
	if (ComboEnum("Effect type", effectType, effectTypeNames)) {
		edited = true;
		sliceCastData.SetEffectType(effectType);

		auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		if (sliceCastData.effectData.blur)
			allocator.Free(sliceCastData.effectData.blur);

		switch (effectType) {
		case EEffectType::BLUR: sliceCastData.effectData.blur = new (&allocator) SRS_BLUR3D{}; break;
		case EEffectType::REFLECT: sliceCastData.effectData.reflect = new (&allocator) SRS_REFLECT3D{}; break;
		default: sliceCastData.effectData.blur = nullptr; break;
		}
	}

	if (sliceCastData.effectData.blur) {
		switch (effectType) {
		case EEffectType::BLUR: edited |= Editor("Effect parameters", *sliceCastData.effectData.blur); break;
		case EEffectType::REFLECT: edited |= Editor("Effect parameters", *sliceCastData.effectData.reflect); break;
		}
	}
#endif

	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::fnd::ManagedResource* resource, SRS_DATA& data)
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		InputText("Name", data.name, resource);

		if (ComboEnum("Type", data.type, userDataTypes)) {
			auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

			managedAllocator.Free(data.value.b);

			switch (data.type) {
			case SRS_DATA::Type::BOOL: data.value.b = new (&managedAllocator) bool{}; break;
			case SRS_DATA::Type::INT: data.value.i32 = new (&managedAllocator) int{}; break;
			case SRS_DATA::Type::UINT: data.value.u32 = new (&managedAllocator) unsigned int{}; break;
			case SRS_DATA::Type::FLOAT: data.value.f32 = new (&managedAllocator) float{}; break;
			case SRS_DATA::Type::STRING: data.value.str = ""; break;
			default: break;
			}
		}

		switch (data.type) {
		case SRS_DATA::Type::BOOL: edited |= Editor("Value", *data.value.b); break;
		case SRS_DATA::Type::INT: edited |= Editor("Value", *data.value.i32); break;
		case SRS_DATA::Type::UINT: edited |= Editor("Value", *data.value.u32); break;
		case SRS_DATA::Type::FLOAT: edited |= Editor("Value", *data.value.f32); break;
		case SRS_DATA::Type::STRING: edited |= InputText("Value", data.value.str, resource); break;
		default: Viewer("Value", data.value.i32); break;
		}

		ImGui::TreePop();
	}
	return edited;
}

void InitData(hh::fnd::ManagedResource* resource, SRS_DATA& data) {
	data.name = "New data";
	data.type = SRS_DATA::Type::STRING;
	data.value.str = "";
}

void DeinitData(hh::fnd::ManagedResource* resource, SRS_DATA& data) {
	auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

	managedAllocator.Free(const_cast<char*>(data.name));
	managedAllocator.Free(data.value.i32);
}

bool Editor(const char* label, hh::fnd::ManagedResource* resource, SRS_USERDATA& userData)
{
	bool edited{};
	resources::ManagedCArray items{ resource, userData.items, userData.count };

	edited |= Editor(label, resource, items, &InitData, &DeinitData);

	return edited;
}

bool Editor(const char* label, hh::fnd::ManagedResource* resource, SRS_USERDATA*& userData)
{
	bool edited{};
	
	ImGui::SeparatorText(label);
	if (userData != nullptr) {
		edited |= Editor(label, resource, *userData);
		if (ImGui::Button("Remove all user data")) {
			resources::ManagedAllocator managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

			resources::ManagedCArray items{ resource, userData->items, userData->count };

			for (auto& item : items)
				DeinitData(resource, item);

			managedAllocator.Free(userData->items);

			userData = nullptr;
		}
	}
	else {
		if (ImGui::Button("Add user data")) {
			resources::ManagedAllocator managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

			userData = new (&managedAllocator) SRS_USERDATA{};

			edited = true;
		}
	}

	return edited;
}

bool Editor(const char* label, SurfRide::SRS_CROPREF& cropRef);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SurfRide::SRS_DATA& data);

#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <resources/ManagedMemoryRegistry.h>
#include <ui/operation-modes/modes/surfride-editor/texture-editor/References.h>
#include "SurfRide.h"

using namespace SurfRide;

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

#ifdef DEVTOOLS_TARGET_SDK_rangers
const char* blendModeNames[]{ "Default", "Add", "Mode2", "Mode3" };
bool Editor(const char* label, SRS_BLUR3D& effect) {
	bool edited{};
	edited |= Editor("Unk1", effect.unk1);
	edited |= Editor("Unk2", effect.unk2);
	edited |= Editor("Crop count", effect.cropCount);
	edited |= Editor("Steps", effect.steps);
	edited |= Editor("Duration", effect.duration);
	edited |= Editor("Color", effect.color);

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

	auto blendMode = effect.GetBlendMode();
	if (ComboEnum("Blend mode", blendMode, blendModeNames)) {
		effect.SetBlendMode(blendMode);
		edited = true;
	}

	return edited;
}
#endif

const char* verticalAlignmentNames[]{ "Top", "Center", "Bottom" };
bool Editor(const char* label, SRS_TEXTDATA& textData) {
	bool edited{};

	auto verticalAlignment = textData.GetVerticalAlignment();
	if (ComboEnum("Pivot type", verticalAlignment, verticalAlignmentNames)) {
		textData.SetVerticalAlignment(verticalAlignment);
		edited = true;
	}
	Viewer("Text", textData.str);
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

const char* pivotTypeNames[]{ "Top Left", "Top Center", "Top Right", "Center Left", "Center Center", "Center Right", "Bottom Left", "Bottom Center", "Bottom Right", "Custom" };
const char* orientationNames[]{ "Up", "Left", "Down", "Right" };
const char* effectTypeNames[]{ "None", "Blur", "Reflect" };
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

	if (pivotType == SurfRide::EPivotType::CUSTOM)
		edited |= Editor("Pivot", imageCastData.pivot);

	ImGui::Separator();
	//edited |= CheckboxFlags("Disable Texture Color", imageCastData.flags, 0x1u);
	//edited |= CheckboxFlags("Disable Material Color", imageCastData.flags, 0x2u);
	//edited |= CheckboxFlags("Disable Illumination Color", imageCastData.flags, 0x4u);
	edited |= Editor("Vertex Color Top Left", imageCastData.vertexColorTopLeft);
	edited |= Editor("Vertex Color Bottom Left", imageCastData.vertexColorBottomLeft);
	edited |= Editor("Vertex Color Top Right", imageCastData.vertexColorTopRight);
	edited |= Editor("Vertex Color Bottom Right", imageCastData.vertexColorBottomRight);

	ImGui::Separator();
	auto orientation = imageCastData.GetOrientation();
	if (ComboEnum("Orientation", orientation, orientationNames)) {
		imageCastData.SetOrientation(orientation);
		edited = true;
	}
	edited |= CheckboxFlags("Flip horizontally", imageCastData.flags, 0x10u);
	edited |= CheckboxFlags("Flip vertically", imageCastData.flags, 0x20u);
	edited |= Editor("Crop Index 0", imageCastData.cropIndex0);
	edited |= Editor("Crop Index 1", imageCastData.cropIndex1);

	resources::ManagedCArray cropRefs0{ resource, imageCastData.cropRefs0, imageCastData.cropRef0Count };
	resources::ManagedCArray cropRefs1{ resource, imageCastData.cropRefs1, imageCastData.cropRef1Count };

	edited |= Editor("Crop References 0", cropRefs0);
	edited |= Editor("Crop References 1", cropRefs1);

	if (imageCastData.textData)
		edited |= Editor("Text Data", *imageCastData.textData);

#ifdef DEVTOOLS_TARGET_SDK_rangers
	ImGui::Separator();
	auto effectType = imageCastData.GetEffectType();
	if (ComboEnum("Effect type", effectType, effectTypeNames)) {
		edited = true;
		imageCastData.SetEffectType(effectType);

		auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		if (imageCastData.effectData)
			allocator.Free(imageCastData.effectData);
	
		switch (effectType) {
		case SRE_EFFECT_TYPE::BLUR: imageCastData.effectData = new (&allocator) SRS_BLUR3D{}; break;
		case SRE_EFFECT_TYPE::REFLECT: imageCastData.effectData = new (&allocator) SRS_REFLECT3D{}; break;
		default: imageCastData.effectData = nullptr; break;
		}
	}

	if (imageCastData.effectData) {
		switch (effectType) {
		case SRE_EFFECT_TYPE::BLUR: edited |= Editor("Effect parameters", *reinterpret_cast<SRS_BLUR3D*>(imageCastData.effectData)); break;
		case SRE_EFFECT_TYPE::REFLECT: edited |= Editor("Effect parameters", *reinterpret_cast<SRS_REFLECT3D*>(imageCastData.effectData)); break;
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

	//edited |= CheckboxFlags("Disable Texture Color", sliceCastData.flags, 0x1u);
	//edited |= CheckboxFlags("Disable Material Color", sliceCastData.flags, 0x2u);
	//edited |= CheckboxFlags("Disable Illumination Color", sliceCastData.flags, 0x4u);
	edited |= CheckboxFlags("Flip horizontally", sliceCastData.flags, 0x10u);
	edited |= CheckboxFlags("Flip vertically", sliceCastData.flags, 0x20u);
	edited |= Editor("Fixed size", sliceCastData.fixedSize);
	edited |= Editor("Vertex Color Top Left", sliceCastData.vertexColorTopLeft);
	edited |= Editor("Vertex Color Bottom Left", sliceCastData.vertexColorBottomLeft);
	edited |= Editor("Vertex Color Top Right", sliceCastData.vertexColorTopRight);
	edited |= Editor("Vertex Color Bottom Right", sliceCastData.vertexColorBottomRight);

#ifdef DEVTOOLS_TARGET_SDK_rangers
	ImGui::Separator();
	auto effectType = sliceCastData.GetEffectType();
	if (ComboEnum("Effect type", effectType, effectTypeNames)) {
		edited = true;
		sliceCastData.SetEffectType(effectType);

		auto allocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

		if (sliceCastData.effectData)
			allocator.Free(sliceCastData.effectData);

		switch (effectType) {
		case SRE_EFFECT_TYPE::BLUR: sliceCastData.effectData = new (&allocator) SRS_BLUR3D{}; break;
		case SRE_EFFECT_TYPE::REFLECT: sliceCastData.effectData = new (&allocator) SRS_REFLECT3D{}; break;
		default: sliceCastData.effectData = nullptr; break;
		}
	}

	if (sliceCastData.effectData) {
		switch (effectType) {
		case SRE_EFFECT_TYPE::BLUR: edited |= Editor("Effect parameters", *reinterpret_cast<SRS_BLUR3D*>(sliceCastData.effectData)); break;
		case SRE_EFFECT_TYPE::REFLECT: edited |= Editor("Effect parameters", *reinterpret_cast<SRS_REFLECT3D*>(sliceCastData.effectData)); break;
		}
	}
#endif

	ImGui::PopID();
	return edited;
}

const char* userDataTypes[]{ "Boolean", "Signed integer", "Unsigned integer", "Float", "Unknown", "String"};
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SRS_DATA& data)
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		InputText("Name", data.name, resource);

		if (ComboEnum("Type", data.type, userDataTypes)) {
			auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

			managedAllocator.Free(data.value);

			switch (data.type) {
			case SRS_DATA::Type::BOOL: data.value = new (&managedAllocator) bool{}; break;
			case SRS_DATA::Type::INT: data.value = new (&managedAllocator) int{}; break;
			case SRS_DATA::Type::UINT: data.value = new (&managedAllocator) unsigned int{}; break;
			case SRS_DATA::Type::FLOAT: data.value = new (&managedAllocator) float{}; break;
			case SRS_DATA::Type::STRING: data.value = const_cast<char*>(""); break;
			default: break;
			}
		}

		switch (data.type) {
		case SRS_DATA::Type::BOOL: edited |= Editor("Value", *static_cast<bool*>(data.value)); break;
		case SRS_DATA::Type::INT: edited |= Editor("Value", *static_cast<int*>(data.value)); break;
		case SRS_DATA::Type::UINT: edited |= Editor("Value", *static_cast<unsigned int*>(data.value)); break;
		case SRS_DATA::Type::FLOAT: edited |= Editor("Value", *static_cast<float*>(data.value)); break;
		case SRS_DATA::Type::STRING: edited |= InputText("Value", const_cast<const char*&>(reinterpret_cast<char*&>(static_cast<void*&>(data.value))), resource); break;
		default: Viewer("Value", static_cast<void*>(data.value)); break;
		}

		ImGui::TreePop();
	}
	return edited;
}

void InitData(hh::fnd::ManagedResource* resource, SRS_DATA& data) {
	data.name = "New data";
	data.type = SRS_DATA::Type::STRING;
	data.value = const_cast<char*>("");
}

void DeinitData(hh::fnd::ManagedResource* resource, SRS_DATA& data) {
	auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(resource);

	managedAllocator.Free(const_cast<char*>(data.name));
	managedAllocator.Free(data.value);
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

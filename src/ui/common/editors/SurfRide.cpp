#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
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

bool Editor(const char* label, Transform& transform) {
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Position", transform.position);
	edited |= Editor("Rotation", transform.rotation);
	edited |= Editor("Scale", transform.scale);
	edited |= Editor("Material color", transform.materialColor);
	edited |= Editor("Illumination color", transform.illuminationColor);
	edited |= Editor("Display", transform.display);
	ImGui::PopID();
	return edited;
}

const char* effectTypeNames[]{ "None", "Blur", "Reflect" };
bool Editor(const char* label, SRS_IMAGECAST& imageCastData) {
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Size", imageCastData.size);
	edited |= Editor("Pivot", imageCastData.pivot);
	edited |= Editor("Vertex Color Top Left", imageCastData.vertexColorTopLeft);
	edited |= Editor("Vertex Color Bottom Left", imageCastData.vertexColorBottomLeft);
	edited |= Editor("Vertex Color Top Right", imageCastData.vertexColorTopRight);
	edited |= Editor("Vertex Color Bottom Right", imageCastData.vertexColorBottomRight);
	edited |= Editor("Crop Index 0", imageCastData.cropIndex0);
	edited |= Editor("Crop Index 1", imageCastData.cropIndex1);
	//edited |= Editor("Text Data", *imageCastData.textData);
	edited |= ComboEnum("Effect type", *reinterpret_cast<SRE_EFFECT_TYPE*>(&imageCastData.effectType), effectTypeNames);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, SurfRide::SRS_REFERENCECAST& referenceCastData)
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
		for (size_t i = 0; i < referenceCastData.layer->animationCount; i++) {
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

bool Editor(const char* label, SurfRide::SRS_SLICECAST& sliceCastData)
{
	bool edited{};
	ImGui::PushID(label);
	ImGui::SeparatorText(label);
	edited |= Editor("Size", sliceCastData.size);
	edited |= Editor("Pivot", sliceCastData.pivot);
	edited |= Editor("Fixed size", sliceCastData.fixedSize);
	edited |= Editor("Vertex Color Top Left", sliceCastData.vertexColorTopLeft);
	edited |= Editor("Vertex Color Bottom Left", sliceCastData.vertexColorBottomLeft);
	edited |= Editor("Vertex Color Top Right", sliceCastData.vertexColorTopRight);
	edited |= Editor("Vertex Color Bottom Right", sliceCastData.vertexColorBottomRight);
	ImGui::PopID();
	return edited;
}

const char* userDataTypes[]{ "Boolean", "Signed integer", "Unsigned integer", "Float", "Unknown", "String"};
bool Editor(const char* label, SurfRide::SRS_DATA& data)
{
	bool edited{};
	if (ImGui::TreeNode(label)) {
		Viewer("Name", data.name);
		ComboEnum("Type", data.type, userDataTypes);

		switch (data.type) {
		case SRS_DATA::Type::BOOL: edited |= Editor("Value", *reinterpret_cast<bool*>(data.value)); break;
		case SRS_DATA::Type::INT: edited |= Editor("Value", *reinterpret_cast<int*>(data.value)); break;
		case SRS_DATA::Type::UINT: edited |= Editor("Value", *reinterpret_cast<unsigned int*>(data.value)); break;
		case SRS_DATA::Type::FLOAT: edited |= Editor("Value", *reinterpret_cast<float*>(data.value)); break;
		case SRS_DATA::Type::STRING: Viewer("Value", reinterpret_cast<const char*>(data.value)); break;
		default: Viewer("Value", reinterpret_cast<void*>(data.value)); break;
		}

		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, SurfRide::SRS_USERDATA& userData)
{
	bool edited{};
	ImGui::PushID(label);

	if (ImGui::CollapsingHeader(label))
		for (size_t i = 0; i < userData.count; i++)
			edited |= Editor(userData.items[i].name, userData.items[i]);

	ImGui::PopID();
	return edited;
}

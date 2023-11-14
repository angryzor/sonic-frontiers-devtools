#include "../Pch.h"
#include "ReflectionEditor.h"
#include "../common/SimpleWidgets.h"
#include "../imgui/imgui_internal.h"

using namespace hh::fnd;


const char* ReflectionEditor::GetRflMemberName(const RflClassMember* member) {
	const char* caption = *reinterpret_cast<const char* const*>(member->GetAttribute("Caption")->GetData());
	const char* name = member->GetName();

	return name;// caption != nullptr ? caption : name;
}

// void EnumEditor(void* obj, const RflClassMember* member) {

// 	ImGui::BeginCombo(GetRflMemberName(member), )
// }
int64_t ReflectionEditor::ReadPrimitiveInt(void* obj, const hh::fnd::RflClassMember::Type type) {
	switch (type) {
	case RflClassMember::TYPE_SINT8:
		return *static_cast<int8_t*>(obj);
	case RflClassMember::TYPE_UINT8:
		return *static_cast<uint8_t*>(obj);
	case RflClassMember::TYPE_SINT16:
		return *static_cast<int16_t*>(obj);
	case RflClassMember::TYPE_UINT16:
		return *static_cast<uint16_t*>(obj);
	case RflClassMember::TYPE_SINT32:
		return *static_cast<int32_t*>(obj);
	case RflClassMember::TYPE_UINT32:
		return *static_cast<uint32_t*>(obj);
	case RflClassMember::TYPE_SINT64:
		return *static_cast<int64_t*>(obj);
	case RflClassMember::TYPE_UINT64:
		return *static_cast<uint64_t*>(obj);
	default:
		assert("Unknown primitive int");
		return 0;
	}
}

void ReflectionEditor::WritePrimitiveInt(void* obj, int64_t value, const hh::fnd::RflClassMember::Type type) {
	switch (type) {
	case RflClassMember::TYPE_SINT8:
		*static_cast<int8_t*>(obj) = static_cast<int8_t>(value);
	case RflClassMember::TYPE_UINT8:
		*static_cast<uint8_t*>(obj) = static_cast<uint8_t>(value);
	case RflClassMember::TYPE_SINT16:
		*static_cast<int16_t*>(obj) = static_cast<int16_t>(value);
	case RflClassMember::TYPE_UINT16:
		*static_cast<uint16_t*>(obj) = static_cast<uint16_t>(value);
	case RflClassMember::TYPE_SINT32:
		*static_cast<int32_t*>(obj) = static_cast<int32_t>(value);
	case RflClassMember::TYPE_UINT32:
		*static_cast<uint32_t*>(obj) = static_cast<uint32_t>(value);
	case RflClassMember::TYPE_SINT64:
		*static_cast<int64_t*>(obj) = static_cast<int64_t>(value);
	case RflClassMember::TYPE_UINT64:
		*static_cast<uint64_t*>(obj) = static_cast<uint64_t>(value);
	default:
		assert("Unknown primitive int");
	}
}

void ReflectionEditor::RenderStubRflParamEditor(const char* name, const RflClassMember* member) {
	ImGui::Text(name);
	ImGui::SameLine();
	ImGui::Text("Coming soon!");
}

void ReflectionEditor::RenderEnumEditor(const char* name, void* obj, const RflClassMember* member) {
	const RflClassEnum* enumClass = member->GetEnumClass();
	
	static const char* invalid = "<INVALID VALUE>";
	const char** previewValue{ &invalid };
	int64_t currentValue = ReadPrimitiveInt(obj, member->GetSubType());
	enumClass->GetEnglishNameOfValue(currentValue, previewValue);

	if (ImGui::BeginCombo(name, *previewValue)) {
		auto* values = enumClass->GetValues();

		for (size_t i = 0; i < enumClass->GetValueCount(); i++) {
			bool is_selected = currentValue == values[i].GetIndex();

			if (ImGui::Selectable(values[i].GetEnglishName(), is_selected))
				WritePrimitiveInt(obj, values[i].GetIndex(), member->GetSubType());

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}

void ReflectionEditor::RenderFlagsEditor(const char* name, void* obj, const RflClassMember* member) {
	auto* enumEntries = reinterpret_cast<const RflArray<RflClassEnumMember>*>(member->GetAttribute("DisplayIndex")->GetData());
	int64_t currentValue = ReadPrimitiveInt(obj, member->GetSubType());

	if (ImGui::TreeNode(name)) {
		auto* values = enumEntries->items;

		for (size_t i = 0; i < enumEntries->count; i++) {
			ImGui::CheckboxFlags(values[i].GetEnglishName(), &currentValue, static_cast<size_t>(1) << values[i].GetIndex());
		}

		ImGui::TreePop();
	}
}

void ReflectionEditor::RflClassMemberEditor(const char* name, void* obj, const RflClassMember* member) {
	switch (member->m_Type) {
	case RflClassMember::TYPE_BOOL:
		ImGui::Checkbox(name, static_cast<bool*>(obj));
		break;
	case RflClassMember::TYPE_SINT8:
		RenderScalarRflParamEditor<int8_t>(name, obj, member, ImGuiDataType_S8, 1);
		break;
	case RflClassMember::TYPE_UINT8:
		RenderScalarRflParamEditor<uint8_t>(name, obj, member, ImGuiDataType_U8, 1);
		break;
	case RflClassMember::TYPE_SINT16:
		RenderScalarRflParamEditor<int16_t>(name, obj, member, ImGuiDataType_S16, 1);
		break;
	case RflClassMember::TYPE_UINT16:
		RenderScalarRflParamEditor<uint16_t>(name, obj, member, ImGuiDataType_U16, 1);
		break;
	case RflClassMember::TYPE_SINT32:
		RenderScalarRflParamEditor<int32_t>(name, obj, member, ImGuiDataType_S32, 1, "RangeSint32");
		break;
	case RflClassMember::TYPE_UINT32:
		RenderScalarRflParamEditor<uint32_t>(name, obj, member, ImGuiDataType_U32, 1);
		break;
	case RflClassMember::TYPE_SINT64:
		RenderScalarRflParamEditor<int64_t>(name, obj, member, ImGuiDataType_S64, 1);
		break;
	case RflClassMember::TYPE_UINT64:
		RenderScalarRflParamEditor<uint64_t>(name, obj, member, ImGuiDataType_U64, 1);
		break;
	case RflClassMember::TYPE_FLOAT:
		RenderScalarRflParamEditor<float>(name, obj, member, ImGuiDataType_Float, 1, "RangeFloat");
		break;
	case RflClassMember::TYPE_VECTOR2:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector2>(name, obj, member, ImGuiDataType_Float, 2, "RangeFloat");
		break;
	case RflClassMember::TYPE_VECTOR3:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(name, obj, member, ImGuiDataType_Float, 3, "RangeFloat");
		break;
	case RflClassMember::TYPE_VECTOR4:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4, "RangeFloat");
		break;
	case RflClassMember::TYPE_QUATERNION:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4, "RangeFloat");
		break;
	case RflClassMember::TYPE_MATRIX34:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		break;
	case RflClassMember::TYPE_MATRIX44:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		break;
	case RflClassMember::TYPE_POINTER:
		RenderStubRflParamEditor(name, member);
		break;
	case RflClassMember::TYPE_ARRAY:
		RenderStubRflParamEditor(name, member);
		break;
	case RflClassMember::TYPE_SIMPLE_ARRAY:
		RenderStubRflParamEditor(name, member);
		break;
	case RflClassMember::TYPE_ENUM:
		RenderEnumEditor(name, obj, member);
		break;
	case RflClassMember::TYPE_STRUCT:
		RenderStructRflParamEditor(name, obj, member->m_pClass);
		break;
	case RflClassMember::TYPE_FLAGS:
		RenderFlagsEditor(name, obj, member);
		break;
	case RflClassMember::TYPE_CSTRING:
		ImGui::Text("%s: %s", name, static_cast<const char*>(obj));
		break;
	case RflClassMember::TYPE_STRING:
		InputText(name, static_cast<csl::ut::VariableString*>(obj));
		break;
	case RflClassMember::TYPE_OBJECT_ID:
		RenderStubRflParamEditor(name, member);
		break;
	case RflClassMember::TYPE_POSITION:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(name, obj, member, ImGuiDataType_Float, 3, "RangeFloat");
		break;
	case RflClassMember::TYPE_COLOR_BYTE: {
		auto originalColor{ static_cast<csl::ut::Color<uint8_t>*>(obj) };
		float colorAsFloat[]{
			static_cast<float>(originalColor->r) / 255,
			static_cast<float>(originalColor->g) / 255,
			static_cast<float>(originalColor->b) / 255,
			static_cast<float>(originalColor->a) / 255,
		};
		float editableColor[4]{ colorAsFloat[0], colorAsFloat[1], colorAsFloat[2], colorAsFloat[3] };

		ImGui::ColorEdit4(name, editableColor, ImGuiColorEditFlags_Uint8);

		if (colorAsFloat[0] != editableColor[0] || colorAsFloat[1] != editableColor[1] || colorAsFloat[2] != editableColor[2] || colorAsFloat[3] != editableColor[3]) {
			originalColor->r = static_cast<uint8_t>(editableColor[0] * 255);
			originalColor->g = static_cast<uint8_t>(editableColor[1] * 255);
			originalColor->b = static_cast<uint8_t>(editableColor[2] * 255);
			originalColor->a = static_cast<uint8_t>(editableColor[3] * 255);
		}

		break;
	}
	case RflClassMember::TYPE_COLOR_FLOAT:
		ImGui::ColorEdit4(name, static_cast<float*>(obj), ImGuiColorEditFlags_Float);
		break;
	}
}

void ReflectionEditor::RflClassMembersEditor(void* obj, const RflClass* rflClass) {
	const RflClass* parent = rflClass->GetBaseType();

	if (parent != nullptr) {
		RflClassMembersEditor(obj, parent);
	}

	for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
		const RflClassMember* member = &rflClass->m_pMembers.items[i];
		size_t constArrSizeOrZero = member->GetCstyleArraySize();
		size_t constArrSize = constArrSizeOrZero == 0 ? 1 : constArrSizeOrZero;
		size_t baseAddr = reinterpret_cast<size_t>(obj) + member->m_Offset;
		auto memberName = GetRflMemberName(member);

		ImGui::PushID(static_cast<int>(i));
		if (constArrSize == 1) {
			RflClassMemberEditor(memberName, reinterpret_cast<void*>(baseAddr), member);
		}
		else {
			if (ImGui::TreeNode("Array", "%s[]", memberName)) {
				for (size_t j = 0; j < constArrSize; j++) {
					char name[200] = "";
					snprintf(name, 200, "%s[%zd]", memberName, j);

					ImGui::PushID(static_cast<int>(j));
					RflClassMemberEditor(name, reinterpret_cast<void*>(baseAddr + j * member->GetSingleSizeInBytes()), member);
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}
		ImGui::PopID();
	}
}

void ReflectionEditor::RenderStructRflParamEditor(const char* name, void* obj, const RflClass* rflClass) {
	if (ImGui::TreeNode(name)) {
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		RflClassMembersEditor(obj, rflClass);
		ImGui::TreePop();
	}
}

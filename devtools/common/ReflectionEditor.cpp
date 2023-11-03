#include "../Pch.h"
#include "ReflectionEditor.h"

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

void ReflectionEditor::RenderStubRflParamEditor(const RflClassMember* member) {
	ImGui::Text(GetRflMemberName(member));
	ImGui::SameLine();
	ImGui::Text("Coming soon!");
}

void ReflectionEditor::RenderEnumEditor(void* obj, const RflClassMember* member) {
	const RflClassEnum* enumClass = member->GetEnumClass();
	
	static const char* invalid = "<INVALID VALUE>";
	const char** previewValue{ &invalid };
	int64_t currentValue = ReadPrimitiveInt(obj, member->GetSubType());
	enumClass->GetEnglishNameOfValue(currentValue, previewValue);

	if (ImGui::BeginCombo(GetRflMemberName(member), *previewValue)) {
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

void ReflectionEditor::RflClassMembersEditor(void* obj, const RflClass* rflClass) {
	const RflClass* parent = rflClass->GetBaseType();

	if (parent != nullptr) {
		RflClassMembersEditor(obj, parent);
	}

	for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
		const RflClassMember* member = &rflClass->m_pMembers.items[i];
		size_t constArrSizeOrZero = member->GetCstyleArraySize();
		size_t constArrSize = constArrSizeOrZero == 0 ? 1 : constArrSizeOrZero;

		ImGui::PushID(static_cast<int>(i));
		for (size_t j = 0; j < constArrSize; j++) {
			void* address = reinterpret_cast<void*>(reinterpret_cast<size_t>(obj) + member->m_Offset + j * member->GetSingleSizeInBytes());

			ImGui::PushID(static_cast<int>(j));
			switch (member->m_Type) {
			case RflClassMember::TYPE_BOOL:
				ImGui::Checkbox(GetRflMemberName(member), static_cast<bool*>(address));
				break;
			case RflClassMember::TYPE_SINT8:
				RenderScalarRflParamEditor<int8_t>(address, member, ImGuiDataType_S8, 1);
				break;
			case RflClassMember::TYPE_UINT8:
				RenderScalarRflParamEditor<uint8_t>(address, member, ImGuiDataType_U8, 1);
				break;
			case RflClassMember::TYPE_SINT16:
				RenderScalarRflParamEditor<int16_t>(address, member, ImGuiDataType_S16, 1);
				break;
			case RflClassMember::TYPE_UINT16:
				RenderScalarRflParamEditor<uint16_t>(address, member, ImGuiDataType_U16, 1);
				break;
			case RflClassMember::TYPE_SINT32:
				RenderScalarRflParamEditor<int32_t>(address, member, ImGuiDataType_S32, 1, "RangeSint32");
				break;
			case RflClassMember::TYPE_UINT32:
				RenderScalarRflParamEditor<uint32_t>(address, member, ImGuiDataType_U32, 1);
				break;
			case RflClassMember::TYPE_SINT64:
				RenderScalarRflParamEditor<int64_t>(address, member, ImGuiDataType_S64, 1);
				break;
			case RflClassMember::TYPE_UINT64:
				RenderScalarRflParamEditor<uint64_t>(address, member, ImGuiDataType_U64, 1);
				break;
			case RflClassMember::TYPE_FLOAT:
				RenderScalarRflParamEditor<float>(address, member, ImGuiDataType_Float, 1, "RangeFloat");
				break;
			case RflClassMember::TYPE_VECTOR2:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector2>(address, member, ImGuiDataType_Float, 2, "RangeFloat");
				break;
			case RflClassMember::TYPE_VECTOR3:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(address, member, ImGuiDataType_Float, 3, "RangeFloat");
				break;
			case RflClassMember::TYPE_VECTOR4:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4, "RangeFloat");
				break;
			case RflClassMember::TYPE_QUATERNION:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4, "RangeFloat");
				break;
			case RflClassMember::TYPE_MATRIX34:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				break;
			case RflClassMember::TYPE_MATRIX44:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(address, member, ImGuiDataType_Float, 4);
				break;
			case RflClassMember::TYPE_POINTER:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_ARRAY:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_SIMPLE_ARRAY:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_ENUM:
				RenderEnumEditor(address, member);
				break;
			case RflClassMember::TYPE_STRUCT:
				RenderStructRflParamEditor(address, GetRflMemberName(member), member->m_pClass);
				break;
			case RflClassMember::TYPE_FLAGS:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_CSTRING:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_STRING:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_OBJECT_ID:
				RenderStubRflParamEditor(member);
				break;
			case RflClassMember::TYPE_POSITION:
				RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(address, member, ImGuiDataType_Float, 3, "RangeFloat");
				break;
			case RflClassMember::TYPE_COLOR_BYTE:
				RenderStubRflParamEditor(member);
				//ImGui::ColorEdit4(GetRflMemberName(member), static_cast<float*>(address), ImGuiColorEditFlags_Uint8);
				break;
			case RflClassMember::TYPE_COLOR_FLOAT:
				ImGui::ColorEdit4(GetRflMemberName(member), static_cast<float*>(address), ImGuiColorEditFlags_Uint8);
				break;
			}
			ImGui::PopID();
		}
		ImGui::PopID();
	}
}

void ReflectionEditor::RenderStructRflParamEditor(void* obj, const char* name, const RflClass* rflClass) {
	if (ImGui::TreeNode(name)) {
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		RflClassMembersEditor(obj, rflClass);
		ImGui::TreePop();
	}
}

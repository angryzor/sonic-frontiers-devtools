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

void ReflectionEditor::RenderStubRflParamEditor(const RflClassMember* member) {
	ImGui::Text(GetRflMemberName(member));
	ImGui::SameLine();
	ImGui::Text("Coming soon!");
}

void ReflectionEditor::RflClassMembersEditor(void* obj, const RflClass* rflClass) {
	const RflClass* parent = rflClass->GetBaseType();

	if (parent != nullptr) {
		RflClassMembersEditor(obj, parent);
		obj = reinterpret_cast<void*>(reinterpret_cast<size_t>(obj) + parent->GetSizeInBytes());
	}

	for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
		const RflClassMember* member = &rflClass->m_pMembers.items[i];

		void* address = reinterpret_cast<void*>(reinterpret_cast<size_t>(obj) + member->m_Offset);

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
			RenderStubRflParamEditor(member);
			break;
		case RflClassMember::TYPE_MATRIX44:
			RenderStubRflParamEditor(member);
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
			RenderStubRflParamEditor(member);
			// EnumEditor(address, member);
			break;
		case RflClassMember::TYPE_STRUCT:
			RenderStructRflParamEditor(address, GetRflMemberName(member), member->m_pClass);
			break;
		case RflClassMember::TYPE_FLAGS:
			ImGui::Text(GetRflMemberName(member));
			ImGui::Text("Coming soon!");
			break;
		case RflClassMember::TYPE_CSTRING:
			ImGui::Text(GetRflMemberName(member));
			ImGui::Text("Coming soon!");
			break;
		case RflClassMember::TYPE_STRING:
			ImGui::Text(GetRflMemberName(member));
			ImGui::Text("Coming soon!");
			break;
		case RflClassMember::TYPE_OBJECT_ID:
			ImGui::Text(GetRflMemberName(member));
			ImGui::Text("Coming soon!");
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
	}
}

void ReflectionEditor::RenderStructRflParamEditor(void* obj, const char* name, const RflClass* rflClass) {
	if (ImGui::TreeNode(name)) {
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		RflClassMembersEditor(obj, rflClass);
		ImGui::TreePop();
	}
}

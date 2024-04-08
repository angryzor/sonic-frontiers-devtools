#include "ReflectionEditor.h"
#include "SimpleWidgets.h"
#include "Translations.h"
#include <imgui_internal.h>

using namespace hh::fnd;

class RflMoveArray : public csl::ut::MoveArray<void*> {
public:
	void change_allocator(csl::fnd::IAllocator* new_allocator, const RflClassMember* member)
	{
		if (!new_allocator)
		{
			return;
		}

		if (isUninitialized() || !this->m_pBuffer)
		{
			m_pAllocator = new_allocator;
			return;
		}

		if (m_pAllocator == new_allocator)
		{
			return;
		}

		size_t subTypeSize = member->GetSubTypeSizeInBytes();

		void* new_buffer = new_allocator->Alloc(this->capacity() * subTypeSize, 16);

		memcpy(new_buffer, this->m_pBuffer, subTypeSize * this->m_length);

		if (m_pAllocator && !isUninitialized())
		{
			m_pAllocator->Free(this->m_pBuffer);
		}

		m_pAllocator = new_allocator;
		this->m_pBuffer = static_cast<void**>(new_buffer);
	}

	void reserve(size_t len, const RflClassMember* member) {
		if (len <= capacity())
			return;

		size_t subTypeSize = member->GetSubTypeSizeInBytes();
		
		if (!m_pAllocator && !isUninitialized())
			change_allocator(hh::fnd::MemoryRouter::GetModuleAllocator(), member);

		void* new_buffer = m_pAllocator->Alloc(subTypeSize * len, 16);

		if (m_pBuffer)
		{
			memcpy(new_buffer, m_pBuffer, subTypeSize * m_length);
		}

		if (!isUninitialized())
		{
			m_pAllocator->Free(m_pBuffer);
		}

		m_capacity = len;
		m_pBuffer = static_cast<void**>(new_buffer);
	}

	void emplace_back(const RflClassMember* member) {
		size_t subTypeSize = member->GetSubTypeSizeInBytes();

		m_length++;

		if (m_length > capacity())
		{
			reserve(m_length * 2, member);
		}

		void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pBuffer) + subTypeSize * (m_length - 1));

		if (member->GetSubType() == RflClassMember::TYPE_STRUCT)
			RflTypeInfoRegistry::GetInstance()->ConstructObject(m_pAllocator, placement, member->GetStructClass()->GetName());
		else
			memset(placement, 0, subTypeSize);
	}

	void remove(size_t i, const RflClassMember* member) {
		size_t subTypeSize = member->GetSubTypeSizeInBytes();
		if (i >= m_length)
			return;

		void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pBuffer) + subTypeSize * i);

		if (member->GetSubType() == RflClassMember::TYPE_STRUCT)
			RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(placement, member->GetName());

		memmove(placement, reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pBuffer) + subTypeSize * (i + 1)), subTypeSize * (m_length - i - 1));

		m_length--;
	}
};


const char* ReflectionEditor::GetRflMemberName(const RflClassMember* member) {
	const char* caption = *reinterpret_cast<const char* const*>(member->GetAttribute("Caption")->GetData());
	const char* name = member->GetName();

	if (caption == nullptr)
		return name;
	
	const char* translation = Translations::GetTranslation(caption);

	return translation == nullptr ? name : translation;
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

void ReflectionEditor::RenderArrayRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member)
{
	auto* arr = static_cast<RflMoveArray*>(obj);
	void* buf = arr->begin();

	if (ImGui::TreeNode("MoveArray", "%s[0..]", name)) {
		for (int i = 0; i < arr->size(); i++) {
			ImGui::PushID(i);
			if (ImGui::Button("x"))
				arr->remove(i, member);
			ImGui::SameLine();

			char indexedName[200];
			snprintf(indexedName, sizeof(indexedName), "%s[%d]", name, i);
			RenderPrimitiveEditor(indexedName, reinterpret_cast<void*>(reinterpret_cast<size_t>(buf) + i * member->GetSubTypeSizeInBytes()), member, member->GetSubType());
			ImGui::PopID();
		}

		if (ImGui::Button("Add item"))
			arr->emplace_back(member);

		ImGui::TreePop();
	}
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

void ReflectionEditor::RenderPrimitiveEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, const hh::fnd::RflClassMember::Type type) {
	switch (type) {
	case RflClassMember::TYPE_BOOL:
		ImGui::Checkbox(name, static_cast<bool*>(obj));
		break;
	case RflClassMember::TYPE_SINT8:
		RenderScalarRflParamEditor<int8_t>(name, obj, member, ImGuiDataType_S8, 1, "RangeSint32");
		break;
	case RflClassMember::TYPE_UINT8:
		RenderScalarRflParamEditor<uint8_t>(name, obj, member, ImGuiDataType_U8, 1, "RangeUint32");
		break;
	case RflClassMember::TYPE_SINT16:
		RenderScalarRflParamEditor<int16_t>(name, obj, member, ImGuiDataType_S16, 1, "RangeSint32");
		break;
	case RflClassMember::TYPE_UINT16:
		RenderScalarRflParamEditor<uint16_t>(name, obj, member, ImGuiDataType_U16, 1, "RangeUint32");
		break;
	case RflClassMember::TYPE_SINT32:
		RenderScalarRflParamEditor<int32_t>(name, obj, member, ImGuiDataType_S32, 1, "RangeSint32");
		break;
	case RflClassMember::TYPE_UINT32:
		RenderScalarRflParamEditor<uint32_t>(name, obj, member, ImGuiDataType_U32, 1, "RangeUint32");
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
		RenderSliderlessScalarRflParamEditor<csl::math::Vector2>(name, obj, member, ImGuiDataType_Float, 2, "RangeVector2");
		break;
	case RflClassMember::TYPE_VECTOR3:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(name, obj, member, ImGuiDataType_Float, 3, "RangeVector3");
		break;
	case RflClassMember::TYPE_VECTOR4:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4, "RangeVector4");
		break;
	case RflClassMember::TYPE_QUATERNION:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, obj, member, ImGuiDataType_Float, 4);
		break;
	case RflClassMember::TYPE_MATRIX34:
		ImGui::PushID(name);
		ImGui::PushID("r1");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[0], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PushID("r2");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[1], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PushID("r3");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[2], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PushID("r4");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[3], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PopID();
		break;
	case RflClassMember::TYPE_MATRIX44:
		ImGui::PushID(name);
		ImGui::PushID("r1");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[0], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PushID("r2");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[1], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PushID("r3");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[2], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PushID("r4");
		RenderSliderlessScalarRflParamEditor<csl::math::Vector4>(name, &static_cast<csl::math::Vector4*>(obj)[3], member, ImGuiDataType_Float, 4);
		ImGui::PopID();
		ImGui::PopID();
		break;
	case RflClassMember::TYPE_POINTER:
		RenderStubRflParamEditor(name, member);
		break;
	case RflClassMember::TYPE_STRUCT:
		RenderStructRflParamEditor(name, obj, member->m_pClass);
		break;
	case RflClassMember::TYPE_CSTRING:
		ImGui::Text("%s: %s", name, static_cast<const char*>(obj));
		break;
	case RflClassMember::TYPE_STRING:
		InputText(name, static_cast<csl::ut::VariableString*>(obj));
		break;
	case RflClassMember::TYPE_OBJECT_ID:
		InputObjectId(name, static_cast<hh::game::ObjectId*>(obj));
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
	case RflClassMember::TYPE_POSITION:
		RenderSliderlessScalarRflParamEditor<csl::math::Vector3>(name, obj, member, ImGuiDataType_Float, 3, "RangeFloat");
		break;
	default:
		assert(!"rfl editor assertion failed: unknown primitive type");
		break;
	}
}

void ReflectionEditor::RflClassMemberEditor(const char* name, void* obj, const RflClassMember* member) {
	switch (member->GetType()) {
	case RflClassMember::TYPE_ARRAY:
		RenderArrayRflParamEditor(name, obj, member);
		break;
	case RflClassMember::TYPE_SIMPLE_ARRAY:
		RenderStubRflParamEditor(name, member);
		break;
	case RflClassMember::TYPE_ENUM:
		RenderEnumEditor(name, obj, member);
		break;
	case RflClassMember::TYPE_FLAGS:
		RenderFlagsEditor(name, obj, member);
		break;
	default:
		RenderPrimitiveEditor(name, obj, member, member->GetType());
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
			if (ImGui::TreeNode("Array", "%s[0..%zd]", memberName, constArrSize - 1)) {
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

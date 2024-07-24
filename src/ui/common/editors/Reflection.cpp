#include "Reflection.h"
#include <imgui_internal.h>
#include <ui/common/Translations.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/Desktop.h>
#include <reflection/ReflectiveOperations.h>
#include <reflection/RflRanges.h>

using namespace hh::fnd;

struct RflDragDropData {
	const RflClass* rflClass;
	void* obj;
};

class RenderStaticReflectionEditor {
public:
	constexpr static size_t arity = 1;
	static inline const char* currentMemberName{};
	static inline const hh::fnd::RflClassMember* currentMember{};
	static inline bool defaultOpen{};
	typedef bool result_type;

	template<typename T> static float rfl_step(const rfl_range_representation_t<T>& rangeValue) { return std::fmaxf(rflMinFloatStep, static_cast<float>(rangeValue)); }
	template<> float rfl_step<RangeVector2>(const rfl_range_representation_t<RangeVector2>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x()); }
	template<> float rfl_step<RangeVector3>(const rfl_range_representation_t<RangeVector3>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x()); }
	template<> float rfl_step<RangeVector4>(const rfl_range_representation_t<RangeVector4>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x()); }

	template<typename F>
	static bool NameScope(const char* memberName, F f) {
		const char* savedMemberName = currentMemberName;
		currentMemberName = memberName;
		bool edited = f();
		currentMemberName = savedMemberName;
		return edited;
	}

	template<typename F>
	static bool MemberScope(const hh::fnd::RflClassMember* member, F f) {
		return NameScope(member->m_pName, [&]() {
			const hh::fnd::RflClassMember* savedMember = currentMember;
			currentMember = member;
			bool edited = f();
			currentMember = savedMember;
			return edited;
		});
	}

	template<typename T, bool allowSliders = true>
	static bool InputRflScalar(T* obj) {
		if (const auto* rangeAttr = currentMember->GetAttribute(rfl_range_type_t<T>::name)) {
			auto rangeValues = reinterpret_cast<const rfl_range_representation_t<rfl_range_type_t<T>>*>(rangeAttr->GetData());
			auto rangeMin = static_cast<T>(rangeValues[0]);
			auto rangeMax = static_cast<T>(rangeValues[1]);
			auto rangeStep = rangeValues[2];

			if constexpr (allowSliders)
				if (rangeMax - rangeMin < rflSliderCutOff)
					return SliderScalar(currentMemberName, *obj, &rangeMin, &rangeMax);

			return DragScalar(currentMemberName, *obj, rfl_step<rfl_range_type_t<T>>(rangeStep), &rangeMin, &rangeMax);
		}
		else
			return DragScalar(currentMemberName, *obj);
	}

	static int64_t ReadPrimitiveInt(void* obj, const RflClassMember::Type type) {
		switch (type) {
		case RflClassMember::TYPE_SINT8: return *static_cast<int8_t*>(obj);
		case RflClassMember::TYPE_UINT8: return *static_cast<uint8_t*>(obj);
		case RflClassMember::TYPE_SINT16: return *static_cast<int16_t*>(obj);
		case RflClassMember::TYPE_UINT16: return *static_cast<uint16_t*>(obj);
		case RflClassMember::TYPE_SINT32: return *static_cast<int32_t*>(obj);
		case RflClassMember::TYPE_UINT32: return *static_cast<uint32_t*>(obj);
		case RflClassMember::TYPE_SINT64: return *static_cast<int64_t*>(obj);
		case RflClassMember::TYPE_UINT64: return *static_cast<uint64_t*>(obj);
		default: assert("Unknown primitive int"); return 0;
		}
	}

	static void WritePrimitiveInt(void* obj, int64_t value, const RflClassMember::Type type) {
		switch (type) {
		case RflClassMember::TYPE_SINT8: *static_cast<int8_t*>(obj) = static_cast<int8_t>(value); break;
		case RflClassMember::TYPE_UINT8: *static_cast<uint8_t*>(obj) = static_cast<uint8_t>(value); break;
		case RflClassMember::TYPE_SINT16: *static_cast<int16_t*>(obj) = static_cast<int16_t>(value); break;
		case RflClassMember::TYPE_UINT16: *static_cast<uint16_t*>(obj) = static_cast<uint16_t>(value); break;
		case RflClassMember::TYPE_SINT32: *static_cast<int32_t*>(obj) = static_cast<int32_t>(value); break;
		case RflClassMember::TYPE_UINT32: *static_cast<uint32_t*>(obj) = static_cast<uint32_t>(value); break;
		case RflClassMember::TYPE_SINT64: *static_cast<int64_t*>(obj) = static_cast<int64_t>(value); break;
		case RflClassMember::TYPE_UINT64: *static_cast<uint64_t*>(obj) = static_cast<uint64_t>(value); break;
		default: assert("Unknown primitive int"); break;
		}
	}

	static const char* GetRflMemberName(const RflClassMember* member) {
		const RflCustomAttribute* captionAttribute = member->GetAttribute("Caption");
		const char* caption = captionAttribute == nullptr ? nullptr : *reinterpret_cast<const char* const*>(captionAttribute->GetData());
		const char* name = member->GetName();

		if (caption == nullptr)
			return name;

		const char* translation = Translations::GetTranslation(caption);

		return translation == nullptr ? name : translation;
	}

	template<typename T>
	static bool VisitPrimitive(T* obj) { return Editor(currentMemberName, *obj); }
	static bool VisitPrimitive(int8_t* obj) { return InputRflScalar<int8_t>(obj); }
	static bool VisitPrimitive(uint8_t* obj) { return InputRflScalar<uint8_t>(obj); }
	static bool VisitPrimitive(int16_t* obj) { return InputRflScalar<int16_t>(obj); }
	static bool VisitPrimitive(uint16_t* obj) { return InputRflScalar<uint16_t>(obj); }
	static bool VisitPrimitive(int32_t* obj) { return InputRflScalar<int32_t>(obj); }
	static bool VisitPrimitive(uint32_t* obj) { return InputRflScalar<uint32_t>(obj); }
	static bool VisitPrimitive(int64_t* obj) { return InputRflScalar<int64_t>(obj); }
	static bool VisitPrimitive(uint64_t* obj) { return InputRflScalar<uint64_t>(obj); }
	static bool VisitPrimitive(float* obj) { return InputRflScalar<float>(obj); }
	static bool VisitPrimitive(csl::math::Vector2* obj) { return InputRflScalar<csl::math::Vector2, false>(obj); }
	static bool VisitPrimitive(csl::math::Vector3* obj) { return InputRflScalar<csl::math::Vector3, false>(obj); }
	static bool VisitPrimitive(csl::math::Vector4* obj) { return InputRflScalar<csl::math::Vector4, false>(obj); }
	static bool VisitPrimitive(const char** obj) { ImGui::Text("%s: %s", currentMemberName, *obj); return false; }

	template<typename F, template<typename> typename A, typename C, typename D>
	static bool VisitArray(RflArrayAccessor<A>& arr, C c, D d, F f) {
		bool edited{};
		if (ImGui::TreeNode(currentMemberName, "%s[0..]", currentMemberName)) {
			for (int i = 0; i < arr.size(); i++) {
				ImGui::PushID(i);
				if (ImGui::Button("x"))
					arr.remove(i);
				ImGui::SameLine();

				char indexedName[200];
				snprintf(indexedName, sizeof(indexedName), "%s[%d]", currentMemberName, i);

				edited |= NameScope(indexedName, [&]() { return f(arr[i]); });

				ImGui::PopID();
			}

			if (ImGui::Button("Add item"))
				arr.emplace_back();

			ImGui::TreePop();
		}
		return edited;
	}

	template<typename F>
	static bool VisitEnum(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
		bool edited{};
		static const char* invalid = "<INVALID VALUE>";
		const char** previewValue{ &invalid };
		int64_t currentValue = ReadPrimitiveInt(obj, type);
		enumClass->GetEnglishNameOfValue(currentValue, previewValue);

		if (ImGui::BeginCombo(currentMemberName, *previewValue)) {
			auto* values = enumClass->GetValues();

			for (size_t i = 0; i < enumClass->GetValueCount(); i++) {
				int64_t index = static_cast<int64_t>(values[i].GetIndex());
				bool is_selected = currentValue == index;

				if (ImGui::Selectable(values[i].GetEnglishName(), is_selected)) {
					WritePrimitiveInt(obj, index, type);
					edited = true;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return edited;
	}

	template<typename F>
	static bool VisitFlags(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
		bool edited{};
		int64_t currentValue = ReadPrimitiveInt(obj, type);

		if (ImGui::TreeNode(currentMemberName)) {
			auto* values = enumEntries->items;

			for (size_t i = 0; i < enumEntries->count; i++) {
				edited |= ImGui::CheckboxFlags(values[i].GetEnglishName(), &currentValue, static_cast<size_t>(1) << values[i].GetIndex());
			}

			ImGui::TreePop();
		}

		if (edited)
			WritePrimitiveInt(obj, currentValue, type);

		return edited;
	}

	template<typename F>
	static bool VisitPointer(void** obj, F f) {
		return obj == nullptr ? false : f(*obj);
	}

	template<typename F>
	static bool VisitClassMember(void* obj, const hh::fnd::RflClassMember* member, F f) {
		return MemberScope(member, [&]() { return f(obj); });
	}

	template<typename F>
	static bool VisitArrayClassMember(void* obj, const hh::fnd::RflClassMember* member, size_t size, F f) {
		return MemberScope(member, [&]() {
			bool edited{};
			if (ImGui::TreeNode(currentMemberName, "%s[0..%zd]", currentMemberName, size - 1)) {
				edited = f(obj);
				ImGui::TreePop();
			}
			return edited;
		});
	}

	template<typename F>
	static bool VisitArrayClassMemberItem(void* obj, const hh::fnd::RflClassMember* member, size_t idx, F f) {
		char name[200] = "";
		snprintf(name, 200, "%s[%zd]", currentMemberName, idx);

		return NameScope(name, [&]() {
			ImGui::PushID(static_cast<int>(idx));
			bool edited = f(obj);
			ImGui::PopID();
			return edited;
		});
	}

	template<typename F>
	static bool VisitBaseStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
		return f(obj);
	}

	template<typename F>
	static bool VisitStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
		ImGui::PushID(obj);
		bool edited{};
		bool isOpen{ ImGui::TreeNodeEx(currentMemberName, RenderStaticReflectionEditor::defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };

		if (ImGui::BeginDragDropSource()) {
			RflDragDropData dndData{ rflClass, obj };
			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);

				if (dndData.rflClass == rflClass && dndData.obj != obj) {
					rflops::Copy::Apply(obj, dndData.obj, rflClass);
					edited = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("RFL Operations")) {
			if (ImGui::Selectable("Reset to default values")) {
				const hh::fnd::RflTypeInfo* typeInfo = hh::fnd::RflTypeInfoRegistry::GetInstance()->GetByName(rflClass->GetName());
				typeInfo->CleanupLoadedObject(obj);
				typeInfo->ConstructObject(obj, hh::fnd::MemoryRouter::GetModuleAllocator());
				edited = true;
			}
			ImGui::EndPopup();
		}
		
		if (isOpen) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			edited = f(obj);
			ImGui::TreePop();
		}
		ImGui::PopID();
		return edited;
	}
};

class RenderResettableReflectionEditor {
public:
	constexpr static size_t arity = 2;
	typedef bool result_type;

	template<typename T>
	static bool VisitPrimitive(T* obj, T* orig) { return RenderStaticReflectionEditor::VisitPrimitive(obj); }

	template<typename F, template<typename> typename A, typename C, typename D>
	static bool VisitArray(RflArrayAccessor<A>& arr, RflArrayAccessor<A>& orig, C c, D d, F f) {
		size_t idx{};
		void* emptyElement{};

		bool edited = RenderStaticReflectionEditor::VisitArray(arr, c, d, [&](void* obj) {
			if (idx >= orig.size() && emptyElement == nullptr)
				emptyElement = c();

			bool edited = f(obj, idx < orig.size() ? orig[idx] : emptyElement);

			idx++;

			return edited;
		});

		if (emptyElement)
			d(emptyElement);

		return edited;
	}

	template<typename F>
	static bool VisitEnum(void* obj, void* orig, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
		return RenderStaticReflectionEditor::VisitEnum(obj, type, enumClass, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitFlags(void* obj, void* orig, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
		return RenderStaticReflectionEditor::VisitFlags(obj, type, enumEntries, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitPointer(void** obj, void* orig, F f) {
		return RenderStaticReflectionEditor::VisitPointer(obj, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitClassMember(void* obj, void* orig, const hh::fnd::RflClassMember* member, F f) {
		return RenderStaticReflectionEditor::VisitClassMember(obj, member, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitArrayClassMember(void* obj, void* orig, const hh::fnd::RflClassMember* member, size_t size, F f) {
		return RenderStaticReflectionEditor::VisitArrayClassMember(obj, member, size, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitArrayClassMemberItem(void* obj, void* orig, const hh::fnd::RflClassMember* member, size_t idx, F f) {
		return RenderStaticReflectionEditor::VisitArrayClassMemberItem(obj, member, idx, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitBaseStruct(void* obj, void* orig, const hh::fnd::RflClass* rflClass, F f) {
		return RenderStaticReflectionEditor::VisitBaseStruct(obj, rflClass, [&](void* obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool VisitStruct(void* obj, void* orig, const hh::fnd::RflClass* rflClass, F f) {
		ImGui::PushID(obj);
		bool edited{};
		bool isOpen{ ImGui::TreeNodeEx(RenderStaticReflectionEditor::currentMemberName, RenderStaticReflectionEditor::defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };

		if (ImGui::BeginDragDropSource()) {
			RflDragDropData dndData{ rflClass, obj };
			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);

				if (dndData.rflClass == rflClass && dndData.obj != obj) {
					rflops::Copy::Apply(obj, dndData.obj, rflClass);
					edited = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("RFL Operations")) {
			if (ImGui::Selectable("Reset to cached values")) {
				rflops::Copy::Apply(obj, orig, rflClass);
				edited = true;
			}

			if (ImGui::Selectable("Reset to default values")) {
				const hh::fnd::RflTypeInfo* typeInfo = hh::fnd::RflTypeInfoRegistry::GetInstance()->GetByName(rflClass->GetName());
				typeInfo->CleanupLoadedObject(obj);
				typeInfo->ConstructObject(obj, hh::fnd::MemoryRouter::GetModuleAllocator());
				edited = true;
			}

			ImGui::EndPopup();
		}
		
		if (isOpen) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			edited |= f(obj, orig);
			ImGui::TreePop();
		}
		ImGui::PopID();
		return edited;
	}

};

bool ReflectionEditor(const char* label, void* reflectionData, const hh::fnd::RflClass* rflClass, bool defaultOpen) {
	ImGui::BeginGroup();
	RenderStaticReflectionEditor::defaultOpen = defaultOpen;
	bool edited = RenderStaticReflectionEditor::NameScope(label, [&]() { return rflops::traversals::rflop<RenderStaticReflectionEditor>::Apply(reflectionData, rflClass); });
	ImGui::EndGroup();
	return edited;
}

bool ResettableReflectionEditor(const char* label, void* reflectionData, void* originalReflectionData, const hh::fnd::RflClass* rflClass) {
	ImGui::BeginGroup();
	RenderStaticReflectionEditor::defaultOpen = false;
	bool edited = RenderStaticReflectionEditor::NameScope(label, [&]() { return rflops::traversals::rflop<RenderResettableReflectionEditor>::Apply(reflectionData, originalReflectionData, rflClass); });
	ImGui::EndGroup();
	return edited;
}

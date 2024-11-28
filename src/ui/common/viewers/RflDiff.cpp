#include "RflDiff.h"
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/traversals/traversal.h>
#include <ui/common/viewers/Basic.h>
#include <sstream>
#include <regex>

using namespace ucsl::reflection;
using namespace ucsl::reflection::traversals;

class RenderRflDiffChange {
public:
	constexpr static size_t arity = 1;
	typedef bool result_type;
	inline static RflDiffChange* change;
	inline static int64_t pathIdx;

	template<typename T>
	static bool visit_primitive(T& obj, const PrimitiveInfo<T>& info) {
		if (pathIdx >= 0)
			ImGui::Text("An error occurred.");

		Viewer("Old value", obj);
		Viewer("New value", *static_cast<const T*>(change->value));
		return false;
	}

	template<typename F, typename A, typename C, typename D>
	static bool _visit_array(A& arr, const ArrayInfo& info, C c, D d, F f) {
		pathIdx--;
		f(arr[change->path[pathIdx].arrayIdx]);
		pathIdx++;
		return false;
	}

	template<typename F, typename A, typename C, typename D> static bool visit_array(A& arr, const ArrayInfo& info, C c, D d, F f) { return _visit_array(arr, info, c, d, f); }
	template<typename F, typename A, typename C, typename D> static bool visit_tarray(A& arr, const ArrayInfo& info, C c, D d, F f) { return _visit_array(arr, info, c, d, f); }

	template<typename T, typename O>
	static bool visit_enum(T& obj, const EnumInfo<O>& info) {
		visit_primitive(obj, PrimitiveInfo<T>{});
		return false;
	}

	template<typename T, typename O>
	static bool visit_flags(T& obj, const FlagsInfo<O>& info) {
		visit_primitive(obj, PrimitiveInfo<T>{});
		return false;
	}

	template<typename F>
	static bool visit_pointer(opaque_obj*& obj, const PointerInfo& info, F f) {
		if (obj != nullptr)
			f(*obj);
		return false;
	}

	template<typename F>
	static bool visit_carray(opaque_obj* obj, const CArrayInfo& info, F f) {
		pathIdx--;
		f(*reinterpret_cast<opaque_obj*>(reinterpret_cast<size_t>(obj) + change->path[pathIdx].arrayIdx * info.stride));
		pathIdx++;
		return false;
	}

	template<typename F>
	static bool visit_type(opaque_obj& obj, const TypeInfo& info, F f) {
		f(obj);
		return false;
	}

	template<typename F>
	static bool visit_field(opaque_obj& obj, const FieldInfo& info, F f) {
		if (strcmp(info.name, change->path[pathIdx].propertyName))
			return false;

		pathIdx--;
		f(obj);
		pathIdx++;
		return false;
	}

	template<typename F>
	static bool visit_base_struct(opaque_obj& obj, const StructureInfo& info, F f) {
		f(obj);
		return false;
	}

	template<typename F>
	static bool visit_struct(opaque_obj& obj, const StructureInfo& info, F f) {
		f(obj);
		return false;
	}

	template<typename F>
	static bool visit_root(opaque_obj& obj, const RootInfo& info, F f) {
		f(obj);
		return false;
	}
};


void Viewer(const char* label, const RflDiffResult& diff, void* obj, const hh::fnd::RflClass* rflClass) {
	for (auto& change : diff.changes) {
		char pathBuf[500];
		size_t idx{};

		for (size_t i = change.path.size(); i > 0; i--) {
			auto& pathEl = change.path[i - 1];
			if (pathEl.type == RflDiffChange::Index::Type::INDEX) {
				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "%zd", pathEl.arrayIdx);
			}
			else {
				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "%s", pathEl.propertyName);
			}
			
			if (i > 1)
				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, ".");
		}
		
		if (change.type == RflDiffChange::Type::UPDATE) {
			if (ImGui::TreeNode(pathBuf, "UPDATED: %s", pathBuf)) {
				RenderRflDiffChange::change = &change;
				RenderRflDiffChange::pathIdx = change.path.size() - 1;
				ucsl::reflection::traversals::traversal<RenderRflDiffChange>{}(*(opaque_obj*)obj, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));
				ImGui::TreePop();
			}
		}
		else
			ImGui::Text("%s: %s", change.type == RflDiffChange::Type::ADD ? "ADDED" : "REMOVED", pathBuf);
	}
}

//std::regex escapes{ "\\" };
//std::regex quotes{ "\"" };
//
//class RenderRflDiffChangeRfl2HMM {
//public:
//	constexpr static size_t arity = 1;
//	typedef bool result_type;
//	inline static RflDiffChange* change;
//	inline static int64_t pathIdx;
//
//	struct RFL2HMMParamValue {
//		std::string str{};
//
//		RFL2HMMParamValue& operator|=(RFL2HMMParamValue&& other) {
//			str = std::move(other.str);
//		}
//	};
//
//	static RFL2HMMParamValue VisitPrimitive(const char* obj) {
//		RFL2HMMParamValue val{};
//		std::string sval{ obj };
//		std::ostringstream oss{};
//
//		sval = std::regex_replace(sval, escapes, "\\\\");
//		sval = std::regex_replace(sval, quotes, "\\\"");
//
//		oss << "\"" << sval << "\"";
//
//		val.str = std::move(oss.str());
//
//		return std::move(val);
//	}
//
//	static RFL2HMMParamValue VisitPrimitive(csl::ut::VariableString* obj) {
//		return VisitPrimitive(obj->c_str());
//	}
//
//	template<typename T>
//	static RFL2HMMParamValue VisitPrimitive(T* obj) {
//		RFL2HMMParamValue val{};
//		std::ostringstream oss{};
//
//		oss << *obj;
//		val.str = std::move(oss.str());
//
//		return std::move(val);
//	}
//
//	template<typename F, typename C, typename D>
//	static RFL2HMMParamValue VisitArray(RflMoveArrayAccessor& arr, C c, D d, F f) {
//		void* obj = arr[change->path[pathIdx].arrayIdx];
//		pathIdx--;
//		RFL2HMMParamValue&& val = f(obj);
//		pathIdx++;
//		return val;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitEnum(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
//		return f(obj);
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitFlags(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
//		return f(obj);
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitClassMember(void* obj, const hh::fnd::RflClassMember* member, F f) {
//		if (strcmp(member->GetName(), change->path[pathIdx].propertyName) == 0) {
//			pathIdx--;
//			f(obj);
//			pathIdx++;
//		}
//		return false;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitArrayClassMember(void* obj, const hh::fnd::RflClassMember* member, size_t size, F f) {
//		if (strcmp(member->GetName(), change->path[pathIdx].propertyName) == 0) {
//			pathIdx--;
//			f(obj);
//			pathIdx++;
//		}
//		return false;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitArrayClassMemberItem(void* obj, size_t idx, F f) {
//		if (idx == change->path[pathIdx].arrayIdx) {
//			pathIdx--;
//			f(obj);
//			pathIdx++;
//		}
//		return false;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
//		f(obj);
//		return false;
//	}
//
//};
//
//
//void Rfl2HmmViewer(const char* label, const RflDiffResult& diff, void* obj, const hh::fnd::RflClass* rflClass)
//{
//	RenderRflDiffChangeRfl2HMM::code = std::string{};
//	for (auto& change : diff.changes) {
//		char pathBuf[500];
//		size_t idx{};
//
//		idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "RFL_SET_PARAM(%s, ", pathEl.arrayIdx);
//		for (size_t i = change.path.size(); i > 0; i--) {
//			auto& pathEl = change.path[i - 1];
//			if (pathEl.type == RflDiffChange::Index::Type::INDEX) {
//				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "[%zd]", pathEl.arrayIdx);
//			}
//			else {
//				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, ".%s", pathEl.propertyName);
//			}
//		}
//		idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, ", ", pathEl.arrayIdx);
//
//		if (change.type == RflDiffChange::Type::UPDATE) {
//			if (ImGui::TreeNode(pathBuf, "UPDATED: %s", pathBuf)) {
//				RenderRflDiffChange::change = &change;
//				RenderRflDiffChange::pathIdx = change.path.size() - 1;
//				rflops::traversals::rflop<RenderRflDiffChange>::Apply(obj, rflClass);
//				ImGui::TreePop();
//			}
//		}
//	}
//}

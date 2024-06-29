#include "RflDiff.h"
#include <ui/common/viewers/Basic.h>
#include <reflection/ReflectiveOperations.h>
#include <sstream>
#include <regex>

class RenderRflDiffChange {
public:
	constexpr static size_t arity = 1;
	typedef bool result_type;
	inline static RflDiffChange* change;
	inline static int64_t pathIdx;

	template<typename T>
	static bool VisitPrimitive(T* obj) {
		if (pathIdx >= 0)
			ImGui::Text("An error occurred.");

		Viewer("Old value", *obj);
		Viewer("New value", *static_cast<const T*>(change->value));
		return false;
	}

	template<typename F, template<typename> typename A, typename C, typename D>
	static bool VisitArray(RflArrayAccessor<A>& arr, C c, D d, F f) {
		void* obj = arr[change->path[pathIdx].arrayIdx];
		pathIdx--;
		f(obj);
		pathIdx++;
		return false;
	}

	template<typename F>
	static bool VisitEnum(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
		f(obj);
		return false;
	}

	template<typename F>
	static bool VisitFlags(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
		f(obj);
		return false;
	}

	template<typename F>
	static bool VisitClassMember(void* obj, const hh::fnd::RflClassMember* member, F f) {
		if (strcmp(member->GetName(), change->path[pathIdx].propertyName) == 0) {
			pathIdx--;
			f(obj);
			pathIdx++;
		}
		return false;
	}

	template<typename F>
	static bool VisitArrayClassMember(void* obj, const hh::fnd::RflClassMember* member, size_t size, F f) {
		if (strcmp(member->GetName(), change->path[pathIdx].propertyName) == 0) {
			pathIdx--;
			f(obj);
			pathIdx++;
		}
		return false;
	}

	template<typename F>
	static bool VisitArrayClassMemberItem(void* obj, size_t idx, F f) {
		if (idx == change->path[pathIdx].arrayIdx) {
			pathIdx--;
			f(obj);
			pathIdx++;
		}
		return false;
	}

	template<typename F>
	static bool VisitStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
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
				rflops::traversals::rflop<RenderRflDiffChange>::Apply(obj, rflClass);
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

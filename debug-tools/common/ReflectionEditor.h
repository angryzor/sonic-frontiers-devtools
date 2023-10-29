#pragma once

class ReflectionEditor {
    static const char* GetRflMemberName(const hh::fnd::RflClassMember* member);

    template<typename T>
    static void RenderSliderlessScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName) {
        const hh::fnd::RflCustomAttribute* rangeAttr;

        if (rangeName && (rangeAttr = member->GetAttribute(rangeName))) {
            const T* rangeValues = reinterpret_cast<const T*>(rangeAttr->GetData());

            ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n, 1.0f, &rangeValues[0], &rangeValues[1]);
        }
        else {
            ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n);
        }
    }

    template<typename T>
    static void RenderSliderlessScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        RenderSliderlessScalarRflParamEditor<T>(obj, member, dtype, n, nullptr);
    }

    template<typename T>
    static void RenderScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName, const T& minSliderRange, const T& maxSliderRange) {
        const hh::fnd::RflCustomAttribute* rangeAttr;

        if (rangeName && (rangeAttr = member->GetAttribute(rangeName))) {
            const T* rangeValues = reinterpret_cast<const T*>(rangeAttr->GetData());

            if (minSliderRange <= rangeValues[0] && rangeValues[1] <= maxSliderRange)
                ImGui::SliderScalarN(GetRflMemberName(member), dtype, obj, n, &rangeValues[0], &rangeValues[1]);
            else
                ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n, 1.0f, &rangeValues[0], &rangeValues[1]);
        }
        else {
            ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n, 1.0f);
        }
    }

    template<typename T>
    static void RenderScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName) {
        RenderScalarRflParamEditor<T>(obj, member, dtype, n, rangeName, std::numeric_limits<T>::lowest() / 2, std::numeric_limits<T>::max() / 2);
    }

    template<typename T>
    static void RenderScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        RenderScalarRflParamEditor<T>(obj, member, dtype, n, nullptr);
    }

    static void RenderStubRflParamEditor(const hh::fnd::RflClassMember* member);
    static void RenderStructRflParamEditor(void* obj, const char* name, const hh::fnd::RflClass* rflClass);
    static void RflClassMembersEditor(void* obj, const hh::fnd::RflClass* rflClass);

public:
    template<typename T> static void Render(T& reflectionData) {
        auto rflClass = rangerssdk::bootstrap::GetAddress(&T::rflClass);

        RenderStructRflParamEditor(&reflectionData, rflClass->m_pName, rflClass);
    }
};

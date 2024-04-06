#pragma once

class ReflectionEditor {
    static const char* GetRflMemberName(const hh::fnd::RflClassMember* member);

    template<typename T> class rfl_type_info {
    public:
        static_assert(std::numeric_limits<T>::is_specialized);
        typedef std::numeric_limits<T> numeric_limits;
    };
    template<> class rfl_type_info<csl::math::Vector2> {
    public:
        typedef std::numeric_limits<float> numeric_limits;
    };
    template<> class rfl_type_info<csl::math::Vector3> {
    public:
        typedef std::numeric_limits<float> numeric_limits;
    };
    template<> class rfl_type_info<csl::math::Vector4> {
    public:
        typedef std::numeric_limits<float> numeric_limits;
    };

    static constexpr float default_step(bool integer) { return integer ? 1.0f : 0.001f; }
    template<typename T> static constexpr float default_step() {  return default_step(rfl_type_info<T>::numeric_limits::is_integer); }

    template<typename T> static float rfl_step(const T& rangeValue) { return static_cast<float>(rangeValue); }
    template<> static float rfl_step<csl::math::Vector2>(const csl::math::Vector2& rangeValue) { return rangeValue.x(); }
    template<> static float rfl_step<csl::math::Vector3>(const csl::math::Vector3& rangeValue) { return rangeValue.x(); }
    template<> static float rfl_step<csl::math::Vector4>(const csl::math::Vector4& rangeValue) { return rangeValue.x(); }

    template<typename T>
    static void RenderSliderlessScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName) {
        const hh::fnd::RflCustomAttribute* rangeAttr;

        if (rangeName && (rangeAttr = member->GetAttribute(rangeName))) {
            const T* rangeValues = reinterpret_cast<const T*>(rangeAttr->GetData());

            ImGui::DragScalarN(name, dtype, obj, n, rfl_step(rangeValues[2]), &rangeValues[0], &rangeValues[1]);
        }
        else {
            ImGui::DragScalarN(name, dtype, obj, n, default_step<T>());
        }
    }

    template<typename T>
    static void RenderSliderlessScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        RenderSliderlessScalarRflParamEditor<T>(name, obj, member, dtype, n, nullptr);
    }

    template<typename T>
    static void RenderScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName, const T& minSliderRange, const T& maxSliderRange) {
        const hh::fnd::RflCustomAttribute* rangeAttr;

        if (rangeName && (rangeAttr = member->GetAttribute(rangeName))) {
            const T* rangeValues = reinterpret_cast<const T*>(rangeAttr->GetData());

            if (minSliderRange <= rangeValues[0] && rangeValues[1] <= maxSliderRange)
                ImGui::SliderScalarN(name, dtype, obj, n, &rangeValues[0], &rangeValues[1]);
            else
                ImGui::DragScalarN(name, dtype, obj, n, rfl_step(rangeValues[2]), &rangeValues[0], &rangeValues[1]);
        }
        else {
            ImGui::DragScalarN(name, dtype, obj, n, default_step<T>());
        }
    }

    template<typename T>
    static void RenderScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName) {
        RenderScalarRflParamEditor<T>(name, obj, member, dtype, n, rangeName, rfl_type_info<T>::numeric_limits::lowest() / 2, rfl_type_info<T>::numeric_limits::max() / 2);
    }

    template<typename T>
    static void RenderScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        RenderScalarRflParamEditor<T>(name, obj, member, dtype, n, nullptr);
    }

    static int64_t ReadPrimitiveInt(void* obj, const hh::fnd::RflClassMember::Type type);
    static void WritePrimitiveInt(void* obj, int64_t value, const hh::fnd::RflClassMember::Type type);
    static void RenderEnumEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member);
    static void RenderFlagsEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member);
    static void RenderPrimitiveEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, const hh::fnd::RflClassMember::Type type);
    static void RenderStubRflParamEditor(const char* name, const hh::fnd::RflClassMember* member);
    static void RenderArrayRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member);
    static void RflClassMemberEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member);
    static void RenderStructRflParamEditor(const char* name, void* obj, const hh::fnd::RflClass* rflClass);
    static void RflClassMembersEditor(void* obj, const hh::fnd::RflClass* rflClass);

public:
    static void Render(void* reflectionData, const hh::fnd::RflClass* rflClass) {
        ImGui::BeginGroup();
        RenderStructRflParamEditor(rflClass->m_pName, reflectionData, rflClass);
        ImGui::EndGroup();
    }

    template<typename T> static void Render(T& reflectionData) {
        Render(&reflectionData, rangerssdk::GetAddress(&T::rflClass));
    }
};

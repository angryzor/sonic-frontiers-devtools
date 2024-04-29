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

    template<typename T> class range_info {
    };
    template<> class range_info<int32_t> {
    public:
        static constexpr const char* name = "RangeSint32";
    };
    template<> class range_info<uint32_t> {
    public:
        static constexpr const char* name = "RangeUint32";
    };
    template<> class range_info<int64_t> {
    public:
        static constexpr const char* name = "RangeSint64";
    };
    template<> class range_info<uint64_t> {
    public:
        static constexpr const char* name = "RangeUint64";
    };
    template<> class range_info<float> {
    public:
        static constexpr const char* name = "RangeFloat";
    };
    template<> class range_info<csl::math::Vector2> {
    public:
        static constexpr const char* name = "RangeVector2";
    };
    template<> class range_info<csl::math::Vector3> {
    public:
        static constexpr const char* name = "RangeVector3";
    };
    template<> class range_info<csl::math::Vector4> {
    public:
        static constexpr const char* name = "RangeVector4";
    };

    static float default_step(bool integer) { return integer ? 1.0f : defaultFloatStep; }
    template<typename T> static float default_step() {  return default_step(rfl_type_info<T>::numeric_limits::is_integer); }

    template<typename T> static float rfl_step(const T& rangeValue) { return std::fmaxf(minFloatStep, static_cast<float>(rangeValue)); }
    template<> static float rfl_step<csl::math::Vector2>(const csl::math::Vector2& rangeValue) { return rfl_step(rangeValue.x()); }
    template<> static float rfl_step<csl::math::Vector3>(const csl::math::Vector3& rangeValue) { return rfl_step(rangeValue.x()); }
    template<> static float rfl_step<csl::math::Vector4>(const csl::math::Vector4& rangeValue) { return rfl_step(rangeValue.x()); }

    template<typename T>
    static void RenderScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        ImGui::DragScalarN(name, dtype, obj, n, default_step<T>());
    }

    template<typename T, typename R, bool allowSliders = true>
    static void RenderScalarRflParamEditor(const char* name, void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        if (const auto* rangeAttr = member->GetAttribute(range_info<typename R>::name)) {
            auto rangeValues = reinterpret_cast<const R*>(rangeAttr->GetData());

            if constexpr (allowSliders) {
                if (rangeValues[1] - rangeValues[0] < sliderCutOff) {
                    ImGui::SliderScalarN(name, dtype, obj, n, &rangeValues[0], &rangeValues[1]);
                    return;
                }
            }
 
            ImGui::DragScalarN(name, dtype, obj, n, rfl_step(rangeValues[2]), &rangeValues[0], &rangeValues[1]);
        }
        else
            RenderScalarRflParamEditor<T>(name, obj, member, dtype, n);
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
    static float defaultFloatStep;
    static float minFloatStep;
    static unsigned int sliderCutOff;

    static void Render(void* reflectionData, const hh::fnd::RflClass* rflClass) {
        ImGui::BeginGroup();
        RenderStructRflParamEditor(rflClass->m_pName, reflectionData, rflClass);
        ImGui::EndGroup();
    }

    template<typename T> static void Render(T& reflectionData) {
        Render(&reflectionData, rangerssdk::GetAddress(&T::rflClass));
    }
};

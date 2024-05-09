#pragma once

static float rflMinFloatStep{ 0.01f };
static unsigned int rflSliderCutOff{ 3000 };

bool ReflectionEditor(const char* label, void* reflectionData, const hh::fnd::RflClass* rflClass);

template<typename T, typename = decltype(T::rflClass)>
static bool Editor(const char* label, T& reflectionData) {
    return ReflectionEditor(label, &reflectionData, rangerssdk::GetAddress(&T::rflClass));
}

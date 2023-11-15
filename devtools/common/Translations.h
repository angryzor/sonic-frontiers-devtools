#pragma once


class Translations {
public:
	enum class Language {
		VAR_NAMES,
		JAPANESE,
		ENGLISH,
	};
private:
	struct CaptionTranslation {
		size_t originalStrAddr;
		const char* translation;
	};

	static constexpr size_t TRANSLATION_COUNT = 7867;
	static CaptionTranslation translations[TRANSLATION_COUNT];

	static csl::ut::PointerMap<const char*, const char*>* translationMap;

	static Language currentLanguage;

public:
	static void Init(csl::fnd::IAllocator* allocator);
	static Language GetCurrentLanguage();
	static void SetCurrentLanguage(Language language);
	static const char* GetLanguageName(Language language);
	static const char* GetTranslation(const char* str);
};

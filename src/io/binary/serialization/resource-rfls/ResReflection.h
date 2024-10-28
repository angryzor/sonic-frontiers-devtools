#pragma once
#include <reflection/RflClassGenFwd.h>

struct ResReflectionHeader {
	unsigned int magic{ 0x32464552u };
	unsigned int version{ 0x30302E31u };
	unsigned int hash{};
	unsigned int padding{};

	ResReflectionHeader(unsigned int hash);
};

template<> struct hh::fnd::RflClassTraits<ResReflectionHeader> : RflClassTraitsImpl<ResReflectionHeader> {};

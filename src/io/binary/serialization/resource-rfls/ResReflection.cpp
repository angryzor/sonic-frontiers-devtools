#include "ResReflection.h"

ResReflectionHeader::ResReflectionHeader(unsigned int hash) : hash{ hash } {}

#include <reflection/RflClassGen.h>

using namespace hh::fnd;

namespace reflection::serialization::resource_rfls::reflection {
	auto resReflectionHeaderMembers = CreateRflClassMembers<void, unsigned int, unsigned int, unsigned int, unsigned int>("magic", "version", "hash", "padding");
}

using namespace reflection::serialization::resource_rfls::reflection;

const RflClass RflClassTraitsImpl<ResReflectionHeader>::rflClass{ "ResReflectionHeader", nullptr, static_cast<uint32_t>(sizeof(ResReflectionHeader)), nullptr, 0, resReflectionHeaderMembers.data(), static_cast<uint32_t>(resReflectionHeaderMembers.size()), nullptr };

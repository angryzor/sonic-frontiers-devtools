#include "ResourceTypes.h"

using namespace hh::fnd;

const char* GetExtensionByTypeInfo(const ResourceTypeInfo* typeInfo)
{
	if (typeInfo == hh::game::ResObjectWorld::GetTypeInfo())
		return "gedit";
	else
		return ResourceTypeRegistry::GetInstance()->GetExtensionByTypeInfo(typeInfo);
}

const ResourceTypeInfo* GetTypeInfoByExtension(const char* extension)
{
	if (!strcmp(extension, "gedit"))
		return hh::game::ResObjectWorld::GetTypeInfo();
	else
		return ResourceTypeRegistry::GetInstance()->GetTypeInfoByExtension(extension);
}

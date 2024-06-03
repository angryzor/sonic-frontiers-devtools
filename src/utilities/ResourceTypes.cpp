#include "ResourceTypes.h"

using namespace hh::fnd;

//template<typename = KnownResourceTypes>
//class ResourceIterator;
//template<typename... T>
//class ResourceIterator<std::tuple<T...>> {
//public:
//	void GetExtensionByTypeInfo() {
//		return (typeInfo == T::GetTypeInfo() ? 
//	}
//};

const char* GetExtensionByTypeInfo(const ResourceTypeInfo* typeInfo)
{
	if (typeInfo == hh::game::ResObjectWorld::GetTypeInfo())
		return "gedit";
	//else if (typeInfo == app::gfx::ResPointcloudModel::GetTypeInfo())
	//	return "pcmodel";
	else
		return ResourceTypeRegistry::GetInstance()->GetExtensionByTypeInfo(typeInfo);
}

const ResourceTypeInfo* GetTypeInfoByExtension(const char* extension)
{
	if (!strcmp(extension, "gedit"))
		return hh::game::ResObjectWorld::GetTypeInfo();
	//else if (!strcmp(extension, "pcmodel"))
	//	return app::gfx::ResPointcloudModel::GetTypeInfo();
	else
		return ResourceTypeRegistry::GetInstance()->GetTypeInfoByExtension(extension);
}

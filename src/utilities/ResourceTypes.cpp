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
#ifdef DEVTOOLS_TARGET_SDK_rangers
	else if (typeInfo == app::gfx::ResPointcloudModel::GetTypeInfo())
		return "pcmodel";
#endif
	else
		return ResourceTypeRegistry::GetInstance()->GetExtensionByTypeInfo(typeInfo);
}

const ResourceTypeInfo* GetTypeInfoByExtension(const char* extension)
{
	if (!strcmp(extension, "gedit"))
		return hh::game::ResObjectWorld::GetTypeInfo();
#ifdef DEVTOOLS_TARGET_SDK_rangers
	else if (!strcmp(extension, "pcmodel"))
		return app::gfx::ResPointcloudModel::GetTypeInfo();
#endif
	else
		return ResourceTypeRegistry::GetInstance()->GetTypeInfoByExtension(extension);
}

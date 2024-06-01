#include "CompatibleObject.h"

#ifdef DEVTOOLS_TARGET_SDK_wars
CompatibleObject::CompatibleObject(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{} {
	pAllocator = allocator;
}
#else
CompatibleObject::CompatibleObject(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{ allocator, true } {
}
#endif
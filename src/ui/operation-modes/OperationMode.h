#pragma once
#include <utilities/CompatibleObject.h>

class OperationMode : public CompatibleObject {
public:
	OperationMode(csl::fnd::IAllocator* allocator);
	virtual void Render() = 0;
};

#pragma once

class OperationMode : public hh::fnd::ReferencedObject {
public:
	OperationMode(csl::fnd::IAllocator* allocator);
	virtual void Render() = 0;
};

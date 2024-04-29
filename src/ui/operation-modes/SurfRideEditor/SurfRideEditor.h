#pragma once

#include <ui/operation-modes/OperationMode.h>

class SurfRideEditor : public OperationMode {
public:
	SurfRideEditor(csl::fnd::IAllocator* allocator);
	~SurfRideEditor();
	virtual void Render() override;
};
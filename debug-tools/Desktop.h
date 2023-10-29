#pragma once
#include "operation-modes/OperationMode.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"

class Desktop : public hh::fnd::BaseObject {
    ObjectInspection operationMode{ GetAllocator() };

public:
    Desktop(csl::fnd::IAllocator* allocator);
    void Render();
};

#pragma once
#include "../OperationMode.h"
#include "ObjectList.h"
#include "ObjectInspector.h"

class ObjectInspection : public OperationMode {
    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };

public:
    ObjectInspection(csl::fnd::IAllocator* allocator);

    hh::fnd::Reference<hh::game::GameObject> focusedObject;

    void Render();
};

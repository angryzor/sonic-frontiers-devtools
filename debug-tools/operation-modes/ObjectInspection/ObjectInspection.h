#pragma once
#include "../OperationMode.h"
#include "ObjectList.h"
#include "ObjectInspector.h"

class Desktop;
class ObjectInspection : public OperationMode {
    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };

public:
    Desktop& desktop;
    ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop);

    hh::fnd::Reference<hh::game::GameObject> focusedObject;

    void Render();
};

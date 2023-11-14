#pragma once
#include "operation-modes/OperationMode.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"
#include "StandaloneWindow.h"

class Desktop : public hh::fnd::BaseObject {
    ObjectInspection operationMode{ GetAllocator(), *this };
    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture;
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windows{ GetAllocator() };

public:
    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader;
    static Desktop* instance;

    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    void Render();
    void AddStandaloneWindow(StandaloneWindow* window);
    void RemoveStandaloneWindow(StandaloneWindow* window);
};

#pragma once
#include "operation-modes/OperationMode.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"

class Desktop : public hh::fnd::BaseObject {
    ObjectInspection operationMode{ GetAllocator(), *this };
    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader;
    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture;

public:
    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    void Render();
};

#include "FxColCollisionShapeData.h"

using namespace ucsl::resources::svcol::v1;

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const ShapeData& shape) {
	csl::ut::Color8 color{ 255, 255, 0, 255 };

	switch (shape.type) {
	case Shape::OBB: {
		Eigen::Affine3f transform{ Eigen::Translation3f{ shape.position } * shape.rotation };

		ctx.DrawOBB(transform, { shape.width / 2, shape.height / 2, shape.depth / 2 }, color);
		break;
	}
	default: break;
	}
}

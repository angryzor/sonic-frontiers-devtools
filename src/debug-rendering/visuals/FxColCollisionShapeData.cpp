#include "FxColCollisionShapeData.h"

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const app::gfx::FxColCollisionShapeData& shape) {
	csl::ut::Color8 color{ 255, 255, 0, 255 };
	csl::ut::Color8 innerColor{ 255, 128, 0, 255 };

	switch (shape.shape) {
	case app::gfx::FxColCollisionShapeData::Shape::SPHERE: {
		auto& extents = shape.extents.sphere;
		Eigen::Affine3f transform{ Eigen::Translation3f{ shape.position } };

		ctx.DrawSphere(transform, extents.radius, color);
		ctx.DrawSphere(transform, extents.radius - extents.borderThickness, innerColor);
		break;
	}
	case app::gfx::FxColCollisionShapeData::Shape::CYLINDER: {
		auto& extents = shape.extents.cylinder;
		Eigen::Affine3f transform{ Eigen::Translation3f{ shape.position } * shape.rotation };

		ctx.DrawCylinder(transform, extents.radius, extents.halfHeight * 2, color);
		ctx.DrawCylinder(transform, extents.radius - extents.borderThickness, extents.halfHeight * 2, innerColor);
		break;
	}
	case app::gfx::FxColCollisionShapeData::Shape::ANISOTROPIC_OBB: {
		auto& extents = shape.extents.anisotropicObb;
		Eigen::Affine3f transform{ Eigen::Translation3f{ shape.position } * shape.rotation };

		ctx.DrawOBB(transform, { extents.width / 2, extents.height / 2, extents.depth / 2 }, color);
		ctx.DrawOBB(transform * Eigen::Translation3f{ 0.0f, 0.0f, (extents.negativeDepthBorderThickness - extents.positiveDepthBorderThickness) / 2 }, { extents.width / 2, extents.height / 2, (extents.depth - extents.positiveDepthBorderThickness - extents.negativeDepthBorderThickness) / 2 }, innerColor);
		break;
	}
	case app::gfx::FxColCollisionShapeData::Shape::ISOTROPIC_OBB: {
		auto& extents = shape.extents.isotropicObb;
		Eigen::Affine3f transform{ Eigen::Translation3f{ shape.position } * shape.rotation };

		ctx.DrawOBB(transform, { extents.width / 2, extents.height / 2, extents.depth / 2 }, color);
		ctx.DrawOBB(transform, { extents.width / 2 - extents.borderThickness, extents.height / 2 - extents.borderThickness, extents.depth / 2 - extents.borderThickness }, innerColor);
		break;
	}
	}
}

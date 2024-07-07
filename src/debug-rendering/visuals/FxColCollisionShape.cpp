#include "FxColCollisionShape.h"

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const app::gfx::FxColCollisionShape& shape) {
	csl::math::Matrix34 transform{};

	switch (shape.shape) {
	case app::gfx::FxColCollisionShape::Shape::SPHERE:
		transform.translate(shape.position);
		ctx.DrawSphere(transform, shape.extents.sphere.radius, { 255, 255, 0, 255 });
		ctx.DrawSphere(transform, shape.extents.sphere.innerRadius, { 255, 128, 0, 255 });
		break;
	case app::gfx::FxColCollisionShape::Shape::CYLINDER:
		transform.translate(shape.position);
		transform.rotate(shape.rotation);
		ctx.DrawCylinder(transform, shape.extents.cylinder.halfHeight * 2, shape.extents.cylinder.radius, { 255, 255, 0, 255 });
		ctx.DrawCylinder(transform, shape.extents.cylinder.halfHeight * 2, shape.extents.cylinder.innerRadius, { 255, 128, 0, 255 });
		break;
	case app::gfx::FxColCollisionShape::Shape::ANISOTROPIC_OBB:
		transform.translate(shape.position);
		transform.rotate(shape.rotation);
		ctx.DrawOBB(transform, { shape.extents.anisotropicObb.width, shape.extents.anisotropicObb.height, shape.extents.anisotropicObb.depth }, { 255, 255, 0, 255 });
		transform.translate(csl::math::Vector3{ 0.0f, 0.0f, shape.extents.anisotropicObb.depth * shape.extents.anisotropicObb.innerDepthStart });
		ctx.DrawOBB(transform, { shape.extents.anisotropicObb.width, shape.extents.anisotropicObb.height, shape.extents.anisotropicObb.depth - shape.extents.anisotropicObb.innerDepthStart - shape.extents.anisotropicObb.innerDepthEnd }, { 255, 128, 0, 255 });
		break;
	case app::gfx::FxColCollisionShape::Shape::ISOTROPIC_OBB:
		transform.translate(shape.position);
		transform.rotate(shape.rotation);
		ctx.DrawOBB(transform, { shape.extents.anisotropicObb.width, shape.extents.anisotropicObb.height, shape.extents.anisotropicObb.depth }, { 255, 255, 0, 255 });
		transform.translate(csl::math::Vector3{
			shape.extents.isotropicObb.width * (1.0f - shape.extents.isotropicObb.innerSize / 2),
			shape.extents.isotropicObb.height * (1.0f - shape.extents.isotropicObb.innerSize / 2),
			shape.extents.isotropicObb.depth * (1.0f - shape.extents.isotropicObb.innerSize / 2),
		});
		ctx.DrawOBB(transform, {
			shape.extents.anisotropicObb.width * shape.extents.isotropicObb.innerSize,
			shape.extents.anisotropicObb.height * shape.extents.isotropicObb.innerSize,
			shape.extents.anisotropicObb.depth * shape.extents.isotropicObb.innerSize,
		}, {255, 128, 0, 255});
		break;
	}
}

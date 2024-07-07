#include "Geometry.h"

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Aabb& aabb, csl::ut::Color8 color) {
	ctx.DrawAABB(aabb.min, aabb.max, color);
}

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Obb& obb, csl::ut::Color8 color) {
	unsigned int colorInt = *reinterpret_cast<unsigned int*>(&color);

	csl::math::Vector3 corners[8]{
		obb.min,
		obb.min + obb.extentX,
		obb.min + obb.extentY,
		obb.min + obb.extentZ,
		obb.min + obb.extentX + obb.extentY,
		obb.min + obb.extentX + obb.extentZ,
		obb.min + obb.extentY + obb.extentZ,
		obb.min + obb.extentX + obb.extentY + obb.extentZ,
	};

	hh::gfnd::DrawVertex vertices[24]{
		{ corners[0].x(), corners[0].y(), corners[0].z(), colorInt }, { corners[1].x(), corners[1].y(), corners[1].z(), colorInt },
		{ corners[0].x(), corners[0].y(), corners[0].z(), colorInt }, { corners[3].x(), corners[3].y(), corners[3].z(), colorInt },
		{ corners[5].x(), corners[5].y(), corners[5].z(), colorInt }, { corners[1].x(), corners[1].y(), corners[1].z(), colorInt },
		{ corners[5].x(), corners[5].y(), corners[5].z(), colorInt }, { corners[3].x(), corners[3].y(), corners[3].z(), colorInt },
		{ corners[2].x(), corners[2].y(), corners[2].z(), colorInt }, { corners[4].x(), corners[4].y(), corners[4].z(), colorInt },
		{ corners[2].x(), corners[2].y(), corners[2].z(), colorInt }, { corners[6].x(), corners[6].y(), corners[6].z(), colorInt },
		{ corners[7].x(), corners[7].y(), corners[7].z(), colorInt }, { corners[4].x(), corners[4].y(), corners[4].z(), colorInt },
		{ corners[7].x(), corners[7].y(), corners[7].z(), colorInt }, { corners[6].x(), corners[6].y(), corners[6].z(), colorInt },
		{ corners[0].x(), corners[0].y(), corners[0].z(), colorInt }, { corners[2].x(), corners[2].y(), corners[2].z(), colorInt },
		{ corners[1].x(), corners[1].y(), corners[1].z(), colorInt }, { corners[4].x(), corners[4].y(), corners[4].z(), colorInt },
		{ corners[3].x(), corners[3].y(), corners[3].z(), colorInt }, { corners[6].x(), corners[6].y(), corners[6].z(), colorInt },
		{ corners[5].x(), corners[5].y(), corners[5].z(), colorInt }, { corners[7].x(), corners[7].y(), corners[7].z(), colorInt },
	};
	unsigned short indices[24]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };

	ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, 24);
}

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Sphere& sphere, csl::ut::Color8 color) {
	csl::math::Matrix34 transform{};
	transform.translate(sphere.position);
	ctx.DrawSphere(transform, sphere.radius, color);
}

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Cylinder& cylinder, csl::ut::Color8 color) {
	csl::math::Matrix34 transform{};
	transform.translate(cylinder.segment.start);
	ctx.DrawCylinder(transform, (cylinder.segment.end - cylinder.segment.start).norm(), cylinder.radius, color);
}

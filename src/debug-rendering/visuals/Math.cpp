#include "Math.h"

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const Ray3f& ray) {
	hh::gfnd::DrawVertex vertices[2]{
		{ ray.start.x(), ray.start.y(), ray.start.z(), 0xFFFFFFFF },
		{ ray.end.x(), ray.end.y(), ray.end.z(), 0xFFFFFFFF },
	};
}

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const FrustumRays& frustumRays) {
	hh::gfnd::DrawVertex vertices[8]{
		{ frustumRays.topLeft.start.x(), frustumRays.topLeft.start.y(), frustumRays.topLeft.start.z(), 0xFFFFFFFF },
		{ frustumRays.topLeft.end.x(), frustumRays.topLeft.end.y(), frustumRays.topLeft.end.z(), 0xFFFFFFFF },
		{ frustumRays.topRight.start.x(), frustumRays.topRight.start.y(), frustumRays.topRight.start.z(), 0xFFFFFFFF },
		{ frustumRays.topRight.end.x(), frustumRays.topRight.end.y(), frustumRays.topRight.end.z(), 0xFFFFFFFF },
		{ frustumRays.botRight.start.x(), frustumRays.botRight.start.y(), frustumRays.botRight.start.z(), 0xFFFFFFFF },
		{ frustumRays.botRight.end.x(), frustumRays.botRight.end.y(), frustumRays.botRight.end.z(), 0xFFFFFFFF },
		{ frustumRays.botLeft.start.x(), frustumRays.botLeft.start.y(), frustumRays.botLeft.start.z(), 0xFFFFFFFF },
		{ frustumRays.botLeft.end.x(), frustumRays.botLeft.end.y(), frustumRays.botLeft.end.z(), 0xFFFFFFFF },
	};
	unsigned short indices[8]{ 0, 1, 2, 3, 4, 5, 6, 7 };

	ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, vertices, indices, 8);
}

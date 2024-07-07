#pragma once

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Aabb& aabb, csl::ut::Color8 color);
void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Obb& obb, csl::ut::Color8 color);
void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Sphere& sphere, csl::ut::Color8 color);
void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const csl::geom::Cylinder& cylinder, csl::ut::Color8 color);

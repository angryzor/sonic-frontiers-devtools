#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const Ray3f& ray);
void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const FrustumRays& frustumRays);

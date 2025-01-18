#include "OcclusionCapsules.h"
#include <utilities/math/MathUtils.h>

namespace devtools::debug_rendering::renderables {
	void OcclusionCapsules::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx)
	{
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::gfx::GOCOcclusionCapsule::GetClass()) {
					auto* cGoc = static_cast<hh::gfx::GOCOcclusionCapsule*>(goc);

#ifdef DEVTOOLS_TARGET_SDK_wars
					if (cGoc->frame == nullptr)
						continue;

					Eigen::Affine3f affine;
					affine.fromPositionOrientationScale(cGoc->worldPos.m_Position, cGoc->worldPos.m_Rotation, cGoc->scale);
					ctx.DrawSphere({ TransformToAffine3f(cGoc->frame->fullTransform) * affine }, 1, { 255, 0, 255, 255 });
#else
					for (auto& capsule : cGoc->capsules) {
						if (capsule.frame == nullptr)
							continue;

						Eigen::Affine3f affine;
						affine.fromPositionOrientationScale(capsule.worldPos.m_Position, capsule.worldPos.m_Rotation, capsule.scale);
						ctx.DrawSphere({ TransformToAffine3f(capsule.frame->fullTransform) * affine }, 1, { 255, 0, 255, 255 });
					}
#endif
				}
			}
		}
	}
}

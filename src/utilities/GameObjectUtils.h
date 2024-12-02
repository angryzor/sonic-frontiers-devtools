#pragma once

#ifndef DEVTOOLS_TARGET_SDK_wars
void ApplyComponentUpdateFlag(hh::game::GOComponent& component, hh::fnd::UpdatingPhase phase);
void SetComponentUpdateFlag(hh::game::GOComponent& component, hh::fnd::UpdatingPhase phase, bool enabled);
#endif
void EnsureGOCTransformIsUpdating(hh::game::GOCTransform& gocTransform);

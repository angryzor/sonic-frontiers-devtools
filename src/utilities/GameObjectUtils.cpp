#include "GameObjectUtils.h"

#ifndef DEVTOOLS_TARGET_SDK_wars
void ApplyComponentUpdateFlag(hh::game::GOComponent& component, hh::fnd::UpdatingPhase phase)
{
	auto* obj = component.owner;

	if (component.updateFlags.test(phase)) {
		auto it = obj->componentsByUpdatingPhase[static_cast<size_t>(phase)].find(&component);
		if (it == -1)
			obj->componentsByUpdatingPhase[static_cast<size_t>(phase)].push_back(&component);
	}
	else {
		auto it = obj->componentsByUpdatingPhase[static_cast<size_t>(phase)].find(&component);
		if (it != -1)
			obj->componentsByUpdatingPhase[static_cast<size_t>(phase)].remove(it);
	}
	obj->UNSAFE_SetComponentLengths(phase);
	obj->UNSAFE_SetUpdateFlags(phase);
}

void SetComponentUpdateFlag(hh::game::GOComponent& component, hh::fnd::UpdatingPhase phase, bool enabled)
{
	if (component.updateFlags.test(phase) != enabled) {
		component.updateFlags.set(phase, enabled);
		ApplyComponentUpdateFlag(component, phase);
	}
}
#endif

void EnsureGOCTransformIsUpdating(hh::game::GOCTransform& gocTransform)
{
#ifdef DEVTOOLS_TARGET_SDK_miller
	SetComponentUpdateFlag(gocTransform, hh::fnd::UpdatingPhase::PRE_ANIM, true);
#endif
}

#include "GOCActivator.h"
#include <ui/common/editors/Basic.h>

void RenderComponentInspector(hh::game::GOCActivator& component) {
	Editor("range", component.range);
	Editor("distance", component.distance);
	Editor("unk101", component.unk101);
	Editor("location", component.position);
	Editor("rangePlusDistanceSquared", component.rangePlusDistanceSquared);
	Editor("enabled", component.enabled);
}

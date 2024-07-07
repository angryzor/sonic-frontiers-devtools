#include "GOCActivator.h"
#include <ui/common/editors/Basic.h>

void RenderComponentInspector(hh::game::GOCActivator& component) {
	Editor("range", component.range);
	Editor("distance", component.distance);
	Editor("unk101", component.unk101);
	Editor("location", component.z);
	Editor("unk103", component.unk103);
	Editor("enabled", component.enabled);
}

#include "PathComponent.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/Viewers/Basic.h>

void RenderComponentInspector(hh::path::PathComponent& component) {
	Viewer("Name", component.GetName());
	Viewer("matrixB0", component.matrixB0);
	Viewer("matrixF0", component.matrixF0);
	Viewer("unk130", component.unk130);
}

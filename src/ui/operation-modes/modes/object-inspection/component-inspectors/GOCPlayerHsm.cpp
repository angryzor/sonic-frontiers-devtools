#include "GOCPlayerHsm.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/HsmBase.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app::player::GOCPlayerHsm& component) {
	Editor("HSM", component.hsm);
}

#include "GOCPlayerCollider.h"
#include <ui/common/editors/Basic.h>

void RenderComponentInspector(app::player::GOCPlayerCollider& component) {
	Editor("playerCollision.distance", component.GetPlayerCollision()->distance);
	Editor("playerCollision.unk2C", component.GetPlayerCollision()->qword2C);
	Editor("playerCollision.unk30", component.GetPlayerCollision()->dword30);
	Editor("playerCollision.collisionMask", component.GetPlayerCollision()->collisionMask);
}

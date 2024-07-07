#include "GOCEvent.h"
#include <ui/common/inputs/Basic.h>

void RenderComponentInspector(app::game::GOCEvent& component)
{
	CheckboxFlags("Triggered", component.flags, app::game::GOCEvent::Flag::ENABLED);
	CheckboxFlags("Unknown 1", component.flags, app::game::GOCEvent::Flag::UNK1);
	CheckboxFlags("Deactivate after event (maybe)", component.flags, app::game::GOCEvent::Flag::MAYBE_DEACTIVATE_AFTER_EVENT);
}

const char* gocEventCollisionConditions[] = { "ON_TRIGGER", "ON_STAY", "UNKNOWN", "PULSE" };

void RenderComponentInspector(app::game::GOCEventCollision& component)
{
	ImGui::SliderFloat("Time left", &component.timeLeft, 0, component.lifetime);
	ImGui::DragFloat("Lifetime", &component.lifetime);
	ImGui::Checkbox("Lifetime reached", &component.lifetimeReached);
	ComboEnum("Condition", component.collisionListener.condition, gocEventCollisionConditions);
}

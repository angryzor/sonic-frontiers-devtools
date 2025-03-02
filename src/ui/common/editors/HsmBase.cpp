#include "HsmBase.h"

bool Editor(const char* label, hh::ut::HsmBase& hsm) {
	bool edited{};

	int bottomStateId = hsm.GetBottomStateId();
	char selectableLabel[100]{ "<none>" };

	if (bottomStateId != -1)
		snprintf(selectableLabel, sizeof(selectableLabel), "%d - %s", bottomStateId, hsm.stateManager->stateDescs[bottomStateId]->name);

	if (ImGui::BeginCombo("Current state", selectableLabel)) {
		for (int i = 0; i < hsm.stateManager->stateDescs.size(); i++) {
			if (auto* stateDesc = hsm.stateManager->stateDescs[i]) {
				snprintf(selectableLabel, sizeof(selectableLabel), "%d - %s", i, stateDesc->name);

				if (ImGui::Selectable(selectableLabel, i == bottomStateId)) {
					hsm.ChangeState(i);
					edited = true;
				}

				if (i == bottomStateId)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	for (auto& state : hsm.stateStack)
		ImGui::Text("%d - %s", state->id, state->name);

	return edited;
}

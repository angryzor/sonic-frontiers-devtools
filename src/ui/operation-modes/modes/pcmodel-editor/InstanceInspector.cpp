#include "InstanceInspector.h"
#include "Behaviors.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>

using namespace ucsl::resources::pointcloud::v2;

const char* rotationOrderNames[]{ "NONE", "XYZ", "YZX", "ZXY", "XZY", "YXZ", "ZYX" };

bool Editor(const char* label, InstanceData& instance, hh::gfx::ResPointcloud* resource) {
	bool edited{};
	edited |= InputText("Name", instance.name, resource);
	edited |= InputText("Resource name", instance.resourceName, resource);
	edited |= Editor("Position", instance.position);
	edited |= EulerEditor("Rotation", instance.rotation);
	edited |= ComboEnum("Rotation order", instance.rotationOrder, rotationOrderNames);
	edited |= Editor("Scale", instance.scale);
	//edited |= Editor("Unk2", instance.unk2);
	return edited;
}

namespace ui::operation_modes::modes::pcmodel_editor {
	void InstanceInspector::RenderPanel()
	{
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();
		if (selection.size() == 0)
			ImGui::Text("Select a instance in the left pane.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple instances selected");
		else
			if (Editor("Instance", *selection[0], GetContext().resource))
				Dispatch(SelectionTransformationBehavior<Context>::SelectionTransformChangedAction{});
	}

	PanelTraits InstanceInspector::GetPanelTraits() const
	{
		return { "Instance inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}
}

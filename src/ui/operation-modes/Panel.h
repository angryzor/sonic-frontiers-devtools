#pragma once
#include "OperationModeBehavior.h"

struct PanelTraits {
	const char* title;
	ImVec2 position;
	ImVec2 size;
	ImVec2 pivot;
};

template<typename Context>
class OperationMode;

class PanelBase : public CompatibleObject {
public:
	using CompatibleObject::CompatibleObject;

	virtual PanelTraits GetPanelTraits() const = 0;
	virtual void RenderPanel() = 0;
};

template<typename Context>
class Panel : public PanelBase {
	OperationMode<Context>& operationMode;

protected:
	Context& GetContext() { return operationMode.context; }
	template<typename T> T* GetBehavior() { return operationMode.GetBehavior<T>(); }

public:
	Panel(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : PanelBase{ allocator }, operationMode{ operationMode } { }

	void Render()
	{
		const ImGuiWindowFlags windowFlags = 0;

		auto traits = GetPanelTraits();

		ImGui::SetNextWindowPos(traits.position, ImGuiCond_FirstUseEver, traits.pivot);
		ImGui::SetNextWindowSize(traits.size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(traits.title, NULL, windowFlags)) {
			RenderPanel();
		}

		ImGui::End();
	}

	void Dispatch(const ActionBase& action) {
		operationMode.Dispatch(action);
	}

	virtual void ProcessAction(const ActionBase& action) {
	}
};

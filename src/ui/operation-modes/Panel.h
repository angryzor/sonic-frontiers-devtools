#pragma once
#include <ui/Component.h>
#include "OperationModeBehavior.h"

struct PanelTraits {
	const char* title;
	ImVec2 position;
	ImVec2 size;
	ImVec2 pivot;
};

template<typename Context>
class OperationMode;

template<typename Context>
class Panel : public Component {
	OperationMode<Context>& operationMode;

protected:
	Context& GetContext() { return operationMode.context; }
	template<typename T> T* GetBehavior() { return operationMode.GetBehavior<T>(); }

public:
	Panel(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Component{ allocator }, operationMode{ operationMode } { }

	virtual void Render() override
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

	virtual PanelTraits GetPanelTraits() const = 0;
	virtual void RenderPanel() = 0;
};

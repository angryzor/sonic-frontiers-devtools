#include "Pch.h"
#include "StandaloneWindow.h"
#include "Desktop.h"

StandaloneWindow::StandaloneWindow(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{ allocator, true } {
	Desktop::instance->AddStandaloneWindow(this);
}

void StandaloneWindow::SetTitle(const char* title) {
	this->title.Set(title);
}

void StandaloneWindow::PreRender() {
}

void StandaloneWindow::Render() {
	PreRender();

	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	bool shouldStayOpen{ true };

	if (ImGui::Begin(title.c_str(), &shouldStayOpen, windowFlags)) {
		RenderContents();
	}
	ImGui::End();

	if (!shouldStayOpen) {
		Desktop::instance->RemoveStandaloneWindow(this);
	}
}

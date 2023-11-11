#include "Pch.h"
#include "StandaloneWindow.h"
#include "Desktop.h"

StandaloneWindow::StandaloneWindow(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{ allocator, true } {
	Desktop::instance->AddStandaloneWindow(this);
}

void StandaloneWindow::SetTitle(const char* title) {
	char buf[500];
	snprintf(buf, sizeof(buf), "%s###%zx", title, reinterpret_cast<size_t>(this));
	this->title.Set(buf);
}

void StandaloneWindow::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
}

bool StandaloneWindow::Render() {
	PreRender();

	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	bool shouldStayOpen{ true };

	if (ImGui::Begin(title.c_str(), &shouldStayOpen, windowFlags)) {
		RenderContents();
	}
	ImGui::End();

	return shouldStayOpen;
}

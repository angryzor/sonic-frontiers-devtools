#include "Pch.h"
#include "StandaloneWindow.h"
#include "Desktop.h"

StandaloneWindow::StandaloneWindow(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{ allocator, true } {
	Desktop::instance->AddStandaloneWindow(this);
}

void StandaloneWindow::SetTitle(const char* title) {
	char buf[500];
	snprintf(buf, sizeof(buf), "%s###%zx%s", title, reinterpret_cast<size_t>(this), title);
	this->title.Set(buf);
}

void StandaloneWindow::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
}

void StandaloneWindow::PostRender() {
}

bool StandaloneWindow::Render() {
	PreRender();

	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	bool shouldStayOpen{ true };

	if (ImGui::Begin(title.c_str(), &shouldStayOpen, windowFlags))
		RenderContents();
	ImGui::End();

	PostRender();

	return shouldStayOpen;
}

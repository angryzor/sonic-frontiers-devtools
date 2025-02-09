#include "imtimeline.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <cmath>
#include <algorithm>

namespace ImTimeline {
	static constexpr float nameColumnWidth = 300.0f;
	static constexpr float playHeadSize = 10.0f;
	static constexpr float minZoom = 0.05f;
	static constexpr float maxZoom = 20.0f;

	struct ImTimelineContext {
		float currentTime{};
		float length{};
		float divisionLength{};
		float zoom{ 1.0f };
		float trackHeight{};
		ImVec2 timelineScreenPos{};
		ImVec2 trackScreenPos{};
		ImVec2 clipDimensions{};
	};

	static ImTimelineContext* gCtx{};

	ImTimelineContext* CreateContext() {
		return IM_NEW(ImTimelineContext)();
	}

	void DestroyContext(ImTimelineContext* context) {
		IM_DELETE(context);
	}

	void Begin(ImTimelineContext* context) {
		gCtx = context;
	}

	void End() {
		gCtx = nullptr;
	}

	bool TimeSelect(const char* id, float* time, ImVec2 size, float min = 0.0f, float max = gCtx->length) {
		auto screenPos = ImGui::GetCursorScreenPos();

		ImGui::PushID(id);
		ImGui::InvisibleButton("button", size);

		auto* s = ImGui::GetStateStorage();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			*time = std::max(min, std::min(max, (ImGui::GetMousePos().x - screenPos.x) / gCtx->zoom));
			s->SetBool(ImGui::GetID("isDragging"), true);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			s->SetBool(ImGui::GetID("isDragging"), false);

		if (s->GetBool(ImGui::GetID("isDragging")) && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			*time = std::max(min, std::min(max, (ImGui::GetMousePos().x - screenPos.x) / gCtx->zoom));

			ImGui::PopID();
			return true;
		}

		ImGui::PopID();
		return false;
	}

	bool TimeDrag(const char* id, float* time, ImVec2 size, float min = 0.0f, float max = gCtx->length) {
		ImGui::PushID(id);
		ImGui::InvisibleButton("button", size);

		auto* s = ImGui::GetStateStorage();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			s->SetBool(ImGui::GetID("isDragging"), true);
			s->SetFloat(ImGui::GetID("origTime"), *time);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			s->SetBool(ImGui::GetID("isDragging"), false);

		if (s->GetBool(ImGui::GetID("isDragging")) && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			*time = std::max(min, std::min(max, s->GetFloat(ImGui::GetID("origTime")) + ImGui::GetMouseDragDelta().x / gCtx->zoom));

			ImGui::PopID();
			return true;
		}

		ImGui::PopID();
		return false;
	}

	bool PlayHead(const char* id, float* currentTime, float height = 20.0f, float minTickSpacing = 10.0f) {
		auto* dl = ImGui::GetCurrentWindow()->DrawList;

		auto screenPos = ImGui::GetCursorScreenPos();
		static constexpr auto playHeadHeight = playHeadSize * 0.86602540378443864676372317075294f;

		bool changed = gCtx->length > 0.0f ? TimeSelect(id, currentTime, ImVec2(gCtx->length * gCtx->zoom, height)) : false;

		auto playHeadScreenPos = ImVec2(screenPos.x + *currentTime * gCtx->zoom, screenPos.y + height);

		dl->AddTriangleFilled(
			playHeadScreenPos + ImVec2(-playHeadSize / 2, -playHeadHeight),
			playHeadScreenPos + ImVec2(playHeadSize / 2, -playHeadHeight),
			playHeadScreenPos + ImVec2(0.0f, 0.0f),
			0xFFFFFFFF
		);

		return changed;
	}

	bool BeginTimeline(const char* id, float* currentTime, float length, float divisionLength, bool* playing, bool* currentTimeChanged)
	{
		ImGui::PushID(id);
		ImGui::Checkbox("Play", playing);

		if (ImGui::IsKeyChordPressed(ImGuiKey_MouseWheelY | ImGuiMod_Ctrl))
			gCtx->zoom = std::max(minZoom, std::min(maxZoom, gCtx->zoom + ImGui::GetIO().MouseWheel));

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

		if (!ImGui::BeginTable("Timeline", 2, ImGuiTableFlags_BordersInner | ImGuiTableFlags_ScrollX)) {
			ImGui::PopStyleVar();
			ImGui::PopID();
			return false;
		}

		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("Track name", ImGuiTableColumnFlags_WidthFixed, nameColumnWidth);
		ImGui::TableSetupColumn("Timeline", ImGuiTableColumnFlags_WidthFixed, length * gCtx->zoom);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();

		gCtx->timelineScreenPos = ImGui::GetCursorScreenPos();
		gCtx->length = length;
		gCtx->divisionLength = divisionLength;

		if (PlayHead("playhead", currentTime) && currentTimeChanged)
			*currentTimeChanged = true;

		gCtx->currentTime = *currentTime;

		return true;
	}

	void EndTimeline()
	{
		auto* dl = ImGui::GetCurrentWindow()->DrawList;
		auto screenPosY = ImGui::GetCursorScreenPos().y;

		auto sliceCount = static_cast<size_t>(gCtx->length / gCtx->divisionLength);
		auto sliceRemainder = std::fmodf(gCtx->length, gCtx->divisionLength);

		// Render "slices" as odd/even background colors.
		for (size_t i = 0; i < sliceCount; i++)
			dl->AddRectFilled(gCtx->timelineScreenPos + ImVec2(i * gCtx->divisionLength * gCtx->zoom, 20.0f), ImVec2(gCtx->timelineScreenPos.x + (i * gCtx->divisionLength + gCtx->divisionLength / 2.0f) * gCtx->zoom, screenPosY), 0x20FFFFFF);

		// Render last slice if it exists.
		if (sliceRemainder > 0.0f)
			dl->AddRectFilled(gCtx->timelineScreenPos + ImVec2(sliceCount * gCtx->divisionLength * gCtx->zoom, 20.0f), ImVec2(gCtx->timelineScreenPos.x + (sliceCount * gCtx->divisionLength + std::min(gCtx->divisionLength / 2.0f, sliceRemainder)) * gCtx->zoom, screenPosY), 0x20FFFFFF);

		dl->AddLine(gCtx->timelineScreenPos + ImVec2(gCtx->currentTime * gCtx->zoom, 20.0f), ImVec2(gCtx->timelineScreenPos.x + gCtx->currentTime * gCtx->zoom, screenPosY), 0xFFFFFFFF);

		ImGui::EndTable();
		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	bool BeginTrackGroup(const char* id) {
		ImGui::TableNextRow(ImGuiTableRowFlags_None, 20.0f);
		ImGui::TableNextColumn();

		auto isOpen = ImGui::TreeNodeEx(id, ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen, "%s", id);

		ImGui::TableNextColumn();

		return isOpen;
	}

	void EndTrackGroup() {
		ImGui::TreePop();
	}

	bool BeginTrack(const char* id) {
		ImGui::TableNextRow(ImGuiTableRowFlags_None, 20.0f);
		ImGui::TableNextColumn();

		auto isOpen = ImGui::TreeNodeEx(id, ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", id);

		ImGui::TableNextColumn();

		gCtx->trackScreenPos = ImGui::GetCursorScreenPos();

		ImGui::PushID(id);

		return isOpen;
	}

	void EndTrack()
	{
		ImGui::PopID();
	}

	bool BeginClip(const char* id, float* startTime, float* endTime, float height, bool* startTimeChanged, bool* endTimeChanged, bool* moved)
	{
		float timeDiff = *endTime - *startTime;

		gCtx->clipDimensions = ImVec2(timeDiff * gCtx->zoom, height);

		ImGui::SetCursorScreenPos(gCtx->trackScreenPos + ImVec2(*startTime * gCtx->zoom, 0.0f));

		if (!ImGui::BeginChild(id, gCtx->clipDimensions + ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing()))) {
			ImGui::EndChild();
			return false;
		}

		auto* dl = ImGui::GetCurrentWindow()->DrawList;

		dl->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImVec2(gCtx->clipDimensions.x, ImGui::GetTextLineHeightWithSpacing()), 0xFF5A6AED);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		ImGui::Text("%s", id);
		ImGui::PopStyleColor();
		ImGui::SetItemTooltip("%s", id);

		auto pos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		if (TimeDrag("startTimeHandle", startTime, ImVec2(5.0f, ImGui::GetTextLineHeightWithSpacing()))) {
			*startTime = std::min(*startTime, *endTime - 1.0f);

			if (startTimeChanged)
				*startTimeChanged = true;
		}

		if (ImGui::IsItemHovered())
			dl->AddRectFilled(ImGui::GetWindowPos() + ImVec2(0.0f, 0.0f), ImGui::GetWindowPos() + ImVec2(5.0f, ImGui::GetTextLineHeightWithSpacing()), ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_SeparatorHovered)));

		ImGui::SetCursorPos(ImVec2(gCtx->clipDimensions.x - 5.0f, 0.0f));
		if (TimeDrag("endTimeHandle", endTime, ImVec2(5.0f, ImGui::GetTextLineHeightWithSpacing()))) {
			*endTime = std::max(*endTime, *startTime + 1.0f);

			if (endTimeChanged)
				*endTimeChanged = true;
		}

		if (ImGui::IsItemHovered())
			dl->AddRectFilled(ImGui::GetWindowPos() + ImVec2(gCtx->clipDimensions.x - 5.0f, 0.0f), ImGui::GetWindowPos() + ImVec2(gCtx->clipDimensions.x, ImGui::GetTextLineHeightWithSpacing()), ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_SeparatorHovered)));

		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		if (TimeDrag("titlebar", startTime, ImVec2(gCtx->clipDimensions.x, ImGui::GetTextLineHeightWithSpacing()), 0.0f, gCtx->length - timeDiff)) {
			*endTime = *startTime + timeDiff;

			if (moved)
				*moved = true;
			if (startTimeChanged)
				*startTimeChanged = true;
			if (endTimeChanged)
				*endTimeChanged = true;
		}

		ImGui::SetCursorPos(pos);

		return true;
	}

	void EndClip()
	{
		ImGui::SetCursorPos(ImVec2(0, 0));
		if (ImGui::BeginChild("overlays", gCtx->clipDimensions + ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_None, ImGuiWindowFlags_NoInputs)) {
			auto screenPos = ImGui::GetCursorScreenPos();

			ImGui::GetCurrentWindow()->DrawList->AddRect(screenPos, screenPos + gCtx->clipDimensions + ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing()), 0xFF000000);
			ImGui::GetCurrentWindow()->DrawList->AddLine(screenPos + ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing()), screenPos + ImVec2(gCtx->clipDimensions.x, ImGui::GetTextLineHeightWithSpacing()), 0xFF000000);
		}
		ImGui::EndChild();
		ImGui::EndChild();
	}

	bool Event(const char* id, float* time)
	{
		static constexpr float radius = 5.0f;
		static constexpr float spacing = 5.0f;

		ImGui::SetCursorScreenPos(gCtx->trackScreenPos + ImVec2(*time * gCtx->zoom, 0.0f));

		auto screenPos = ImGui::GetCursorScreenPos();

		ImGui::PushID(id);

		ImGui::GetCurrentWindow()->DrawList->AddCircleFilled(screenPos + ImVec2(0.0f, radius + spacing), radius, 0xFF5A6AED);
		ImGui::GetCurrentWindow()->DrawList->AddCircle(screenPos + ImVec2(0.0f, radius + spacing), radius, 0xFF000000);

		bool changed = TimeDrag("handle", time, ImVec2((radius + spacing) * 2.0f, (radius + spacing) * 2.0f));

		ImGui::PopID();

		return changed;
	}

	bool IsNameColumnHovered() {
		return ImGui::TableGetColumnFlags(0) & ImGuiTableColumnFlags_IsHovered;
	}

	bool IsTimelineColumnHovered() {
		return ImGui::TableGetColumnFlags(1) & ImGuiTableColumnFlags_IsHovered;
	}

	float GetMouseTime() {
		auto mousePos = ImGui::GetMousePos();

		return (mousePos.x - gCtx->timelineScreenPos.x) / gCtx->zoom;
	}

	ImVec2 GetClipSize() {
		return gCtx->clipDimensions;
	}
}

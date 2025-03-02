#pragma once
#include <imgui.h>

namespace ImTimeline {
	struct ImTimelineContext;

	ImTimelineContext* CreateContext();
	void DestroyContext(ImTimelineContext* context);
	void Begin(ImTimelineContext* context);
	void End();

	bool BeginTimeline(const char* id, float* currentTime, float length, float divisionLength, bool* playing, bool* currentTimeChanged = nullptr);
	void EndTimeline();

	bool BeginTrack(const char* id);
	void EndTrack();

	bool BeginTrackGroup(const char* id);
	void EndTrackGroup();

	bool BeginClip(const char* id, float* startTime, float* endTime, float height = 0.0f, bool* startTimeChanged = nullptr, bool* endTimeChanged = nullptr, bool* moved = nullptr);
	void EndClip();

	bool Event(const char* id, float* time, bool* clicked = nullptr);

	bool IsNameColumnHovered();
	bool IsTimelineColumnHovered();
	float GetMouseTime();
	ImVec2 GetClipSize();
}

#pragma once

template<typename T, size_t Len>
static void Inspector(const char* label, T(&arr)[Len], bool readonly) {
	char name[200];
	snprintf(name, sizeof(name), "%s[0..%zd]", label, Len);

	if (ImGui::TreeNode(name)) {
		for (size_t i = 0; i < Len; i++) {
			snprintf(name, sizeof(name), "%s[%zd]", label, i);
			SubInspector(name, arr[i], readonly);
		}

		ImGui::TreePop();
	}
}

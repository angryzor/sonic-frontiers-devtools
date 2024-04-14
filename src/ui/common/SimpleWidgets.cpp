#include "SimpleWidgets.h"
#include <utilities/math/MathUtils.h>

void MatrixValues(const Eigen::Matrix4f& mat) {
	ImGui::Text("%f %f %f %f", mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3));
	ImGui::Text("%f %f %f %f", mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3));
	ImGui::Text("%f %f %f %f", mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3));
	ImGui::Text("%f %f %f %f", mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
}

void ViewportDataInfo(const hh::gfnd::ViewportData& viewport)
{
	ImGui::SeparatorText("Dimensions");
	ImGui::Text("X: %f", viewport.viewportDimensions.x);
	ImGui::Text("Y: %f", viewport.viewportDimensions.y);
	ImGui::Text("Width: %f", viewport.viewportDimensions.width);
	ImGui::Text("Height: %f", viewport.viewportDimensions.height);
	ImGui::Text("Render width: %f", viewport.viewportDimensions.maybeRenderWidth);
	ImGui::Text("Render height: %f", viewport.viewportDimensions.maybeRenderHeight);

	ImGui::SeparatorText("Projection settings");
	ImGui::Text("Aspect ratio: %f", viewport.aspectRatio);
	ImGui::Text("FOV: %f", viewport.fov);
	ImGui::Text("Near clipping plane: %f", viewport.nearClip);
	ImGui::Text("Far clipping plane: %f", viewport.farClip);

	ImGui::SeparatorText("View matrix");
	MatrixValues(viewport.viewMatrix);

	ImGui::SeparatorText("Inverse view matrix");
	MatrixValues(viewport.inverseViewMatrix);

	ImGui::SeparatorText("Projection matrix");
	MatrixValues(viewport.projMatrix);
}

void VectorEditor(const char* label, csl::math::Vector3& vec) {
	ImGui::DragFloat3(label, vec.data(), 0.01f);
}

void QuaternionEditor(const char* label, csl::math::Quaternion& quat) {
	auto euler = MatrixToEuler(quat.toRotationMatrix());

	ImGui::DragFloat3(label, euler.data(), 0.005f);
	if (ImGui::IsItemEdited())
		quat = EulerToQuat(euler);
}

void WorldPositionEditor(hh::fnd::WorldPosition& worldPos) {
	ImGui::BeginGroup();
	VectorEditor("Position", worldPos.m_Position);
	QuaternionEditor("Rotation", worldPos.m_Rotation);
	ImGui::EndGroup();
}

void TransformEditor(csl::math::Transform& transform) {
	ImGui::BeginGroup();
	VectorEditor("Position", transform.position);
	QuaternionEditor("Rotation", transform.rotation);
	VectorEditor("Scale", transform.scale);
	ImGui::EndGroup();
}

bool BeginVectorViewerTable (const char* id, bool withWAxis) {
	bool tableResult = ImGui::BeginTable(id, withWAxis ? 5 : 4);

	if (tableResult) {
		ImGui::GetStateStorage()->SetBool(ImGui::GetID("WithW"), withWAxis);

		ImGui::TableSetupColumn("X");
		ImGui::TableSetupColumn("Y");
		ImGui::TableSetupColumn("Z");
		if (withWAxis)
			ImGui::TableSetupColumn("W");
		ImGui::TableSetupColumn("");

		ImGui::TableHeadersRow();
	}

	return tableResult;
}

void EndVectorViewerTable() {
	ImGui::EndTable();
}

void VectorViewerTableItem(const char* label, const csl::math::Vector3& vec) {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.x());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.y());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.z());
	if (ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")))
		ImGui::TableNextColumn();
	ImGui::TableNextColumn();
	ImGui::Text(label);
}

void VectorViewerTableItem(const char* label, const csl::math::Vector4& vec) {
	IM_ASSERT(ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.x());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.y());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.z());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.w());
	ImGui::TableNextColumn();
	ImGui::Text(label);
}

void VectorViewerTableItem(const char* label, const csl::math::Quaternion& quat) {
	IM_ASSERT(ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.x());
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.y());
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.z());
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.w());
	ImGui::TableNextColumn();
	ImGui::Text(label);
}

void WorldPositionViewer(const char* id, const hh::fnd::WorldPosition& worldPos)
{
	if (BeginVectorViewerTable(id, true)) {
		VectorViewerTableItem("Position", worldPos.m_Position);
		VectorViewerTableItem("Rotation", worldPos.m_Rotation);
		EndVectorViewerTable();
	}
}

void TransformViewer(const char* id, csl::math::Transform& transform)
{
	if (BeginVectorViewerTable(id, true)) {
		VectorViewerTableItem("Position", transform.position);
		VectorViewerTableItem("Rotation", transform.rotation);
		VectorViewerTableItem("Scale", transform.scale);
		EndVectorViewerTable();
	}
}

static char dummy[1] = { '\0' };

static int StringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto my_str = static_cast<csl::ut::String*>(data->UserData);
		my_str->reserve(data->BufSize);
		data->Buf = my_str->c_str();
	}
	return 0;
}

void InputText(const char* label, csl::ut::String* str, ImGuiInputTextFlags flags) {
	ImGui::InputText(label, str->c_str() == nullptr ? dummy : str->c_str(), str->size(), flags | ImGuiInputTextFlags_CallbackResize, StringResizeCallback, str);
}

// This is a completely nonstandard hack around the fact that these things don't have resizeable buffers
// (instead they are always allocated to the exact string length) but it shouldn't cause issues.
class ResizeableVariableString : public csl::ut::VariableString {
public:
	void resize(size_t newSize) {
		size_t allocatorAddr = reinterpret_cast<size_t>(m_pAllocator);
		csl::fnd::IAllocator* allocator = m_pAllocator == nullptr ? hh::fnd::MemoryRouter::GetModuleAllocator() : reinterpret_cast<csl::fnd::IAllocator*>(allocatorAddr & ~1);

		char* oldStr = m_pStr;
		m_pStr = (char*)allocator->Alloc(newSize, 1);
		m_pAllocator = reinterpret_cast<csl::fnd::IAllocator*>(reinterpret_cast<size_t>(allocator) | 1);

		if (oldStr) {
			strcpy_s(m_pStr, newSize, oldStr);

			if (allocatorAddr & 1)
				allocator->Free(oldStr);
		}
		else {
			m_pStr[0] = '\0';
		}
	}

	size_t size() {
		return m_pStr == nullptr ? 0 : strlen(m_pStr);
	}

	char* c_str() {
		return m_pStr == nullptr ? dummy : m_pStr;
	}
};

static int VariableStringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto my_str = static_cast<ResizeableVariableString*>(data->UserData);
		//IM_ASSERT(my_str->c_str() == data->Buf);
		my_str->resize(data->BufSize);
		data->Buf = my_str->c_str();
	}
	return 0;
}

void InputText(const char* label, csl::ut::VariableString *str, ImGuiInputTextFlags flags) {
	auto* rstr = static_cast<ResizeableVariableString*>(str);

	ImGui::InputText(label, rstr->c_str(), rstr->size() + 1, flags | ImGuiInputTextFlags_CallbackResize, VariableStringResizeCallback, rstr);
}

void InputObjectId(const char* label, hh::game::ObjectId* id) {
	auto* objWorld = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjectWorld>();

	if (objWorld == nullptr) {
		ImGui::Text("%s: %016zx%016zx", label, id->groupId, id->objectId);
	}
	else {
		char unkNameBuf[50];
		sprintf_s(unkNameBuf, 50, "<%016zx%016zx>", id->groupId, id->objectId);

		const char* name = unkNameBuf;

		for (auto* chunk : objWorld->GetWorldChunks()) {
			int idx = chunk->GetObjectIndexById(*id);

			if (idx != -1)
				name = chunk->GetWorldObjectStatusByIndex(idx).objectData->name;
		}

		if (ImGui::BeginCombo(label, name)) {
			for (auto* chunk : objWorld->GetWorldChunks()) {
				for (auto* layers : chunk->GetLayers()) {
					for (auto* obj : layers->GetResource()->GetObjects()) {
						if (ImGui::Selectable(obj->name))
							*id = obj->id;
						if (*id == obj->id)
							ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
	}
}

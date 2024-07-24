#include "Needle.h"
#include <imgui_internal.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

template<typename T>
bool BitFieldEditor(const char* label, ImGuiDataType dtype, T* field, T startBit, T size) {
	bool edited{};
	T minVal = 0;
	T maxVal = ((1 << size) - 1);
	T orig = (*field >> startBit) & maxVal;
	T v = orig;

	if (ImGui::DragScalar(label, dtype, &v, 1.0f, &minVal, &maxVal)) {
		*field = (*field & ~(maxVal << startBit)) | (v << startBit);
		edited = true;
	}

	return edited;
}

bool Editor(const char* label, hh::needle::RsFlagMask& mask)
{
	bool edited{};

	if (ImGui::TreeNode(label)) {
		ImGui::Text("Mask: %zx", mask.flags);
		edited |= BitFieldEditor("Blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 28ull, 4ull);
		edited |= BitFieldEditor("Blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 23ull, 4ull);
		edited |= BitFieldEditor("Blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 19ull, 3ull);
		edited |= BitFieldEditor("Alpha blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 14ull, 4ull);
		edited |= BitFieldEditor("Alpha blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 9ull, 4ull);
		edited |= BitFieldEditor("Alpha blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 5ull, 3ull);
		edited |= ImGui::CheckboxFlags("Enable blending", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 46ull);
		edited |= ImGui::CheckboxFlags("Cull front", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 41ull);
		edited |= ImGui::CheckboxFlags("Cull back", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 40ull);
		edited |= ImGui::CheckboxFlags("Cull unknown", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 39ull);
		ImGui::Text("Mask: %zx", mask.flags);
		ImGui::TreePop();
	}

	return edited;
}

void Viewer(const char* label, const hh::needle::FxCamera& camera) {
	if (ImGui::TreeNode(label)) {
		if (BeginVectorViewerTable("Eye and LookAt", false)) {
			Viewer("Eye position", camera.GetEyePos());
			Viewer("LookAt position", camera.GetLookAtPos());
			EndVectorViewerTable();
		}
		Viewer("View matrix", camera.GetViewMatrix());
		Viewer("Projection matrix", camera.GetProjectionMatrix());
		Viewer("Field of view", camera.GetFieldOfView());

		float nearClip{}, farClip{};
		camera.GetClipPlanes(&nearClip, &farClip);
		Viewer("Near clip plane", nearClip);
		Viewer("Far clip plane", farClip);
		ImGui::TreePop();
	}
}

void Viewer(const char* label, const hh::gfx::RenderManager::Impl::LocalFxCamera& camera)
{
	if (ImGui::TreeNode(label)) {
		Viewer("Viewport ID", camera.id);
		Viewer("FxCamera", static_cast<const hh::needle::FxCamera&>(camera));
		ImGui::TreePop();
	}
}

bool Editor(const char* label, hh::needle::SupportFX::FxViewport& viewport) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		if (viewport.camera)
			Viewer("Camera", *viewport.camera);
		edited |= Editor("x", viewport.x);
		edited |= Editor("y", viewport.y);
		edited |= Editor("width", viewport.width);
		edited |= Editor("height", viewport.height);
		edited |= Editor("prevRenderWidth", viewport.prevRenderWidth);
		edited |= Editor("prevRenderHeight", viewport.prevRenderHeight);
		edited |= Editor("renderWidth", viewport.renderWidth);
		edited |= Editor("renderHeight", viewport.renderHeight);
		edited |= Editor("unk7", viewport.unk7);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, hh::needle::SupportFX::FxRenderParam& renderParam) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("Viewports", renderParam.viewports);
	edited |= Editor("Viewport count", renderParam.numViewports);
	edited |= Editor("unk32", renderParam.unk32);
	edited |= Editor("unk33", renderParam.unk33);
	edited |= Editor("unk34", renderParam.unk34);
	edited |= Editor("unk35", renderParam.unk35);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::PipelineInfo& pipelineInfo, hh::needle::WorldRenderingPipeline& pipeline) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("dword8", pipelineInfo.dword8);
	edited |= Editor("qword10", pipelineInfo.qword10);
	edited |= Editor("qword18", pipelineInfo.qword18);
	edited |= Editor("qword20", pipelineInfo.qword20);
	edited |= Editor("Render parameters", pipelineInfo.renderParam);
	edited |= Editor("qword138", pipelineInfo.qword138);
	edited |= Editor("qword148", pipelineInfo.qword148);
	edited |= Editor("qword150", pipelineInfo.qword150);
	if (pipelineInfo.renderUnitNameId)
		Viewer("renderUnitNameId", pipelineInfo.renderUnitNameId->name);
	if (pipelineInfo.sceneNameId)
		Viewer("sceneNameId", pipelineInfo.sceneNameId->name);
	edited |= Editor("Camera ID", pipelineInfo.cameraId);
	//edited |= Editor("sceneParamContainer", pipelineInfo.sceneParamContainer);
	edited |= Editor("qword178", pipelineInfo.qword178);
	edited |= Editor("qword180", pipelineInfo.qword180);
	edited |= Editor("dword188", pipelineInfo.dword188);
	edited |= Editor("qword198", pipelineInfo.qword198);
	edited |= Editor("qword1A0", pipelineInfo.qword1A0);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::RenderingPipeline& pipeline)
{
	bool edited{};
	ImGui::PushID(label);
	Viewer("name", pipeline.name);
	edited |= Editor("unk2", pipeline.unk2);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::WorldRenderingPipeline& pipeline)
{
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("RenderingPipeline", static_cast<hh::needle::RenderingPipeline&>(pipeline));
	edited |= Editor("unk101", pipeline.unk101);
	edited |= Editor("unk102", pipeline.unk102);
	edited |= Editor("unk103", pipeline.unk103);
	edited |= Editor("unk104", pipeline.unk104);
	edited |= Editor("unk105", pipeline.unk105);
	edited |= Editor("unk106", pipeline.unk106);
	edited |= Editor("unk107", pipeline.unk107);
	edited |= Editor("unk108", pipeline.unk108);
	edited |= Editor("unk109", pipeline.unk109);
	edited |= Editor("unk110", pipeline.unk110);
	edited |= Editor("unk111", pipeline.unk111);
	edited |= Editor("unk114", pipeline.unk114);
	edited |= Editor("unk115", pipeline.unk115);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::RenderingPipelineRangers& pipeline)
{
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("WorldRenderingPipeline", static_cast<hh::needle::WorldRenderingPipeline&>(pipeline));
	Viewer("name", pipeline.name);
	edited |= Editor("qword118", pipeline.qword118);
	edited |= Editor("qword120", pipeline.qword120);
	edited |= Editor("qword128", pipeline.qword128);
	edited |= Editor("qword130", pipeline.qword130);
	edited |= Editor("qword138", pipeline.qword138);
	edited |= Editor("qword140", pipeline.qword140);
	edited |= Editor("qword148", pipeline.qword148);
	edited |= Editor("qword150", pipeline.qword150);
	edited |= Editor("qword178", pipeline.qword178);
	edited |= Editor("qword180", pipeline.qword180);
	edited |= Editor("qword188", pipeline.qword188);
	edited |= Editor("qword190", pipeline.qword190);
	edited |= Editor("qword198", pipeline.qword198);
	edited |= Editor("qword1A0", pipeline.qword1A0);
	edited |= Editor("qword1A8", pipeline.qword1A8);
	edited |= Editor("qword1B0", pipeline.qword1B0);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::RenderUnit::Unk1& renderUnitUnk1) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("unk1", renderUnitUnk1.unk1);
	edited |= Editor("unk2", renderUnitUnk1.unk2);
	Viewer("unk2", renderUnitUnk1.viewMatrices);
	Viewer("unk3", renderUnitUnk1.projectionMatrices);
	edited |= Editor("unk4", renderUnitUnk1.unk4);
	edited |= Editor("unk5", renderUnitUnk1.unk5);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::RenderUnit& renderUnit)
{
	bool edited{};
	ImGui::PushID(label);
	Viewer("Name", renderUnit.nameId->name);
	Viewer("Scene name", renderUnit.sceneNameId->name);
	edited |= Editor("Pipeline", *renderUnit.pipeline);
	edited |= Editor("pipelineInfo", *renderUnit.pipelineInfo, *renderUnit.pipeline);
	edited |= Editor("unk1", renderUnit.unk1);
	edited |= Editor("unk2", renderUnit.unk2);
	edited |= Editor("unk6", renderUnit.unk6);
	edited |= Editor("unk7", renderUnit.unk7);
	edited |= Editor("unk8", renderUnit.unk8);
	edited |= Editor("unk8b", renderUnit.unk8b);
	edited |= Editor("unk9", renderUnit.unk9);
	edited |= Editor("unk10", renderUnit.unk10);
	edited |= Editor("unk10b", renderUnit.unk10b);
	edited |= Editor("unk11", renderUnit.unk11);
	edited |= Editor("unk12", renderUnit.unk12);
	edited |= Editor("paramsInitialized", renderUnit.paramsInitialized);
	edited |= Editor("createSceneParamListeners", renderUnit.createSceneParamListeners);
	edited |= Editor("unk14", renderUnit.unk14);
	edited |= Editor("cameraId", renderUnit.cameraId);
	edited |= Editor("unk17", renderUnit.unk17);
	edited |= Editor("unk18", renderUnit.unk18);
	edited |= Editor("flags", renderUnit.flags);
	edited |= Editor("execContext.pipelineinfo", renderUnit.renderingPipelineExecContext->pipelineInfo, *renderUnit.pipeline);
	//pipelineInfo
	//renderingPipelineExecContext
	//sceneParamContainer
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::needle::RenderTextureHandle& renderTexture)
{
	bool edited{};
	ImGui::PushID(label);
	Editor("RenderUnit", static_cast<hh::needle::RenderUnit&>(renderTexture));
	Viewer("Camera", *renderTexture.camera);
	ImGui::PopID();
	return edited;
}

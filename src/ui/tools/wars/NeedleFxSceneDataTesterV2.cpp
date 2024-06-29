#include "NeedleFxSceneDataTesterV2.h"

using namespace hh::fnd;

NeedleFxSceneDataTesterV2::NeedleFxSceneDataTesterV2(csl::fnd::IAllocator* allocator) : StandaloneWindow(allocator)
{
	SetTitle("NeedleFxSceneData testing tool V2");
}

void NeedleFxSceneDataTesterV2::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(600, 350), ImGuiCond_Always);
}

constexpr size_t rootJobId = 0x31744634ull;
constexpr size_t timeJobId = 0x56235675ull;
constexpr size_t weatherJobId = 0x76894421ull;

void NeedleFxSceneDataTesterV2::RenderContents()
{
	auto* fxParamManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxParamManager>();

	if (!fxParamManager) {
		ImGui::Text("FxParamManager service not active!");
		return;
	}

	if (ImGui::BeginChild("DropRegion", ImVec2(600, 250))) {
		if (resource == nullptr) {
			ImGui::Text("Drag a NeedleFxSceneData RFL resource into this window to test it out.");
			ImGui::Text("Do not drag non-NeedleFxSceneData RFL resources into this window!");
		}
		else {
			ImGui::Text("Linked to resource \"%s\" (ResReflection @ 0x%zx)", resource->GetName(), resource);

			if (ImGui::Checkbox("Set NeedleFX Parameter", &setParam)) {
				if (setParam) {
					auto& param = resource->GetData()->items[itemId];

					fxParamManager->paramInterpolators.bloomInterpolator->AddJob(rootJobId, &param.bloom, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.dofInterpolator->AddJob(rootJobId, &param.dof, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.toneMapTypeInterpolator->AddJob(rootJobId, &param.tonemapType, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.exposureModeInterpolator->AddJob(rootJobId, &param.exposureMode, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.colorContrastInterpolator->AddJob(rootJobId, &param.colorContrast, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.toneMapInterpolator->AddJob(rootJobId, &param.tonemap, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.exposureInterpolator->AddJob(rootJobId, &param.exposure, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.shadowMapInterpolator->AddJob(rootJobId, &param.shadowmap, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.blurInterpolator->AddJob(rootJobId, &param.blur, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.ssaoInterpolator->AddJob(rootJobId, &param.ssao, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.lightFieldInterpolator->AddJob(rootJobId, &param.lightfield, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.shLightFieldInterpolator->AddJob(rootJobId, &param.shlightfield, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.lightScatteringInterpolator->AddJob(rootJobId, &param.lightscattering, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.rlrInterpolator->AddJob(rootJobId, &param.rlr, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.occlusionCapsuleInterpolator->AddJob(rootJobId, &param.occlusionCapsule, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.godrayInterpolator->AddJob(rootJobId, &param.godray, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.ssGodrayInterpolator->AddJob(rootJobId, &param.ssGodray, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.heatHazeInterpolator->AddJob(rootJobId, &param.heatHaze, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.sceneEnvironmentInterpolator->AddJob(rootJobId, &param.sceneEnv, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.renderOptionInterpolator->AddJob(rootJobId, &param.renderOption, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.sggiInterpolator->AddJob(rootJobId, &param.sggi, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.taaInterpolator->AddJob(rootJobId, &param.taa, 0xFFFFFFFF, 20000, 0.0f, -1);
					fxParamManager->paramInterpolators.effectInterpolator->AddJob(rootJobId, &param.effect, 0xFFFFFFFF, 20000, 0.0f, -1);
				}
				else {
					fxParamManager->paramInterpolators.bloomInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.dofInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.toneMapTypeInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.exposureModeInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.colorContrastInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.toneMapInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.exposureInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.shadowMapInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.blurInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.ssaoInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.lightFieldInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.shLightFieldInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.lightScatteringInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.rlrInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.occlusionCapsuleInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.godrayInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.ssGodrayInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.heatHazeInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.sceneEnvironmentInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.renderOptionInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.sggiInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.taaInterpolator->ReverseJob(rootJobId, 0.0f);
					fxParamManager->paramInterpolators.effectInterpolator->ReverseJob(rootJobId, 0.0f);
				}
			}

			if (setParam) {
				auto& param = resource->GetData()->items[itemId];

				ImGui::Indent();
				ImGui::SliderInt("Item ID", &itemId, 0, 15);
				fxParamManager->paramInterpolators.bloomInterpolator->interpolationJobs.begin()->parameters = param.bloom;
				fxParamManager->paramInterpolators.dofInterpolator->interpolationJobs.begin()->parameters = param.dof;
				fxParamManager->paramInterpolators.toneMapTypeInterpolator->interpolationJobs.begin()->parameters = param.tonemapType;
				fxParamManager->paramInterpolators.exposureModeInterpolator->interpolationJobs.begin()->parameters = param.exposureMode;
				fxParamManager->paramInterpolators.colorContrastInterpolator->interpolationJobs.begin()->parameters = param.colorContrast;
				fxParamManager->paramInterpolators.toneMapInterpolator->interpolationJobs.begin()->parameters = param.tonemap;
				fxParamManager->paramInterpolators.exposureInterpolator->interpolationJobs.begin()->parameters = param.exposure;
				fxParamManager->paramInterpolators.shadowMapInterpolator->interpolationJobs.begin()->parameters = param.shadowmap;
				fxParamManager->paramInterpolators.blurInterpolator->interpolationJobs.begin()->parameters = param.blur;
				fxParamManager->paramInterpolators.ssaoInterpolator->interpolationJobs.begin()->parameters = param.ssao;
				fxParamManager->paramInterpolators.lightFieldInterpolator->interpolationJobs.begin()->parameters = param.lightfield;
				fxParamManager->paramInterpolators.shLightFieldInterpolator->interpolationJobs.begin()->parameters = param.shlightfield;
				fxParamManager->paramInterpolators.lightScatteringInterpolator->interpolationJobs.begin()->parameters = param.lightscattering;
				fxParamManager->paramInterpolators.rlrInterpolator->interpolationJobs.begin()->parameters = param.rlr;
				fxParamManager->paramInterpolators.occlusionCapsuleInterpolator->interpolationJobs.begin()->parameters = param.occlusionCapsule;
				fxParamManager->paramInterpolators.godrayInterpolator->interpolationJobs.begin()->parameters = param.godray;
				fxParamManager->paramInterpolators.ssGodrayInterpolator->interpolationJobs.begin()->parameters = param.ssGodray;
				fxParamManager->paramInterpolators.heatHazeInterpolator->interpolationJobs.begin()->parameters = param.heatHaze;
				fxParamManager->paramInterpolators.sceneEnvironmentInterpolator->interpolationJobs.begin()->parameters = param.sceneEnv;
				fxParamManager->paramInterpolators.renderOptionInterpolator->interpolationJobs.begin()->parameters = param.renderOption;
				fxParamManager->paramInterpolators.sggiInterpolator->interpolationJobs.begin()->parameters = param.sggi;
				fxParamManager->paramInterpolators.taaInterpolator->interpolationJobs.begin()->parameters = param.taa;
				fxParamManager->paramInterpolators.effectInterpolator->interpolationJobs.begin()->parameters = param.effect;

				fxParamManager->paramInterpolators.bloomInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.dofInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.toneMapTypeInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.exposureModeInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.colorContrastInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.toneMapInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.exposureInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.shadowMapInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.blurInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.ssaoInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.lightFieldInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.shLightFieldInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.lightScatteringInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.rlrInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.occlusionCapsuleInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.godrayInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.ssGodrayInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.heatHazeInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.sceneEnvironmentInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.renderOptionInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.sggiInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.taaInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				fxParamManager->paramInterpolators.effectInterpolator->interpolationGroupEnabledBits = 0xFFFFFFFF;
				ImGui::Unindent();
			}
		}
	}
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResReflection::GetTypeInfo())
				resource = static_cast<ResReflectionT<hh::NeedleFxSceneData>*>(res);
		}
		ImGui::EndDragDropTarget();
	}
}

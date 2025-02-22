#include "SceneSettings.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>
#include <dieventlib/dvscene.h>

namespace ui::operation_modes::modes::dvscene_editor {
	constexpr std::pair<size_t, size_t> NodeDataInfo[] = {
		{},
		{80, offsetof(hh::dv::DvNodePath, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeCamera::Data), offsetof(hh::dv::DvNodeCamera, binaryData)},
		{sizeof(hh::dv::DvNodeCameraMotion::Data), offsetof(hh::dv::DvNodeCameraMotion, binaryData)},
		{sizeof(hh::dv::DvNodeCharacter::Data), offsetof(hh::dv::DvNodeCharacter, binaryData)},
		{sizeof(hh::dv::DvNodeCharacterMotion::Data), offsetof(hh::dv::DvNodeCharacterMotion, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeModel::Data), offsetof(hh::dv::DvNodeModel, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeModelMotion::Data), offsetof(hh::dv::DvNodeModelMotion, binaryData)},
		{sizeof(hh::dv::DvNodeModelNode::Data), offsetof(hh::dv::DvNodeModelNode, binaryData)},
		{}
	};
	constexpr std::pair<size_t, size_t> ElementDataInfo[] = {
		{},
		{sizeof(hh::dv::DvElementCameraParams::Data), offsetof(hh::dv::DvElementCameraParams, binaryData)},
		{},
		{sizeof(hh::dv::DvElementDrawOff::Data), offsetof(hh::dv::DvElementDrawOff, binaryData)},
		{},
		{sizeof(hh::dv::DvElementPathOffset::Data), offsetof(hh::dv::DvElementPathOffset, binaryData)},
		{sizeof(hh::dv::DvElementCameraShake::Data), offsetof(hh::dv::DvElementCameraShake, binaryData)},
		{sizeof(hh::dv::DvElementCameraShakeLoop::Data), offsetof(hh::dv::DvElementCameraShakeLoop, binaryData)},
		{/*sizeof(hh::dv::DvElementEffect::Data)*/676, offsetof(hh::dv::DvElementEffect, binaryData)}, //due to aligning im hardcoding the size for now
		{sizeof(hh::dv::DvElementDirectionalLight::Data), offsetof(hh::dv::DvElementDirectionalLight, binaryData)},
		{sizeof(hh::dv::DvElementPathInterpolation::Data), offsetof(hh::dv::DvElementPathInterpolation, binaryData)},
		{0, 0},
		{sizeof(hh::dv::DvElementCameraNearFar::Data), offsetof(hh::dv::DvElementCameraNearFar, binaryData)},
		{sizeof(hh::dv::DvElementUVAnim::Data), offsetof(hh::dv::DvElementUVAnim, binaryData)},
		{sizeof(hh::dv::DvElementVisibilityAnim::Data), offsetof(hh::dv::DvElementVisibilityAnim, binaryData)},
		{sizeof(hh::dv::DvElementMaterialAnim::Data), offsetof(hh::dv::DvElementMaterialAnim, binaryData)},
		{sizeof(hh::dv::DvElementMultipleAnim::Data), offsetof(hh::dv::DvElementMultipleAnim, binaryData)},
		{sizeof(hh::dv::DvElementCameraOffset::Data), offsetof(hh::dv::DvElementCameraOffset, binaryData)},
		{sizeof(hh::dv::DvElementModelFade::Data), offsetof(hh::dv::DvElementModelFade, binaryData)},
		{sizeof(hh::dv::DvElementDebugMotion::Data), offsetof(hh::dv::DvElementDebugMotion, binaryData)},
		{sizeof(hh::dv::DvElementCameraHedgehog::Data), offsetof(hh::dv::DvElementCameraHedgehog, binaryData)},
		{sizeof(hh::dv::DvElementCameraInGame::Data), offsetof(hh::dv::DvElementCameraInGame, binaryData)},
		{sizeof(hh::dv::DvElementPointLight::Data), offsetof(hh::dv::DvElementPointLight, binaryData)},
		{sizeof(hh::dv::DvElementVertexAnimationTexture::Data), offsetof(hh::dv::DvElementVertexAnimationTexture, binaryData)},
		{sizeof(hh::dv::DvElementSpotlight::Data), offsetof(hh::dv::DvElementSpotlight, binaryData)},
		{sizeof(hh::dv::DvElementControllerVibration::Data), offsetof(hh::dv::DvElementControllerVibration, binaryData)},
#ifdef DEVTOOLS_TARGET_SDK_rangers
		{sizeof(hh::dv::DvElementSpotlightModel::Data), offsetof(hh::dv::DvElementSpotlightModel, binaryData)}
#elif DEVTOOLS_TARGET_SDK_miller
		{sizeof(hh::dv::DvElementTexturePatternAnim::Data), offsetof(hh::dv::DvElementTexturePatternAnim, binaryData)},
		{sizeof(hh::dv::DvElementMaterialParam::Data), offsetof(hh::dv::DvElementMaterialParam, binaryData)},
#endif
	};

	static dv::Resource* ConvertGameResourceToFileResource(hh::dv::DvResource* resource) {
		dv::Resource* dvResource = new dv::Resource;
		memcpy(&dvResource->guid, resource->guid, 16);
		dvResource->type = static_cast<dv::Resource::Type>(resource->type);
		dvResource->flags0 = 0;
		dvResource->flags1 = 1;
		memset(&dvResource->filename, 0, 192);
		strcpy(dvResource->filename, resource->resource->GetName());
		memset(&dvResource->unkData, 0, 596);
		return dvResource;
	}

	static void GetResources(std::vector<hh::dv::DvResource*>& resources, hh::dv::DvNodeBase* node) {
		if (auto* x = node->dvResource)
			resources.push_back(x);
		for (auto* x : node->childrenElements0)
			GetResources(resources, x);
		for (auto* x : node->childrenElements1)
			GetResources(resources, x);
		for (auto* x : node->childrenElements2)
			GetResources(resources, x);
		for (auto* x : node->childrenPath)
			GetResources(resources, x);
		for (auto* x : node->childrenCamera)
			GetResources(resources, x);
		for (auto* x : node->childrenMotion)
			GetResources(resources, x);
	}

	static void GetDrawNodeNum(int& num, hh::dv::DvNodeBase* node) {
		if (node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER || node->nodeType == hh::dv::DvNodeBase::NodeType::MODEL)
			num++;
		for (auto* x : node->childrenElements0)
			GetDrawNodeNum(num, x);
		for (auto* x : node->childrenElements1)
			GetDrawNodeNum(num, x);
		for (auto* x : node->childrenElements2)
			GetDrawNodeNum(num, x);
		for (auto* x : node->childrenPath)
			GetDrawNodeNum(num, x);
		for (auto* x : node->childrenCamera)
			GetDrawNodeNum(num, x);
		for (auto* x : node->childrenMotion)
			GetDrawNodeNum(num, x);
	}

	static dv::DvNode* ConvertGameNodeToFileNode(hh::dv::DvNodeBase* node) {
		dv::DvNode* dvNode = new dv::DvNode;
		memset(&dvNode->name, 0, 64);
		strcpy(dvNode->name, node->nodeName.c_str());
		memcpy(reinterpret_cast<char*>(&dvNode->guid), node->guid, 16);
		dvNode->category = (unsigned int)node->nodeType;
		dvNode->nodeFlags = 0;
		dvNode->priority = 0;
		if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT) {
			auto* element = reinterpret_cast<hh::dv::DvNodeElement*>(node);
			auto* nodeElementData = reinterpret_cast<char*>(&element->binaryData);
			char* elementData;
			size_t elementSize = 0;
			if (static_cast<unsigned int>(element->binaryData.elementId) >= 1000) {
				auto* appDvElement = reinterpret_cast<app::dv::AppDvElementBase*>(element->element);
				elementData = static_cast<char*>(appDvElement->elementBinaryData);
				elementSize = appDvElement->elementBinaryDataSize;
			}
			else {
				auto* dvElement = reinterpret_cast<hh::dv::DvElementBase*>(element->element);
				elementData = &reinterpret_cast<char*>(dvElement)[ElementDataInfo[static_cast<unsigned int>(element->binaryData.elementId)].second];
				elementSize = ElementDataInfo[static_cast<unsigned int>(element->binaryData.elementId)].first;
			}
			dvNode->data = new char[sizeof(hh::dv::DvNodeElement::Data) + elementSize];
			memcpy(dvNode->data, nodeElementData, sizeof(hh::dv::DvNodeElement::Data));
			memcpy(&dvNode->data[sizeof(hh::dv::DvNodeElement::Data)], elementData, elementSize);
			dvNode->dataSize = sizeof(hh::dv::DvNodeElement::Data) + elementSize;
		}
		else {
			dvNode->data = &reinterpret_cast<char*>(node)[NodeDataInfo[dvNode->category].second];
			dvNode->dataSize = NodeDataInfo[dvNode->category].first;
		}
		for (auto* x : node->childrenElements0)
			dvNode->childNodes.push_back(*ConvertGameNodeToFileNode(x));
		for (auto* x : node->childrenElements1)
			dvNode->childNodes.push_back(*ConvertGameNodeToFileNode(x));
		for (auto* x : node->childrenElements2)
			dvNode->childNodes.push_back(*ConvertGameNodeToFileNode(x));
		for (auto* x : node->childrenPath)
			dvNode->childNodes.push_back(*ConvertGameNodeToFileNode(x));
		for (auto* x : node->childrenCamera)
			dvNode->childNodes.push_back(*ConvertGameNodeToFileNode(x));
		for (auto* x : node->childrenMotion)
			dvNode->childNodes.push_back(*ConvertGameNodeToFileNode(x));
		return dvNode;
	}

	static dv::DvCondition* ConvertGameConditionToFileCondition(hh::dv::DvPageConditionBase* condition) {
		dv::DvCondition* dvCondition = new dv::DvCondition;
		if (dynamic_cast<hh::dv::DvPageConditionEnd*>(condition)) {
			dvCondition->type = static_cast<dv::DvCondition::ConditionType>(4);
			dvCondition->dataSize = 0;
			dvCondition->data = new char[0];
		}
		else if (auto* x = dynamic_cast<app::dv::DvPageConditionQTE*>(condition)) {
			dvCondition->type = static_cast<dv::DvCondition::ConditionType>(1000);
			dvCondition->dataSize = sizeof(app::dv::DvPageConditionQTE::Data);
			dvCondition->data = reinterpret_cast<char*>(&x->binaryData);
		}
		return dvCondition;
	}

	static dv::DvTransition* ConvertGameTransitionToFileTransition(hh::dv::DvPageTransition* transition) {
		dv::DvTransition* dvTransition = new dv::DvTransition;
		dvTransition->destinationPageID = transition->binaryData.destinationPageID;
		dvTransition->conditions.push_back(*ConvertGameConditionToFileCondition(transition->condition));
		return dvTransition;
	}

	static dv::DvPage* ConvertGamePageToFilePage(hh::dv::DvPage* page) {
		dv::DvPage* dvPage = new dv::DvPage;
		dvPage->frameStart = page->binaryData.start;
		dvPage->frameEnd = page->binaryData.end;
		dvPage->index = page->binaryData.pageIndex;
		dvPage->skipFrame = page->binaryData.skipFrame;
		memset(&dvPage->name, 0, 32);
		strcpy(dvPage->name, page->binaryData.pageName);
		for (auto* x : page->transitions)
			dvPage->transition.push_back(*ConvertGameTransitionToFileTransition(x));
		return dvPage;
	}

	static dv::DvScene* ConvertGameDvSceneToFileDvScene(hh::dv::DvSceneControl* dvsc) {
		dv::DvScene* dv = new dv::DvScene;
		auto* timeline = dvsc->timeline;
		dv->dvCommon = new dv::DvCommon;
		dv->dvCommon->frameStart = timeline->frameStart/100;
		dv->dvCommon->frameEnd = timeline->frameEnd/100;
		dv->dvCommon->drawNodeNumber = 0;
		GetDrawNodeNum(dv->dvCommon->drawNodeNumber, dvsc->nodeTree->mainNode);
		dv->dvCommon->chainCameraIn = -1;
		dv->dvCommon->chainCameraOut = -1;
		dv->dvCommon->type = 0;
		dv->dvCommon->skipPointTick = 0;
		dv->dvCommon->cuts.resize(timeline->cuts.size());
		memcpy(dv->dvCommon->cuts.data(), timeline->cuts.begin(), sizeof(float) * timeline->cuts.size());
		dv->dvCommon->resourceCutInfo.push_back(dv->dvCommon->frameEnd);
		for (auto* x : timeline->pages)
			dv->dvCommon->pages.push_back(*ConvertGamePageToFilePage(x));
		dv->dvCommon->node = ConvertGameNodeToFileNode(dvsc->nodeTree->mainNode);
		std::vector<hh::dv::DvResource*> resources{};
		GetResources(resources, dvsc->nodeTree->mainNode);
		for (auto* x : resources)
			dv->dvResource.push_back(*ConvertGameResourceToFileResource(x));
		return dv;
	}

	void SceneSettings::RenderPanel()
	{
		char sceneName[400];
		auto& context = GetContext();

		if (context.goDVSC != nullptr)
			snprintf(sceneName, sizeof(sceneName), "%s - %zx", context.goDVSC->cutsceneName.c_str(), reinterpret_cast<size_t>(&*context.goDVSC));
		else
			strcpy_s(sceneName, "none");

		if (ImGui::BeginCombo("DvSceneControl", sceneName)) {
			for (auto* obj : hh::game::GameManager::GetInstance()->objects)
				if (obj->objectClass == hh::dv::DvSceneControl::GetClass()) {
					auto* dvsc = reinterpret_cast<hh::dv::DvSceneControl*>(obj);
					snprintf(sceneName, sizeof(sceneName), "%s - %zx", dvsc->cutsceneName.c_str(), reinterpret_cast<size_t>(obj));
					if (ImGui::Selectable(sceneName, obj == context.goDVSC)) {
						GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
						context.goDVSC = dvsc;
					}
					if (context.goDVSC == obj)
						ImGui::SetItemDefaultFocus();
				}
			ImGui::EndCombo();
		}

		if (context.goDVSC == nullptr) {
            ImGui::Text("No DvSceneControl selected");
			return;
		}

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = context.goDVSC;
			ImGuiFileDialog::Instance()->OpenDialog("ResDvSceneExportDialog", "Choose File", ".dvscene", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResDvSceneExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* dvsc = static_cast<hh::dv::DvSceneControl*>(ImGuiFileDialog::Instance()->GetUserDatas());

				dv::DvScene* dv = ConvertGameDvSceneToFileDvScene(dvsc);
				dv::internal::Buffer buffer = dv->write();

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				ofs.write(buffer.data, buffer.size);
				ofs.close();
			}
			ImGuiFileDialog::Instance()->Close();
		}

		auto* timeline = context.goDVSC->timeline;
        ImGui::SeparatorText("DvCommon Settings");
		int start = static_cast<int>(timeline->frameStart / 100);
        if(Editor("Start", start));
			timeline->frameStart = static_cast<float>(start) * 100;
		int end = static_cast<int>(timeline->frameEnd / 100);
		if (Editor("End", end));
			timeline->frameEnd = static_cast<float>(end) * 100;
	}

	PanelTraits SceneSettings::GetPanelTraits() const
	{
		return { "Scene Settings", ImVec2(ImGui::GetMainViewport()->WorkSize.x-250, ImGui::GetMainViewport()->WorkSize.y - 200), ImVec2(250, 200) };
	}
}

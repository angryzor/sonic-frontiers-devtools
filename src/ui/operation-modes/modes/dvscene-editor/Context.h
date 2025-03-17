#pragma once
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/CompatibleObject.h>
#include <dieventlib/dvscene.h>

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
	inline constexpr const char* nodeTypeNames[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};

	inline constexpr const char* elementIDStrings[] = {
		"",
		"CameraParams",
		"",
		"DrawOff",
		"",
		"PathOffset",
		"CameraShake",
		"CameraShakeLoop",
		"Effect",
		"DirectionalLight",
		"PathInterpolation",
		"CullingDisable",
		"CameraNearFar",
		"UVAnim",
		"VisibilityAnim",
		"MaterialAnim",
		"MultipleAnim",
		"CameraOffset",
		"ModelFade",
		"DebugMotion",
		"CameraHedgehog",
		"CameraInGame",
		"PointLight",
		"VertexAnimationTexture",
		"Spotlight",
		"ControllerVibration",
		"SpotlightModel",

		"BloomParam",
		"DOFParam",
		"ColorContrast",
		"CameraControlParam",
		"ShadowResolution",
		"GodrayParam",
		"AtmosphereGodrayParam",
		"AtmosphereHeightFogParam",
		"ChromaticAberrationFilterParam",
		"VignetteParam",
		"Fade",
		"LetterBox",
		"ModelClipping",
		"PbaReset",
		"BossName",
		"Caption",
		"Sound",
		"Time",
		"Sun",
		"LookAtIK",
		"CameraBlurParam",
		"GeneralTrigger",
		"FootIk",
		"DitherParam",
		"QTE",
		"FacialAnimation",
		"OverrideASM",
		"Aura",
		"ChangeTimeScale",
		"CyberSpaceNoise",
		"LipAnimation",
		"AuraRoad",
		"MovieView",
		"CrossFade",
		"Weather",
		"ShadowMapParam",
		"VariablePointLight",
		"OpeningLogo",
		"DensitySectorPoint",
		"FxColUpdate",
		"VAT",
		"QTEAccel",
		"TheEndCableObject",
		"RifleBeastLighting"
	};

	constexpr unsigned int hhElementCount = 27;
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
	inline constexpr const char* nodeTypeNames[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};

	inline constexpr const char* elementIDStrings[] = {
		"",
		"CameraParams",
		"",
		"DrawOff",
		"",
		"PathOffset",
		"CameraShake",
		"CameraShakeLoop",
		"Effect",
		"DirectionalLight",
		"PathInterpolation",
		"CullingDisable",
		"CameraNearFar",
		"UVAnim",
		"VisibilityAnim",
		"MaterialAnim",
		"MultipleAnim",
		"CameraOffset",
		"ModelFade",
		"DebugMotion",
		"CameraHedgehog",
		"CameraInGame",
		"PointLight",
		"VertexAnimationTexture",
		"Spotlight",
		"ControllerVibration",
		"TexturePatternAnim",
		"MaterialParam",

		"BloomParam",
		"DOFParam",
		"ColorContrast",
		"CameraControlParam",
		"ShadowResolution",
		"SSAOParam",
		"OcclusionCapsuleParam",
		"GodrayParam",
		"AtmosphereGodrayParam",
		"AtmosphereHeightFogParam",
		"ChromaticAberrationFilterParam",
		"VignetteParam",
		"Fade",
		"LetterBox",
		"ModelClipping",
		"PbaReset",
		"BossName",
		"Caption",
		"Sound",
		"Time",
		"Sun",
		"LookAtIK",
		"CameraBlurParam",
		"GeneralTrigger",
		"FootIk",
		"DitherParam",
		"QTE",
		"FacialAnimation",
		"OverrideASM",
		"Aura",
		"ChangeTimeScale",
		"CyberSpaceNoise",
		"LipAnimation",
		"AuraRoad",
		"MovieView",
		"CrossFade",
		"Weather",
		"ShadowMapParam",
		"VariablePointLight",
		"OpeningLogo",
		"DensitySectorPoint",
		"BulletTime",
		"TimeStop",
		"ObjectTimeStop",
		"ShadowShape",
		"Falloff",
		"Fog",
		"DOF",
		"FxColUpdate"
	};

	constexpr unsigned int hhElementCount = 28;
#endif

	struct DvPage;

	class Context : public CompatibleObject {
		void ContainsElement(bool& contains, const unsigned int& elementId, hh::dv::DvNodeBase* node);
		void GetFileNode(dv::DvNode& curNode, hh::dv::DvNodeBase* node, dv::DvNode*& result);
		void GetFilePage(dv::DvPage& curPage, hh::dv::DvPage* page, dv::DvPage*& result);
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::dv::DvSceneControl> goDVSC{};
		dv::DvScene* parsedScene;
		app::evt::EventScene* evtScene;
		std::mt19937 mt{ std::random_device{}() };
		csl::ut::MoveArray32<DvPage*> dvPages;

		csl::ut::VariableString cutsceneName;
		csl::ut::VariableString nodeName;
		int nodeType;
		int elementId;

		bool ContainsElement(const unsigned int& elementId);
		static void GetChildren(hh::dv::DvNodeBase* node, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool& includeElements);
		static void GetNodes(hh::dv::DvSceneNodeTree* nodeTree, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool includeElements);
		hh::dv::DvNodeBase* CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId, hh::dv::DvNodeBase* parent);
		dv::DvNode CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId);
		void ParentNode(hh::dv::DvNodeBase* parent, hh::dv::DvNodeBase* child);
		void ParentNode(dv::DvNode& parent, dv::DvNode& child);
		dv::DvNode* GetFileNode(hh::dv::DvNodeBase* node);
		dv::DvPage* GetFilePage(hh::dv::DvPage* page);
		hh::dv::DvNodeBase* GetRuntimeNode(dv::DvNode* node);
		void GenerateGUID(char* guid);

		Context(csl::fnd::IAllocator* allocator);
	};

	struct DvPage {
	public:
		Context* ctx;
		hh::dv::DvPage* page;
		dv::DvPage* filePage;

		DvPage();
		DvPage(hh::dv::DvPage* page, Context& ctx) : page{ page }, ctx{ &ctx }
		{
			filePage = ctx.GetFilePage(page);
		}

		void UpdateRuntimePage() {
			page->binaryData.start = filePage->frameStart;
			page->binaryData.end = filePage->frameEnd;
			page->binaryData.skipFrame = filePage->skipFrame;
			page->binaryData.pageIndex = filePage->index;
			memcpy(page->binaryData.pageName, filePage->name, 32);
			page->binaryData.transitionCount = filePage->transition.size();
			page->binaryData.dataSize = filePage->unkData.size();
			for (int trans = 0; trans < filePage->transition.size(); trans++) {
				auto* transition = page->transitions[trans];
				auto& fileTransition = filePage->transition[trans];
				transition->binaryData.destinationPageID = fileTransition.destinationPageID;
				transition->binaryData.conditionCount = fileTransition.conditions.size();
				auto& fileCondition = fileTransition.conditions[0];
				if (fileCondition.type == dv::DvCondition::ConditionType::QTE) {
					auto* qteCond = reinterpret_cast<app::dv::DvPageConditionQTE*>(transition->condition);
					memcpy(&qteCond->binaryData, fileCondition.data, fileCondition.dataSize);
				}
			}
		}
	};
}

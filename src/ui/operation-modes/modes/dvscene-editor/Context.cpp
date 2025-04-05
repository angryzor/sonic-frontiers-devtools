#include "Context.h"
#include "timeline/Elements.h"

using namespace hh::dv;
using namespace app::dv;

namespace ui::operation_modes::modes::dvscene_editor {
    constexpr std::pair<size_t, size_t> NodeDataSize[] = {
        {},
        {sizeof(DvNodePath), sizeof(DvNodePath::Description)},
        {},
        {sizeof(DvNodeCamera), sizeof(DvNodeCamera::Description)},
        {sizeof(DvNodeCameraMotion), sizeof(DvNodeCameraMotion::Description)},
        {sizeof(DvNodeCharacter), sizeof(DvNodeCharacter::Description)},
        {sizeof(DvNodeCharacterMotion), sizeof(DvNodeCharacterMotion::Description)},
        {},
        {sizeof(DvNodeModel), sizeof(DvNodeModel::Description)},
        {},
        {sizeof(DvNodeModelMotion), sizeof(DvNodeModelMotion::Description)},
        {sizeof(DvNodeModelNode), sizeof(DvNodeModelNode::Description)},
        {sizeof(DvNodeElement), sizeof(DvNodeElement::Description<DvElementBase::Description>)}
    };

#ifdef DEVTOOLS_TARGET_SDK_rangers
    constexpr size_t ElementDataSize[] = {
        0, // 0
        sizeof(DvElementCameraParams::Description), // CAMERA_PARAMS
        0, // 2 (Empty)
        sizeof(DvElementDrawOff::Description), // DRAW_OFF
        0, // 4 (Empty)
        sizeof(DvElementPathOffset::Description), // PATH_OFFSET
        sizeof(DvElementCameraShake::Description), // CAMERA_SHAKE
        sizeof(DvElementCameraShakeLoop::Description), // CAMERA_SHAKE_LOOP
        /*sizeof(DvElementEffect::Description)*/676, // EFFECT
        sizeof(DvElementDirectionalLight::Description), // DIRECTIONAL_LIGHT
        sizeof(DvElementPathInterpolation::Description), // PATH_INTERPOLATION
        0, // CULLING_DISABLE
        sizeof(DvElementCameraNearFar::Description), // CAMERA_NEAR_FAR
        sizeof(DvElementUVAnim::Description), // UV_ANIM
        sizeof(DvElementVisibilityAnim::Description), // VISIBILITY_ANIM
        sizeof(DvElementMaterialAnim::Description), // MATERIAL_ANIM
        sizeof(DvElementMultipleAnim::Description), // MULTIPLE_ANIM
        sizeof(DvElementCameraOffset::Description), // CAMERA_OFFSET
        sizeof(DvElementModelFade::Description), // MODEL_FADE
        sizeof(DvElementDebugMotion::Description), // DEBUG_MOTION
        sizeof(DvElementCameraHedgehog::Description), // CAMERA_HEDGEHOG
        sizeof(DvElementCameraInGame::Description), // CAMERA_IN_GAME
        sizeof(DvElementPointLight::Description), // POINT_LIGHT
        sizeof(DvElementVertexAnimationTexture::Description), // VERTEX_ANIMATION_TEXTURE
        sizeof(DvElementSpotlight::Description), // SPOTLIGHT
        sizeof(DvElementControllerVibration::Description), // CONTROLLER_VIBRATION
        sizeof(DvElementSpotlightModel::Description), // SPOTLIGHT_MODEL

        sizeof(app::dv::DvElementBloomParam::Description), // BLOOM_PARAM
        sizeof(app::dv::DvElementDOFParam::Description), // DOF_PARAM
        sizeof(app::dv::DvElementColorContrast::Description), // COLOR_CONTRAST
        sizeof(app::dv::DvElementCameraControlParam::Description), // CAMERA_CONTROL_PARAM
        sizeof(app::dv::DvElementShadowResolution::Description), // SHADOW_RESOLUTION
        sizeof(app::dv::DvElementGodrayParam::Description), // GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereGodrayParam::Description), // ATMOSPHERE_GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereHeightFogParam::Description), // ATMOSPHERE_HEIGHT_FOG_PARAM
        sizeof(app::dv::DvElementChromaticAberrationFilterParam::Description), // CHROMATIC_ABERRATION_FILTER_PARAM
        sizeof(app::dv::DvElementVignetteParam::Description), // VIGNETTE_PARAM
        sizeof(app::dv::DvElementFade::Description), // FADE
        sizeof(app::dv::DvElementLetterBox::Description), // LETTER_BOX
        sizeof(app::dv::DvElementModelClipping::Description), // MODEL_CLIPPING
        sizeof(app::dv::DvElementPbaReset::Description), // PBA_RESET
        sizeof(app::dv::DvElementBossName::Description), // BOSS_NAME
        sizeof(app::dv::DvElementCaption::Description), // CAPTION
        sizeof(app::dv::DvElementSound::Description), // SOUND
        sizeof(app::dv::DvElementTime::Description), // TIME
        sizeof(app::dv::DvElementSun::Description), // SUN
        sizeof(app::dv::DvElementLookAtIK::Description), // LOOK_AT_IK
        sizeof(app::dv::DvElementCameraBlurParam::Description), // CAMERA_BLUR_PARAM
        sizeof(app::dv::DvElementGeneralTrigger::Description), // GENERAL_TRIGGER
        sizeof(app::dv::DvElementFootIk::Description), // FOOT_IK
        sizeof(app::dv::DvElementDitherParam::Description), // DITHER_PARAM
        sizeof(app::dv::DvElementQTE::Description), // QTE
        sizeof(app::dv::DvElementFacialAnimation::Description), // FACIAL_ANIMATION
        sizeof(app::dv::DvElementOverrideASM::Description), // OVERRIDE_ASM
        sizeof(app::dv::DvElementAura::Description), // AURA
        sizeof(app::dv::DvElementChangeTimeScale::Description), // CHANGE_TIME_SCALE
        sizeof(app::dv::DvElementCyberSpaceNoise::Description), // CYBER_SPACE_NOISE
        sizeof(app::dv::DvElementLipAnimation::Description), // LIP_ANIMATION
        sizeof(app::dv::DvElementAuraRoad::Description), // AURA_ROAD
        0, // MOVIE_VIEW
        sizeof(app::dv::DvElementCrossFade::Description), // CROSS_FADE
        sizeof(app::dv::DvElementWeather::Description), // WEATHER
        sizeof(app::dv::DvElementShadowMapParam::Description), // SHADOW_MAP_PARAM
        sizeof(app::dv::DvElementVariablePointLight::Description), // VARIABLE_POINT_LIGHT
        0, // OPENING_LOGO
        sizeof(app::dv::DvElementDensitySectorPoint::Description), // DENSITY_SECTOR_POINT
        0, // FX_COL_UPDATE
        sizeof(app::dv::DvElementQTEAccel::Description), // QTE_ACCEL
        sizeof(app::dv::DvElementTheEndCableObject::Description), // THE_END_CABLE_OBJECT
        sizeof(app::dv::DvElementRifleBeastLighting::Description), // RIFLE_BEAST_LIGHTING
    };
#elif DEVTOOLS_TARGET_SDK_miller
    constexpr size_t ElementDescriptionSize[] = {
        0, // 0 (Unused)
        sizeof(DvElementCameraParams::Description), // CAMERA_PARAMS
        0, // 2 (Empty)
        sizeof(DvElementDrawOff::Description), // DRAW_OFF
        0, // 4 (Empty)
        sizeof(DvElementPathOffset::Description), // PATH_OFFSET
        sizeof(DvElementCameraShake::Description), // CAMERA_SHAKE
        sizeof(DvElementCameraShakeLoop::Description), // CAMERA_SHAKE_LOOP
        sizeof(DvElementEffect::Description), // EFFECT
        sizeof(DvElementDirectionalLight::Description), // DIRECTIONAL_LIGHT
        sizeof(DvElementPathInterpolation::Description), // PATH_INTERPOLATION
        0, // CULLING_DISABLE
        sizeof(DvElementCameraNearFar::Description), // CAMERA_NEAR_FAR
        sizeof(DvElementUVAnim::Description), // UV_ANIM
        sizeof(DvElementVisibilityAnim::Description), // VISIBILITY_ANIM
        sizeof(DvElementMaterialAnim::Description), // MATERIAL_ANIM
        sizeof(DvElementMultipleAnim::Description), // MULTIPLE_ANIM
        sizeof(DvElementCameraOffset::Description), // CAMERA_OFFSET
        sizeof(DvElementModelFade::Description), // MODEL_FADE
        sizeof(DvElementDebugMotion::Description), // DEBUG_MOTION
        sizeof(DvElementCameraHedgehog::Description), // CAMERA_HEDGEHOG
        sizeof(DvElementCameraInGame::Description), // CAMERA_IN_GAME
        sizeof(DvElementPointLight::Description), // POINT_LIGHT
        sizeof(DvElementVertexAnimationTexture::Description), // VERTEX_ANIMATION_TEXTURE
        sizeof(DvElementSpotlight::Description), // SPOTLIGHT
        sizeof(DvElementControllerVibration::Description), // CONTROLLER_VIBRATION
        sizeof(DvElementTexturePatternAnim::Description), // TEXTURE_PATTERN_ANIM
        sizeof(DvElementMaterialParam::Description), // MATERIAL_PARAM

        sizeof(app::dv::DvElementBloomParam::Description), // BLOOM_PARAM
        sizeof(app::dv::DvElementDOFParam::Description), // DOF_PARAM
        sizeof(app::dv::DvElementColorContrast::Description), // COLOR_CONTRAST
        sizeof(app::dv::DvElementCameraControlParam::Description), // CAMERA_CONTROL
        sizeof(app::dv::DvElementShadowResolution::Description), // SHADOW_RESOLUTION
        sizeof(app::dv::DvElementSSAOParam::Description), // SSAO_PARAM
        sizeof(app::dv::DvElementOcclusionCapsuleParam::Description), // OCCLUSION_CAPSULE_PARAM
        sizeof(app::dv::DvElementGodrayParam::Description), // GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereGodrayParam::Description), // ATMOSPHERE_GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereHeightFogParam::Description), // ATMOSPHERE_HEIGHT_FOG_PARAM
        sizeof(app::dv::DvElementChromaticAberrationFilterParam::Description), // CHROMATIC_ABERRATION_FILTER_PARAM
        sizeof(app::dv::DvElementVignetteParam::Description), // VIGNETTE_PARAM
        sizeof(app::dv::DvElementFade::Description), // FADE
        sizeof(app::dv::DvElementLetterBox::Description), // LETTER_BOX
        sizeof(app::dv::DvElementModelClipping::Description), // MODEL_CLIPPING
        sizeof(app::dv::DvElementPbaReset::Description), // PBA_RESET
        sizeof(app::dv::DvElementBossName::Description), // BOSS_NAME
        sizeof(app::dv::DvElementCaption::Description), // CAPTION
        sizeof(app::dv::DvElementSound::Description), // SOUND
        sizeof(app::dv::DvElementTime::Description), // TIME
        sizeof(app::dv::DvElementSun::Description), // SUN
        sizeof(app::dv::DvElementLookAtIK::Description), // LOOK_AT_IK
        sizeof(app::dv::DvElementCameraBlurParam::Description), // CAMERA_BLUR_PARAM
        sizeof(app::dv::DvElementGeneralTrigger::Description), // GENERAL_TRIGGER
        sizeof(app::dv::DvElementFootIk::Description), // FOOT_IK
        sizeof(app::dv::DvElementDitherParam::Description), // DITHER_PARAM
        sizeof(app::dv::DvElementQTE::Description), // QTE
        sizeof(app::dv::DvElementFacialAnimation::Description), // FACIAL_ANIMATION
        sizeof(app::dv::DvElementOverrideASM::Description), // OVERRIDE_ASM
        sizeof(app::dv::DvElementAura::Description), // AURA
        sizeof(app::dv::DvElementChangeTimeScale::Description), // CHANGE_TIME_SCALE
        sizeof(app::dv::DvElementCyberSpaceNoise::Description), // CYBER_SPACE_NOISE
        sizeof(app::dv::DvElementLipAnimation::Description), // LIP_ANIMATION
        sizeof(app::dv::DvElementAuraRoad::Description), // AURA_ROAD
        sizeof(app::dv::DvElementMovieView::Description), // MOVIE_VIEW
        sizeof(app::dv::DvElementCrossFade::Description), // CROSS_FADE
        sizeof(app::dv::DvElementWeather::Description), // WEATHER
        sizeof(app::dv::DvElementShadowMapParam::Description), // SHADOW_MAP_PARAM
        sizeof(app::dv::DvElementVariablePointLight::Description), // VARIABLE_POINT_LIGHT
        sizeof(app::dv::DvElementOpeningLogo::Description), // OPENING_LOGO
        sizeof(app::dv::DvElementDensitySectorPoint::Description), // DENSITY_SECTOR_POINT
        sizeof(app::dv::DvElementBulletTime::Description), // BULLET_TIME
        sizeof(app::dv::DvElementTimeStop::Description), // TIME_STOP
        sizeof(app::dv::DvElementObjectTimeStop::Description), // OBJECT_TIME_STOP
        sizeof(app::dv::DvElementShadowShape::Description), // SHADOW_SHAPE
        sizeof(app::dv::DvElementFalloff::Description), // FALLOFF
        sizeof(app::dv::DvElementFog::Description), // FOG
        sizeof(app::dv::DvElementDOF::Description), // DOF
        sizeof(app::dv::DvElementFxColUpdate::Description), // FX_COL_UPDATE
    };
#endif


    void Context::ContainsElement(bool& contains, const unsigned int& elementId, DvNodeBase* node)
    {
        if (contains)
            return;

        if (node->nodeType == DvNodeBase::NodeType::ELEMENT) {
            auto* element = reinterpret_cast<DvNodeElement*>(node);
            if (static_cast<unsigned int>(element->binaryData.elementId) == elementId){
                contains = true;
                return;
            }
        }

        for (auto* x : node->childrenElements0)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenElements1)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenElements2)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenPath)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenCamera)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenMotion)
            ContainsElement(contains, elementId, x);
    }

    void Context::GetFileNode(dv::DvNode& curNode, DvNodeBase* node, dv::DvNode*& result)
    {
        if (strcmp(curNode.name, node->nodeName.c_str()) == 0 && memcmp(reinterpret_cast<char*>(&curNode.guid), node->guid, 16) == 0 && curNode.category == static_cast<int>(node->nodeType)) {
            result = &curNode;
            return;
        }
        for (auto& x : curNode.childNodes)
            GetFileNode(x, node, result);
    }

    void Context::GetFilePage(dv::DvPage& curPage, hh::dv::DvPage* page, dv::DvPage*& result)
    {
        if (memcmp(curPage.name, page->binaryData.pageName, 32) == 0 && curPage.index == page->binaryData.pageIndex)
            result = &curPage;
    }

    void Context::GetRuntimeNode(DvNodeBase* curNode, dv::DvNode* node, DvNodeBase*& result)
    {
        if (strcmp(node->name, curNode->nodeName.c_str()) == 0 && memcmp(reinterpret_cast<char*>(&node->guid), curNode->guid, 16) == 0 && node->category == static_cast<int>(curNode->nodeType)) {
            result = curNode;
            return;
        }
        for (auto* x : curNode->childrenCamera)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenPath)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenMotion)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenElements0)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenElements1)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenElements2)
            GetRuntimeNode(x, node, result);
    }

    void Context::GetParentNode(dv::DvNode& curNode, dv::DvNode* node, dv::DvNode*& result)
    {
        for(auto& x : curNode.childNodes) {
            if (strcmp(x.name, node->name) == 0 && memcmp(reinterpret_cast<char*>(&x.guid), reinterpret_cast<char*>(&node->guid), 16) == 0)
            {
                result = &curNode;
                return;
            }
            GetParentNode(x, node, result);
        }
    }

    void Context::GetChildren(DvNodeBase* node, csl::ut::MoveArray32<DvNodeBase*>& value, bool& includeElements)
    {
        if (node->nodeType == DvNodeBase::NodeType::ELEMENT && !includeElements)
            return;

        value.push_back(node);

        for (auto* x : node->childrenCamera)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenPath)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenMotion)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenElements0)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenElements1)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenElements2)
            GetChildren(x, value, includeElements);
    }

    bool Context::ContainsElement(const unsigned int& elementId)
    {
        bool contains = false;
        if (goDVSC)
            ContainsElement(contains, elementId, goDVSC->nodeTree->mainNode);
        return contains;
    }

    void Context::GetNodes(DvSceneNodeTree* nodeTree, csl::ut::MoveArray32<DvNodeBase*>& value, bool includeElements)
    {
        GetChildren(nodeTree->mainNode, value, includeElements);
    }



    void Context::SetNodeBasicProps(DvNodeBase* node, const char* nodeName, unsigned int nodeType, DvNodeBase* parent)
    {
        GenerateGUID(node->guid);
        node->dvsceneNodeTree = parent->dvsceneNodeTree;
        node->parent = parent;
        node->nodeType = static_cast<DvNodeBase::NodeType>(nodeType);
        for (int x = 0; x < strlen(nodeName) + 1; x++)
            node->nodeName.push_back(nodeName[x]);
    }

    DvNodeBase* Context::CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId, DvNodeBase* parent)
    {
        DvNodeBase* node;
        auto* allocator = parent->dvsceneNodeTree->GetAllocator();
        switch (static_cast<DvNodeBase::NodeType>(nodeType)) {
        case DvNodeBase::NodeType::PATH: {
            node = new (allocator) DvNodePath{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodePath::Description setup{};
            setup.matrix = csl::math::Matrix44::Identity();
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CAMERA: {
            node = new (allocator) DvNodeCamera{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCamera::Description setup{};
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CAMERA_MOTION: {
            node = new (allocator) DvNodeCameraMotion{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCameraMotion::Description setup{};
            setup.end = 100;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CHARACTER: {
            node = new (allocator) DvNodeCharacter{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCharacter::Description setup{};
            strcpy(setup.modelName, "chr_sonic");
            strcpy(setup.skeletonName, "chr_sonic");
            strcpy(setup.name3, "sn");
            setup.useName3 = true;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CHARACTER_MOTION: {
            node = new (allocator) DvNodeCharacterMotion{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCharacterMotion::Description setup{};
            setup.end = 100;
            strcpy(setup.asmState, "Dst0000");
            setup.speed = 1.0f;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::MODEL: {
            node = new (allocator) DvNodeModel{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeModel::Description setup{};
            strcpy(setup.modelName, "chr_sonic");
            strcpy(setup.skeletonName, "chr_sonic");
            strcpy(setup.name3, "sn");
            setup.useName3 = true;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::MODEL_MOTION: {
            node = new (allocator) DvNodeModelMotion{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeModelMotion::Description setup{};
            setup.end = 100;
            strcpy(setup.asmState, "Dst0000");
            setup.speed = 1.0f;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::MODEL_NODE: {
            node = new (allocator) DvNodeModelNode{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeModelNode::Description setup{};
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::ELEMENT: {
            node = new (allocator) DvNodeElement{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            switch (static_cast<DvNodeElement::ElementID>(elementId)) {
            case DvNodeElement::ElementID::PATH_OFFSET: {
                DvNodeElement::Description<DvElementPathOffset::Description> setup = CreateElementDesc<DvElementPathOffset::Description>(elementId);
                setup.elementDescription.offsetMatrix = csl::math::Matrix44::Identity();
                node->Setup(setup);
                break;
            }
            case DvNodeElement::ElementID::PATH_INTERPOLATION: {
                DvNodeElement::Description<DvElementPathInterpolation::Description> setup = CreateElementDesc<DvElementPathInterpolation::Description>(elementId);
                setup.elementDescription.interpolation.scale = { 1,1,1 };
                setup.elementDescription.finishInterpolation.scale = { 1,1,1 };
                node->Setup(setup);
                break;
            }
            case DvNodeElement::ElementID::EFFECT: {
                DvNodeElement::Description<DvElementEffect::Description> setup = CreateElementDesc<DvElementEffect::Description>(elementId);
                setup.elementDescription.r = 255;
                setup.elementDescription.g = 255;
                setup.elementDescription.b = 255;
                setup.elementDescription.a = 255;
                setup.elementDescription.effectMatrix = csl::math::Matrix44::Identity();
                node->Setup(setup);
                break;
            }
            case DvNodeElement::ElementID::FADE: {
                DvNodeElement::Description<DvElementFade::Description> setup = CreateElementDesc<DvElementFade::Description>(elementId);
                setup.elementDescription.enabled = true;
                node->Setup(setup);
                break;
            }
            case DvNodeElement::ElementID::UV_ANIM: 
            case DvNodeElement::ElementID::VISIBILITY_ANIM: 
            case DvNodeElement::ElementID::MATERIAL_ANIM: 
            case DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE: 
            {
                DvNodeElement::Description<DvElementUVAnim::Description> setup = CreateElementDesc<DvElementUVAnim::Description>(elementId);
                setup.elementDescription.speed = 1.0f;
                node->Setup(setup);
                break;
            }
            default: {

                break;
            }
            }
            break;
        }
        }
        return node;
    }

    dv::DvNode Context::CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId)
    {
        dv::DvNode node{};
        size_t setupSize = NodeDataSize[nodeType].second;       
        if (static_cast<DvNodeBase::NodeType>(nodeType) == DvNodeBase::NodeType::ELEMENT) {
            if (elementId >= 1000)
                setupSize += ElementDataSize[elementId + hhElementCount - 1000];
            else
                setupSize += ElementDataSize[elementId];
        }
        node.data = static_cast<char*>(hh::fnd::MemoryRouter::GetModuleAllocator()->Alloc(setupSize, 4));
        node.dataSize = setupSize;
        char*& setup = node.data;
        memset(setup, 0x00, setupSize);
        switch (static_cast<DvNodeBase::NodeType>(nodeType)) {
        case DvNodeBase::NodeType::PATH:
            reinterpret_cast<DvNodePath::Description*>(setup)->matrix = csl::math::Matrix44::Identity();
            break;
        case DvNodeBase::NodeType::CAMERA_MOTION:
            reinterpret_cast<DvNodeCameraMotion::Description*>(setup)->end = 100;
            break;
        case DvNodeBase::NodeType::CHARACTER: {
            auto* setupData = reinterpret_cast<DvNodeCharacter::Description*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case DvNodeBase::NodeType::CHARACTER_MOTION: {
            auto* setupData = reinterpret_cast<DvNodeCharacterMotion::Description*>(setup);
            setupData->end = 100;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case DvNodeBase::NodeType::MODEL: {
            auto* setupData = reinterpret_cast<DvNodeModel::Description*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case DvNodeBase::NodeType::MODEL_MOTION: {
            auto* setupData = reinterpret_cast<DvNodeModelMotion::Description*>(setup);
            setupData->end = 100;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case DvNodeBase::NodeType::ELEMENT: {
            
            auto* setupData = reinterpret_cast<DvNodeElement::Description<DvElementBase::Description>*>(setup);
            setupData->end = 100.0f;
            setupData->playType = DvNodeElement::PlayType::NORMAL;
            setupData->updateTiming = DvNodeElement::UpdateTiming::ON_EXEC_PATH;
            setupData->elementId = static_cast<DvNodeElement::ElementID>(elementId);
            switch (setupData->elementId) {
            case DvNodeElement::ElementID::PATH_OFFSET: {
                auto* elementData = reinterpret_cast<DvElementPathOffset::Description*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->offsetMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case DvNodeElement::ElementID::PATH_INTERPOLATION: {
                auto* elementData = reinterpret_cast<DvElementPathInterpolation::Description*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->interpolation.scale = { 1,1,1 };
                elementData->finishInterpolation.scale = { 1,1,1 };
                break;
            }
            case DvNodeElement::ElementID::EFFECT: {
                auto* elementData = reinterpret_cast<DvElementEffect::Description*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->r = 255;
                elementData->g = 255;
                elementData->b = 255;
                elementData->a = 255;
                elementData->effectMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case DvNodeElement::ElementID::FADE: {
                auto* elementData = reinterpret_cast<app::dv::DvElementFade::Description*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->enabled = true;
                break;
            }
            case DvNodeElement::ElementID::UV_ANIM:
            case DvNodeElement::ElementID::VISIBILITY_ANIM:
            case DvNodeElement::ElementID::MATERIAL_ANIM:
            case DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE:
            {
                auto* elementData = reinterpret_cast<DvElementUVAnim::Description*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->speed = 1.0f;
                break;
            }
            }
            auto curveDataInfo = GetElementTimelineRender(static_cast<DvNodeElement::ElementID>(elementId));
            if (curveDataInfo != std::pair<size_t, size_t>{})
                Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(setup) + setupSize + curveDataInfo.first), curveDataInfo.second/sizeof(float), 0, false);
            break;
        }
        }
        GenerateGUID(reinterpret_cast<char*>(&node.guid));
        node.category = nodeType;
        strcpy(node.name, nodeName);
        return node;
    }

    void Context::ParentNode(DvNodeBase* parent, DvNodeBase* child)
    {
        switch (child->nodeType) {
        case DvNodeBase::NodeType::PATH:
            parent->childrenPath.push_back(child);
            break;
        case DvNodeBase::NodeType::CAMERA:
            parent->childrenCamera.push_back(child);
            break;
        case DvNodeBase::NodeType::CAMERA_MOTION:
        case DvNodeBase::NodeType::CHARACTER_MOTION:
        case DvNodeBase::NodeType::MODEL_MOTION:
            parent->childrenMotion.push_back(child);
            break;
        default:
            parent->childrenElements0.push_back(child);
            break;
        }
    }

    void Context::ParentNode(dv::DvNode& parent, dv::DvNode& child)
    {
        parent.childNodes.push_back(child);
    }

    dv::DvNode* Context::GetFileNode(DvNodeBase* node)
    {
        dv::DvNode* result;
        GetFileNode(*parsedScene->dvCommon->node, node, result);
        return result;
    }

    dv::DvPage* Context::GetFilePage(hh::dv::DvPage* page)
    {
        dv::DvPage* result{};
        for (auto& x : parsedScene->dvCommon->pages) {
            if (!result)
                GetFilePage(x, page, result);
            else
                break;
        }
        return result;
    }

    DvNodeBase* Context::GetRuntimeNode(dv::DvNode* node)
    {
        DvNodeBase* result;
        GetRuntimeNode(goDVSC->nodeTree->mainNode, node, result);
        return result;
    }

    void Context::GenerateGUID(char* guid)
    {
        std::uniform_int_distribution<int> dis(0, 255);
        for (char x = 0; x < 16; x++)
            guid[x] = static_cast<char>(dis(mt));
    }

    dv::DvNode* Context::GetParentNode(dv::DvNode* node)
    {
        dv::DvNode* result;
        GetParentNode(*parsedScene->dvCommon->node, node, result);
        return result;
    }

    dv::Resource* Context::GetResourceByFileNode(dv::DvNode* node)
    {
        for(auto& res : parsedScene->dvResource)
            if(memcmp(&node->guid, &res.guid, 16) == 0)
                return &res;
        return nullptr;
    }

    dv::DvPage Context::CreatePage(const char* pageName, unsigned int idx)
    {
        dv::DvPage page{};
        page.frameEnd = 100;
        page.index = idx;
        page.skipFrame = 150;
        memset(page.name, 0, 32);
        strcpy(page.name, pageName);
        return page;
    }

    hh::dv::DvPage* Context::CreatePage(const char* pageName, unsigned int idx, DvSceneControl* dvsc)
    {
        auto* allocator = dvsc->GetAllocator();
        auto* rawMem = allocator->Alloc(sizeof(hh::dv::DvPage), alignof(hh::dv::DvPage));
        hh::dv::DvPage* page = new (rawMem) hh::dv::DvPage(allocator);
        page->dvSceneControl = dvsc;
        page->binaryData.end = 100;
        page->binaryData.pageIndex = idx;
        page->binaryData.skipFrame = 150;
        strcpy(page->binaryData.pageName, pageName);
        return page;
    }

    void Context::CreateWrapperPages()
    {
        dvPages.clear();
        for (auto* page : goDVSC->timeline->pages)
            dvPages.push_back(new DvPage(page, *this));
    }

    Context::Context(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }, cutsceneName{ allocator }, nodeName{ allocator }, dvPages{ allocator } {}

    template<typename T>
    hh::dv::DvNodeElement::Description<T> Context::CreateElementDesc(unsigned int elementId)
    {
        hh::dv::DvNodeElement::Description<T> desc{};
        desc.end = 100.0f;
        desc.playType = hh::dv::DvNodeElement::PlayType::NORMAL;
        desc.updateTiming = hh::dv::DvNodeElement::UpdateTiming::ON_EXEC_PATH;
        desc.elementId = static_cast<hh::dv::DvNodeElement::ElementID>(elementId);
        desc.elementDescription = {};
        auto curveDataInfo = GetElementTimelineRender(static_cast<hh::dv::DvNodeElement::ElementID>(elementId));
        if (curveDataInfo != std::pair<size_t, size_t>{})
            Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(&desc) + sizeof(hh::dv::DvNodeElement::Description<hh::dv::DvElementBase>) + curveDataInfo.first), curveDataInfo.second / sizeof(float), 0, false);
        return desc;
    }
}

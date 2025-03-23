#include "Context.h"
#include "timeline/Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    constexpr std::pair<size_t, size_t> NodeDataSize[] = {
        {},
        {sizeof(hh::dv::DvNodePath), sizeof(hh::dv::DvNodePath::Data)},
        {},
        {sizeof(hh::dv::DvNodeCamera), sizeof(hh::dv::DvNodeCamera::Data)},
        {sizeof(hh::dv::DvNodeCameraMotion), sizeof(hh::dv::DvNodeCameraMotion::Data)},
        {sizeof(hh::dv::DvNodeCharacter), sizeof(hh::dv::DvNodeCharacter::Data)},
        {sizeof(hh::dv::DvNodeCharacterMotion), sizeof(hh::dv::DvNodeCharacterMotion::Data)},
        {},
        {sizeof(hh::dv::DvNodeModel), sizeof(hh::dv::DvNodeModel::Data)},
        {},
        {sizeof(hh::dv::DvNodeModelMotion), sizeof(hh::dv::DvNodeModelMotion::Data)},
        {sizeof(hh::dv::DvNodeModelNode), sizeof(hh::dv::DvNodeModelNode::Data)},
        {sizeof(hh::dv::DvNodeElement), sizeof(hh::dv::DvNodeElement::Data)}
    };

#ifdef DEVTOOLS_TARGET_SDK_rangers
    constexpr size_t ElementDataSize[] = {
        0, // 0
        sizeof(hh::dv::DvElementCameraParams::Data), // CAMERA_PARAMS
        0, // 2 (Empty)
        sizeof(hh::dv::DvElementDrawOff::Data), // DRAW_OFF
        0, // 4 (Empty)
        sizeof(hh::dv::DvElementPathOffset::Data), // PATH_OFFSET
        sizeof(hh::dv::DvElementCameraShake::Data), // CAMERA_SHAKE
        sizeof(hh::dv::DvElementCameraShakeLoop::Data), // CAMERA_SHAKE_LOOP
        /*sizeof(hh::dv::DvElementEffect::Data)*/676, // EFFECT
        sizeof(hh::dv::DvElementDirectionalLight::Data), // DIRECTIONAL_LIGHT
        sizeof(hh::dv::DvElementPathInterpolation::Data), // PATH_INTERPOLATION
        0, // CULLING_DISABLE
        sizeof(hh::dv::DvElementCameraNearFar::Data), // CAMERA_NEAR_FAR
        sizeof(hh::dv::DvElementUVAnim::Data), // UV_ANIM
        sizeof(hh::dv::DvElementVisibilityAnim::Data), // VISIBILITY_ANIM
        sizeof(hh::dv::DvElementMaterialAnim::Data), // MATERIAL_ANIM
        sizeof(hh::dv::DvElementMultipleAnim::Data), // MULTIPLE_ANIM
        sizeof(hh::dv::DvElementCameraOffset::Data), // CAMERA_OFFSET
        sizeof(hh::dv::DvElementModelFade::Data), // MODEL_FADE
        sizeof(hh::dv::DvElementDebugMotion::Data), // DEBUG_MOTION
        sizeof(hh::dv::DvElementCameraHedgehog::Data), // CAMERA_HEDGEHOG
        sizeof(hh::dv::DvElementCameraInGame::Data), // CAMERA_IN_GAME
        sizeof(hh::dv::DvElementPointLight::Data), // POINT_LIGHT
        sizeof(hh::dv::DvElementVertexAnimationTexture::Data), // VERTEX_ANIMATION_TEXTURE
        sizeof(hh::dv::DvElementSpotlight::Data), // SPOTLIGHT
        sizeof(hh::dv::DvElementControllerVibration::Data), // CONTROLLER_VIBRATION
        sizeof(hh::dv::DvElementSpotlightModel::Data), // SPOTLIGHT_MODEL

        sizeof(app::dv::DvElementBloomParam::Data), // BLOOM_PARAM
        sizeof(app::dv::DvElementDOFParam::Data), // DOF_PARAM
        sizeof(app::dv::DvElementColorContrast::Data), // COLOR_CONTRAST
        sizeof(app::dv::DvElementCameraControlParam::Data), // CAMERA_CONTROL_PARAM
        sizeof(app::dv::DvElementShadowResolution::Data), // SHADOW_RESOLUTION
        sizeof(app::dv::DvElementGodrayParam::Data), // GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereGodrayParam::Data), // ATMOSPHERE_GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereHeightFogParam::Data), // ATMOSPHERE_HEIGHT_FOG_PARAM
        sizeof(app::dv::DvElementChromaticAberrationFilterParam::Data), // CHROMATIC_ABERRATION_FILTER_PARAM
        sizeof(app::dv::DvElementVignetteParam::Data), // VIGNETTE_PARAM
        sizeof(app::dv::DvElementFade::Data), // FADE
        sizeof(app::dv::DvElementLetterBox::Data), // LETTER_BOX
        sizeof(app::dv::DvElementModelClipping::Data), // MODEL_CLIPPING
        sizeof(app::dv::DvElementPbaReset::Data), // PBA_RESET
        sizeof(app::dv::DvElementBossName::Data), // BOSS_NAME
        sizeof(app::dv::DvElementCaption::Data), // CAPTION
        sizeof(app::dv::DvElementSound::Data), // SOUND
        sizeof(app::dv::DvElementTime::Data), // TIME
        sizeof(app::dv::DvElementSun::Data), // SUN
        sizeof(app::dv::DvElementLookAtIK::Data), // LOOK_AT_IK
        sizeof(app::dv::DvElementCameraBlurParam::Data), // CAMERA_BLUR_PARAM
        sizeof(app::dv::DvElementGeneralTrigger::Data), // GENERAL_TRIGGER
        sizeof(app::dv::DvElementFootIk::Data), // FOOT_IK
        sizeof(app::dv::DvElementDitherParam::Data), // DITHER_PARAM
        sizeof(app::dv::DvElementQTE::Data), // QTE
        sizeof(app::dv::DvElementFacialAnimation::Data), // FACIAL_ANIMATION
        sizeof(app::dv::DvElementOverrideASM::Data), // OVERRIDE_ASM
        sizeof(app::dv::DvElementAura::Data), // AURA
        sizeof(app::dv::DvElementChangeTimeScale::Data), // CHANGE_TIME_SCALE
        sizeof(app::dv::DvElementCyberSpaceNoise::Data), // CYBER_SPACE_NOISE
        sizeof(app::dv::DvElementLipAnimation::Data), // LIP_ANIMATION
        sizeof(app::dv::DvElementAuraRoad::Data), // AURA_ROAD
        0, // MOVIE_VIEW
        sizeof(app::dv::DvElementCrossFade::Data), // CROSS_FADE
        sizeof(app::dv::DvElementWeather::Data), // WEATHER
        sizeof(app::dv::DvElementShadowMapParam::Data), // SHADOW_MAP_PARAM
        sizeof(app::dv::DvElementVariablePointLight::Data), // VARIABLE_POINT_LIGHT
        0, // OPENING_LOGO
        sizeof(app::dv::DvElementDensitySectorPoint::Data), // DENSITY_SECTOR_POINT
        0, // FX_COL_UPDATE
        sizeof(app::dv::DvElementQTEAccel::Data), // QTE_ACCEL
        sizeof(app::dv::DvElementTheEndCableObject::Data), // THE_END_CABLE_OBJECT
        sizeof(app::dv::DvElementRifleBeastLighting::Data), // RIFLE_BEAST_LIGHTING
    };
#elif DEVTOOLS_TARGET_SDK_miller
    constexpr size_t ElementDataSize[] = {
        0, // 0 (Unused)
        sizeof(hh::dv::DvElementCameraParams::Data), // CAMERA_PARAMS
        0, // 2 (Empty)
        sizeof(hh::dv::DvElementDrawOff::Data), // DRAW_OFF
        0, // 4 (Empty)
        sizeof(hh::dv::DvElementPathOffset::Data), // PATH_OFFSET
        sizeof(hh::dv::DvElementCameraShake::Data), // CAMERA_SHAKE
        sizeof(hh::dv::DvElementCameraShakeLoop::Data), // CAMERA_SHAKE_LOOP
        sizeof(hh::dv::DvElementEffect::Data), // EFFECT
        sizeof(hh::dv::DvElementDirectionalLight::Data), // DIRECTIONAL_LIGHT
        sizeof(hh::dv::DvElementPathInterpolation::Data), // PATH_INTERPOLATION
        0, // CULLING_DISABLE
        sizeof(hh::dv::DvElementCameraNearFar::Data), // CAMERA_NEAR_FAR
        sizeof(hh::dv::DvElementUVAnim::Data), // UV_ANIM
        sizeof(hh::dv::DvElementVisibilityAnim::Data), // VISIBILITY_ANIM
        sizeof(hh::dv::DvElementMaterialAnim::Data), // MATERIAL_ANIM
        sizeof(hh::dv::DvElementMultipleAnim::Data), // MULTIPLE_ANIM
        sizeof(hh::dv::DvElementCameraOffset::Data), // CAMERA_OFFSET
        sizeof(hh::dv::DvElementModelFade::Data), // MODEL_FADE
        sizeof(hh::dv::DvElementDebugMotion::Data), // DEBUG_MOTION
        sizeof(hh::dv::DvElementCameraHedgehog::Data), // CAMERA_HEDGEHOG
        sizeof(hh::dv::DvElementCameraInGame::Data), // CAMERA_IN_GAME
        sizeof(hh::dv::DvElementPointLight::Data), // POINT_LIGHT
        sizeof(hh::dv::DvElementVertexAnimationTexture::Data), // VERTEX_ANIMATION_TEXTURE
        sizeof(hh::dv::DvElementSpotlight::Data), // SPOTLIGHT
        sizeof(hh::dv::DvElementControllerVibration::Data), // CONTROLLER_VIBRATION
        sizeof(hh::dv::DvElementTexturePatternAnim::Data), // TEXTURE_PATTERN_ANIM
        sizeof(hh::dv::DvElementMaterialParam::Data), // MATERIAL_PARAM

        sizeof(app::dv::DvElementBloomParam::Data), // BLOOM_PARAM
        sizeof(app::dv::DvElementDOFParam::Data), // DOF_PARAM
        sizeof(app::dv::DvElementColorContrast::Data), // COLOR_CONTRAST
        sizeof(app::dv::DvElementCameraControlParam::Data), // CAMERA_CONTROL
        sizeof(app::dv::DvElementShadowResolution::Data), // SHADOW_RESOLUTION
        sizeof(app::dv::DvElementSSAOParam::Data), // SSAO_PARAM
        sizeof(app::dv::DvElementOcclusionCapsuleParam::Data), // OCCLUSION_CAPSULE_PARAM
        sizeof(app::dv::DvElementGodrayParam::Data), // GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereGodrayParam::Data), // ATMOSPHERE_GODRAY_PARAM
        sizeof(app::dv::DvElementAtmosphereHeightFogParam::Data), // ATMOSPHERE_HEIGHT_FOG_PARAM
        sizeof(app::dv::DvElementChromaticAberrationFilterParam::Data), // CHROMATIC_ABERRATION_FILTER_PARAM
        sizeof(app::dv::DvElementVignetteParam::Data), // VIGNETTE_PARAM
        sizeof(app::dv::DvElementFade::Data), // FADE
        sizeof(app::dv::DvElementLetterBox::Data), // LETTER_BOX
        sizeof(app::dv::DvElementModelClipping::Data), // MODEL_CLIPPING
        sizeof(app::dv::DvElementPbaReset::Data), // PBA_RESET
        sizeof(app::dv::DvElementBossName::Data), // BOSS_NAME
        sizeof(app::dv::DvElementCaption::Data), // CAPTION
        sizeof(app::dv::DvElementSound::Data), // SOUND
        sizeof(app::dv::DvElementTime::Data), // TIME
        sizeof(app::dv::DvElementSun::Data), // SUN
        sizeof(app::dv::DvElementLookAtIK::Data), // LOOK_AT_IK
        sizeof(app::dv::DvElementCameraBlurParam::Data), // CAMERA_BLUR_PARAM
        sizeof(app::dv::DvElementGeneralTrigger::Data), // GENERAL_TRIGGER
        sizeof(app::dv::DvElementFootIk::Data), // FOOT_IK
        sizeof(app::dv::DvElementDitherParam::Data), // DITHER_PARAM
        sizeof(app::dv::DvElementQTE::Data), // QTE
        sizeof(app::dv::DvElementFacialAnimation::Data), // FACIAL_ANIMATION
        sizeof(app::dv::DvElementOverrideASM::Data), // OVERRIDE_ASM
        sizeof(app::dv::DvElementAura::Data), // AURA
        sizeof(app::dv::DvElementChangeTimeScale::Data), // CHANGE_TIME_SCALE
        sizeof(app::dv::DvElementCyberSpaceNoise::Data), // CYBER_SPACE_NOISE
        sizeof(app::dv::DvElementLipAnimation::Data), // LIP_ANIMATION
        sizeof(app::dv::DvElementAuraRoad::Data), // AURA_ROAD
        sizeof(app::dv::DvElementMovieView::Data), // MOVIE_VIEW
        sizeof(app::dv::DvElementCrossFade::Data), // CROSS_FADE
        sizeof(app::dv::DvElementWeather::Data), // WEATHER
        sizeof(app::dv::DvElementShadowMapParam::Data), // SHADOW_MAP_PARAM
        sizeof(app::dv::DvElementVariablePointLight::Data), // VARIABLE_POINT_LIGHT
        sizeof(app::dv::DvElementOpeningLogo::Data), // OPENING_LOGO
        sizeof(app::dv::DvElementDensitySectorPoint::Data), // DENSITY_SECTOR_POINT
        sizeof(app::dv::DvElementBulletTime::Data), // BULLET_TIME
        sizeof(app::dv::DvElementTimeStop::Data), // TIME_STOP
        sizeof(app::dv::DvElementObjectTimeStop::Data), // OBJECT_TIME_STOP
        sizeof(app::dv::DvElementShadowShape::Data), // SHADOW_SHAPE
        sizeof(app::dv::DvElementFalloff::Data), // FALLOFF
        sizeof(app::dv::DvElementFog::Data), // FOG
        sizeof(app::dv::DvElementDOF::Data), // DOF
        sizeof(app::dv::DvElementFxColUpdate::Data), // FX_COL_UPDATE
    };
#endif


    void Context::ContainsElement(bool& contains, const unsigned int& elementId, hh::dv::DvNodeBase* node)
    {
        if (contains)
            return;

        if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT) {
            auto* element = reinterpret_cast<hh::dv::DvNodeElement*>(node);
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

    void Context::GetFileNode(dv::DvNode& curNode, hh::dv::DvNodeBase* node, dv::DvNode*& result)
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
        if (strcmp(curPage.name, page->binaryData.pageName) == 0 && curPage.index == page->binaryData.pageIndex)
            result = &curPage;
    }

    void Context::GetRuntimeNode(hh::dv::DvNodeBase* curNode, dv::DvNode* node, hh::dv::DvNodeBase*& result)
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

    void Context::GetChildren(hh::dv::DvNodeBase* node, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool& includeElements)
    {
        if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT && !includeElements)
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

    void Context::GetNodes(hh::dv::DvSceneNodeTree* nodeTree, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool includeElements)
    {
        GetChildren(nodeTree->mainNode, value, includeElements);
    }

    hh::dv::DvNodeBase* Context::CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId, hh::dv::DvNodeBase* parent)
    {
        hh::dv::DvNodeBase* node;
        auto* allocator = parent->dvsceneNodeTree->GetAllocator();
        void* rawMem = allocator->Alloc(NodeDataSize[nodeType].first, 16);
        size_t setupSize = NodeDataSize[nodeType].second;
        void* setup = allocator->Alloc(setupSize, 8);
        memset(setup, 0x00, setupSize);
        switch (static_cast<hh::dv::DvNodeBase::NodeType>(nodeType)) {
        case hh::dv::DvNodeBase::NodeType::PATH:
            node = new (rawMem) hh::dv::DvNodePath(allocator);
            reinterpret_cast<hh::dv::DvNodePath::Data*>(setup)->matrix = csl::math::Matrix44::Identity();
            break;
        case hh::dv::DvNodeBase::NodeType::CAMERA:
            node = new (rawMem) hh::dv::DvNodeCamera(allocator);
            break;
        case hh::dv::DvNodeBase::NodeType::CAMERA_MOTION:
            node = new (rawMem) hh::dv::DvNodeCameraMotion(allocator);
            reinterpret_cast<hh::dv::DvNodeCameraMotion::Data*>(setup)->end = 100;
            break;
        case hh::dv::DvNodeBase::NodeType::CHARACTER: {
            node = new (rawMem) hh::dv::DvNodeCharacter(allocator);
            auto* setupData = reinterpret_cast<hh::dv::DvNodeCharacter::Data*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION: {
            node = new (rawMem) hh::dv::DvNodeCharacterMotion(allocator);
            auto* setupData = reinterpret_cast<hh::dv::DvNodeCharacterMotion::Data*>(setup);
            setupData->end = 100;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::MODEL: {
            node = new (rawMem) hh::dv::DvNodeModel(allocator);
            auto* setupData = reinterpret_cast<hh::dv::DvNodeModel::Data*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::MODEL_MOTION: {
            node = new (rawMem) hh::dv::DvNodeModelMotion(allocator);
            auto* setupData = reinterpret_cast<hh::dv::DvNodeModelMotion::Data*>(setup);
            setupData->end = 100;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::MODEL_NODE:
            node = new (rawMem) hh::dv::DvNodeModelNode(allocator);
            break;
        case hh::dv::DvNodeBase::NodeType::ELEMENT: {
            node = new (rawMem) hh::dv::DvNodeElement(allocator);
            size_t elementDataSize = 0;
            if (elementId >= 1000)
                elementDataSize = ElementDataSize[elementId + 27 - 1000];
            else
                elementDataSize = ElementDataSize[elementId];
            setup = allocator->Alloc(setupSize + elementDataSize, 8);
            memset(setup, 0x00, setupSize + elementDataSize);
            auto* setupData = reinterpret_cast<hh::dv::DvNodeElement::Data*>(setup);
            setupData->end = 100.0f;
            setupData->playType = hh::dv::DvNodeElement::PlayType::NORMAL;
            setupData->updateTiming = hh::dv::DvNodeElement::UpdateTiming::ON_EXEC_PATH;
            setupData->elementId = static_cast<hh::dv::DvNodeElement::ElementID>(elementId);
            switch (setupData->elementId) {
            case hh::dv::DvNodeElement::ElementID::PATH_OFFSET: {
                auto* elementData = reinterpret_cast<hh::dv::DvElementPathOffset::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->offsetMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case hh::dv::DvNodeElement::ElementID::PATH_INTERPOLATION: {
                auto* elementData = reinterpret_cast<hh::dv::DvElementPathInterpolation::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->interpolation.scale = { 1,1,1 };
                elementData->finishInterpolation.scale = { 1,1,1 };
                break;
            }
            case hh::dv::DvNodeElement::ElementID::EFFECT: {
                auto* elementData = reinterpret_cast<hh::dv::DvElementEffect::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->r = 255;
                elementData->g = 255;
                elementData->b = 255;
                elementData->a = 255;
                elementData->effectMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case hh::dv::DvNodeElement::ElementID::FADE: {
                auto* elementData = reinterpret_cast<app::dv::DvElementFade::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->enabled = true;
                break;
            }
            case hh::dv::DvNodeElement::ElementID::UV_ANIM: 
            case hh::dv::DvNodeElement::ElementID::VISIBILITY_ANIM: 
            case hh::dv::DvNodeElement::ElementID::MATERIAL_ANIM: 
            case hh::dv::DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE: 
            {
                auto* elementData = reinterpret_cast<hh::dv::DvElementUVAnim::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->speed = 1.0f;
                break;
            }
            }
            auto curveDataInfo = GetElementTimelineRender(elementId);
            if (curveDataInfo != std::pair<size_t, size_t>{})
                Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(setup) + setupSize + curveDataInfo.first), curveDataInfo.second, 0, false);
            break;
        }
        }
        GenerateGUID(node->guid);
        node->dvsceneNodeTree = parent->dvsceneNodeTree;
        node->parent = parent;
        node->nodeType = static_cast<hh::dv::DvNodeBase::NodeType>(nodeType);
        for (int x = 0; x < strlen(nodeName) + 1; x++)
            node->nodeName.push_back(nodeName[x]);
        node->Setup(setup);
        return node;
    }

    dv::DvNode Context::CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId)
    {
        dv::DvNode node{};
        char* rawMem = new char[NodeDataSize[nodeType].first];
        size_t setupSize = NodeDataSize[nodeType].second;       
        if (static_cast<hh::dv::DvNodeBase::NodeType>(nodeType) == hh::dv::DvNodeBase::NodeType::ELEMENT) {
            if (elementId >= 1000)
                setupSize += ElementDataSize[elementId + hhElementCount - 1000];
            else
                setupSize += ElementDataSize[elementId];
        }
        node.data = new char[setupSize];
        node.dataSize = setupSize;
        char*& setup = node.data;
        memset(setup, 0x00, setupSize);
        switch (static_cast<hh::dv::DvNodeBase::NodeType>(nodeType)) {
        case hh::dv::DvNodeBase::NodeType::PATH:
            reinterpret_cast<hh::dv::DvNodePath::Data*>(setup)->matrix = csl::math::Matrix44::Identity();
            break;
        case hh::dv::DvNodeBase::NodeType::CAMERA_MOTION:
            reinterpret_cast<hh::dv::DvNodeCameraMotion::Data*>(setup)->end = 100;
            break;
        case hh::dv::DvNodeBase::NodeType::CHARACTER: {
            auto* setupData = reinterpret_cast<hh::dv::DvNodeCharacter::Data*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION: {
            auto* setupData = reinterpret_cast<hh::dv::DvNodeCharacterMotion::Data*>(setup);
            setupData->end = 100;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::MODEL: {
            auto* setupData = reinterpret_cast<hh::dv::DvNodeModel::Data*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::MODEL_MOTION: {
            auto* setupData = reinterpret_cast<hh::dv::DvNodeModelMotion::Data*>(setup);
            setupData->end = 100;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case hh::dv::DvNodeBase::NodeType::ELEMENT: {
            
            auto* setupData = reinterpret_cast<hh::dv::DvNodeElement::Data*>(setup);
            setupData->end = 100.0f;
            setupData->playType = hh::dv::DvNodeElement::PlayType::NORMAL;
            setupData->updateTiming = hh::dv::DvNodeElement::UpdateTiming::ON_EXEC_PATH;
            setupData->elementId = static_cast<hh::dv::DvNodeElement::ElementID>(elementId);
            switch (setupData->elementId) {
            case hh::dv::DvNodeElement::ElementID::PATH_OFFSET: {
                auto* elementData = reinterpret_cast<hh::dv::DvElementPathOffset::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->offsetMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case hh::dv::DvNodeElement::ElementID::PATH_INTERPOLATION: {
                auto* elementData = reinterpret_cast<hh::dv::DvElementPathInterpolation::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->interpolation.scale = { 1,1,1 };
                elementData->finishInterpolation.scale = { 1,1,1 };
                break;
            }
            case hh::dv::DvNodeElement::ElementID::EFFECT: {
                auto* elementData = reinterpret_cast<hh::dv::DvElementEffect::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->r = 255;
                elementData->g = 255;
                elementData->b = 255;
                elementData->a = 255;
                elementData->effectMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case hh::dv::DvNodeElement::ElementID::FADE: {
                auto* elementData = reinterpret_cast<app::dv::DvElementFade::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->enabled = true;
                break;
            }
            case hh::dv::DvNodeElement::ElementID::UV_ANIM:
            case hh::dv::DvNodeElement::ElementID::VISIBILITY_ANIM:
            case hh::dv::DvNodeElement::ElementID::MATERIAL_ANIM:
            case hh::dv::DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE:
            {
                auto* elementData = reinterpret_cast<hh::dv::DvElementUVAnim::Data*>(reinterpret_cast<size_t>(setup) + setupSize);
                elementData->speed = 1.0f;
                break;
            }
            }
            auto curveDataInfo = GetElementTimelineRender(elementId);
            if (curveDataInfo != std::pair<size_t, size_t>{})
                Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(setup) + setupSize + curveDataInfo.first), curveDataInfo.second, 0, false);
            break;
        }
        }
        GenerateGUID(reinterpret_cast<char*>(&node.guid));
        node.category = nodeType;
        strcpy(node.name, nodeName);
        return node;
    }

    void Context::ParentNode(hh::dv::DvNodeBase* parent, hh::dv::DvNodeBase* child)
    {
        switch (child->nodeType) {
        case hh::dv::DvNodeBase::NodeType::PATH:
            parent->childrenPath.push_back(child);
            break;
        case hh::dv::DvNodeBase::NodeType::CAMERA:
            parent->childrenCamera.push_back(child);
            break;
        case hh::dv::DvNodeBase::NodeType::CAMERA_MOTION:
        case hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION:
        case hh::dv::DvNodeBase::NodeType::MODEL_MOTION:
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

    dv::DvNode* Context::GetFileNode(hh::dv::DvNodeBase* node)
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

    hh::dv::DvNodeBase* Context::GetRuntimeNode(dv::DvNode* node)
    {
        hh::dv::DvNodeBase* result;
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

    Context::Context(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }, cutsceneName{ allocator }, nodeName{ allocator }, dvPages{ allocator } {}
}

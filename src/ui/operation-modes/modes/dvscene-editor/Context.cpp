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
    };


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
        std::uniform_int_distribution<int> dis(0, 255);
        for (char x = 0; x < 16; x++)
            node->guid[x] = static_cast<char>(dis(mt));
        node->dvsceneNodeTree = parent->dvsceneNodeTree;
        node->parent = parent;
        node->nodeType = static_cast<hh::dv::DvNodeBase::NodeType>(nodeType);
        for (int x = 0; x < std::strlen(nodeName); x++)
            node->nodeName.push_back(nodeName[x]);
        node->Setup(setup);
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

    Context::Context(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }, cutsceneName{ allocator }, nodeName{ allocator } {}
}

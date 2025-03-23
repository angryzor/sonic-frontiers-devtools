#include <ui/common/editors/Basic.h>
#include <ui/operation-modes/modes/dvscene-editor/Context.h>
#include "DvScene.h"

bool Editor(const char* label, char* guid, hh::dv::DvSceneNodeTree* nodeTree)
{
    bool changed = false;
    csl::ut::MoveArray32<hh::dv::DvNodeBase*> value{ nodeTree->GetAllocator() };
    ui::operation_modes::modes::dvscene_editor::Context::GetNodes(nodeTree, value, false);
    hh::dv::DvNodeBase* selectedNode;
    for (auto* x : value)
        if (strcmp(x->GetGUID(), guid) == 0) {
            selectedNode = x;
            break;
        }
    if (ImGui::BeginCombo(label, value.empty() ? "None" : selectedNode->nodeName.c_str())) {
        for (auto* x : value) {
            bool isSelected = x == selectedNode;
            if (changed |= ImGui::Selectable(x->nodeName.c_str(), isSelected))
                selectedNode = x;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    value.clear();
    return changed;
}

bool SearchableCombo(const char* label, int* current_item, const char*const* items, int item_count, int searchBufSize) 
{
    static char search_buf[128] = "";
    bool value_changed = false;

    ImGui::PushID(label);

    ImGui::SetNextItemWidth(ImGui::CalcTextSize("e").x * searchBufSize);
    ImGui::InputText("##search", search_buf, searchBufSize);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::CalcTextSize("e").x * searchBufSize);
    if (ImGui::BeginCombo("##combo", items[*current_item])) {
        for (int i = 0; i < item_count; ++i) 
            if (strstr(items[i], search_buf)) 
                if (ImGui::Selectable(items[i], *current_item == i)) {
                    *current_item = i;
                    value_changed = true;
                }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::Text(label);

    ImGui::PopID();
    return value_changed;
}

bool Editor(const char* label, hh::dv::DvElementCameraHedgehog::Data::Camera& cam) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Unk0", cam.unk0);
        changed |= Editor("Position", cam.position);
        changed |= Editor("Target Position", cam.targetPosition);
        changed |= Editor("Rotation", cam.rotation);
        changed |= Editor("Unk1", cam.unk1);
        changed |= Editor("Near Clip", cam.nearClip);
        changed |= Editor("Far Clip", cam.farClip);
        changed |= Editor("FOV", cam.fov);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Position", data.position);
        changed |= Editor("Target Position", data.targetPosition);
        changed |= Editor("FOV", data.fov);
        changed |= Editor("Direction", data.direction);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data, csl::ut::Bitset<hh::dv::DvElementCameraParams::Data::Flags>& flags, bool finishParams) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        if(flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_POSITION : hh::dv::DvElementCameraParams::Data::Flags::POSITION))
            changed |= Editor("Position", data.position);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_TARGET_POSITION : hh::dv::DvElementCameraParams::Data::Flags::TARGET_POSITION))
            changed |= Editor("Target Position", data.targetPosition);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_FOV : hh::dv::DvElementCameraParams::Data::Flags::FOV))
            changed |= Editor("FOV", data.fov);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_ROTATION : hh::dv::DvElementCameraParams::Data::Flags::ROTATION))
            changed |= Editor("Rotation", data.direction);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementPathInterpolation::Data::Interpolation& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Position", data.position);
        changed |= Editor("Rotation", data.rotation);
        changed |= Editor("Scale", data.scale);
        ImGui::TreePop();
    }
    return changed;
}

#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, hh::dv::DvElementPointLight::Data::Parameters& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Range", data.range);
        changed |= Editor("Intensity", data.intensity);
        changed |= Editor("Falloff", data.falloff);
        changed |= Editor("Angle", data.angle);
        ImGui::TreePop();
    }
    return changed;
}
#endif


bool Editor(const char* label, app::dv::DvElementDOFParam::Data::DOFParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)) {
        changed |= Editor("Focus", param.focus);
        changed |= Editor("Focus Range", param.focusRange);
        changed |= Editor("Near Distance", param.nearDist);
        changed |= Editor("Far Distance", param.farDist);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementChromaticAberrationFilterParam::Data::ChromaticAberrationParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)) {
        changed |= Editor("Color Offset", param.colorOffset);
        changed |= Editor("Sphere Curve", param.sphereCurve);
        changed |= Editor("Scale", param.scale);
        changed |= Editor("Position", param.position);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementLookAtIK::Data::Object& obj) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Unk0", obj.unk1);
        changed |= Editor("GUID", obj.guid);
        changed |= Editor("Offset", obj.offset);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementAura::Data::AuraNode& node) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        float color[4] = {static_cast<float>(node.color[1])/255.0f,static_cast<float>(node.color[2])/255.0f,static_cast<float>(node.color[3])/255.0f,static_cast<float>(node.color[0])/255.0f};
        if(changed |= ImGui::ColorEdit4("Color", color)){
            node.color[0] = static_cast<unsigned int>(color[3]*255.0f);
            node.color[1] = static_cast<unsigned int>(color[0]*255.0f);
            node.color[2] = static_cast<unsigned int>(color[1]*255.0f);
            node.color[3] = static_cast<unsigned int>(color[2]*255.0f);
        }
        changed |= Editor("Distance", node.distance);
        changed |= Editor("Noise Texture Scroll Speed", node.noiseTextureScrollSpeed);
        changed |= Editor("Blur Scale", node.blurScale);
        changed |= Editor("Color Gain", node.colorGain);
        changed |= Editor("Noise Gain", node.noiseGain);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementVariablePointLight::Data::Parameters& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Range", data.range);
        changed |= Editor("Intensity", data.intensity);
        changed |= Editor("Falloff", data.falloff);
        changed |= Editor("Angle", data.angle);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementBloomParam::Data::Parameters& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Bloom", data.bloom);
        changed |= Editor("Sample Radius", data.sampleRadius);
        changed |= Editor("Blur Quality", data.blurQuality);
        ImGui::TreePop();
    }
    return changed;
}


#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::ImageCircleParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Position", param.position);
        changed |= Editor("Size", param.size);
        changed |= Editor("Scale", param.scale);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementCameraBlurParam::Data::BlurParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Blur Power", param.blurPower);
        changed |= Editor("Focus Range", param.focusRange);
        changed |= Editor("Alpha Slope", param.alphaSlope);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementFog::Data::FogParameter::DistanceFogParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Near Dist", param.nearDist);
        changed |= Editor("Far Dist", param.farDist);
        changed |= Editor("Influence", param.influence);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementFog::Data::FogParameter::HeightFogParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Min Height", param.minHeight);
        changed |= Editor("Max Height", param.maxHeight);
        changed |= Editor("Near Dist", param.nearDist);
        changed |= Editor("Far Dist", param.farDist);
        changed |= Editor("Influence", param.influence);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementFog::Data::FogParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= ImGui::ColorEdit3("Color", param.color);
        changed |= Editor("Intensity", param.intensity);
        changed |= Editor("Sky Blend", param.skyBlend);
        changed |= Editor("Sky Mips", param.skyMips);
        changed |= Editor("Distance Fog Parameters", param.distanceFogParam);
        changed |= Editor("Height Fog Parameters", param.heightFogParam);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementDOF::Data::DOFParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Focal Length (mm)", param.focalLengthInMilliMeters);
        changed |= Editor("F-Number", param.fNumber);
        changed |= Editor("Fixed FOV Y", param.fixedFovY);
        changed |= Editor("Focus Length", param.focusLength);
        changed |= Editor("Force Focus Range", param.forceFocusRange);
        changed |= Editor("Foreground Bokeh Max Depth", param.foregroundBokehMaxDepth);
        changed |= Editor("Foreground Bokeh Start Depth", param.foregroundBokehStartDepth);
        changed |= Editor("Background Bokeh Start Depth", param.backgroundBokehStartDepth);
        changed |= Editor("Background Bokeh Max Depth", param.backgroundBokehMaxDepth);
        changed |= Editor("COC Max Radius", param.cocMaxRadius);
        changed |= Editor("Foreground Bokeh Scale", param.foregroundBokehScale);
        changed |= Editor("Background Bokeh Scale", param.backgroundBokehScale);
        ImGui::TreePop();
    }
    return changed;
}
#endif

bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::VignetteParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= Editor("Position", param.position);
        changed |= Editor("Size", param.size);
        changed |= Editor("Scale", param.scale);
        changed |= Editor("Direction", param.lineDirection);
        float opacity = static_cast<float>(param.opacity) / 255.0f;
        if (changed |= Editor("Opacity", opacity))
            param.opacity = static_cast<unsigned int>(opacity * 255);
        float color[3] = { static_cast<float>(param.color[0]) / 255, static_cast<float>(param.color[1]) / 255, static_cast<float>(param.color[2]) / 255 };
        if (changed |= ImGui::ColorEdit3("Color", color)) {
            param.color[0] = static_cast<unsigned int>(color[0] * 255);
            param.color[1] = static_cast<unsigned int>(color[1] * 255);
            param.color[2] = static_cast<unsigned int>(color[2] * 255);
        }
        changed |= Editor("Penumbra Scale", param.penumbraScale);
        changed |= Editor("Intensity", param.intensity);
        changed |= Editor("Rotation", param.rotation);
#elif DEVTOOLS_TARGET_SDK_miller
        changed |= Editor("Image Circle Parameters", param.imgCrclParam);
        changed |= Editor("Direction", param.direction);
        changed |= Editor("Opacity", param.opacity);
        float color[3] = { static_cast<float>(param.color[0]) / 255, static_cast<float>(param.color[1]) / 255, static_cast<float>(param.color[2]) / 255 };
        if(changed |= ImGui::ColorEdit3("Color", color)){
            param.color[0] = static_cast<unsigned int>(color[0] * 255);
            param.color[1] = static_cast<unsigned int>(color[1] * 255);
            param.color[2] = static_cast<unsigned int>(color[2] * 255);
        }
        changed |= Editor("Penumbra Scale", param.penumbraScale);
        changed |= Editor("Intensity", param.intensity);
        changed |= Editor("Rotation", param.rotation);
#endif
        ImGui::TreePop();
    }
    return changed;
}

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::DepthParam& param) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Min Penumbra Scale", param.minMaxPenumbraScale[0]);
        changed |= Editor("Max Penumbra Scale", param.minMaxPenumbraScale[1]);
        changed |= Editor("Bokeh Scale", param.bokehScale);
        changed |= Editor("Min DOF Opacity Scale", param.minMaxDOFOpacityScale[0]);
        changed |= Editor("Max DOF Opacity Scale", param.minMaxDOFOpacityScale[1]);
        changed |= Editor("Min Opacity Scale", param.minMaxOpacityScale[0]);
        changed |= Editor("Max Opacity Scale", param.minMaxOpacityScale[1]);
        changed |= Editor("Min Opacity Distance", param.minMaxOpacityDist[0]);
        changed |= Editor("Max Opacity Distance", param.minMaxOpacityDist[1]);
        ImGui::TreePop();
    }
    return changed;
}
#endif

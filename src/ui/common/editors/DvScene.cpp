#include <ui/common/editors/Basic.h>

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::dv::DvElementDOFParam::Data::DOFParam& param) {
    if(ImGui::TreeNode(label)) {
        Editor("Focus", param.focus);
        Editor("Focus Range", param.focusRange);
        Editor("Near Distance", param.nearDist);
        Editor("Far Distance", param.farDist);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementChromaticAberrationFilterParam::Data::ChromaticAberrationParam& param) {
    if(ImGui::TreeNode(label)) {
        Editor("Color Offset", param.colorOffset);
        Editor("Sphere Curve", param.sphereCurve);
        Editor("Scale", param.scale);
        Editor("Position", param.position);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::VignetteParam& param) {
    if(ImGui::TreeNode(label)){
        Editor("Position", param.position);
        Editor("Size", param.size);
        Editor("Scale", param.scale);
        Editor("Rotation", param.rotation);
        Editor("Unk0", param.unk0);
        Editor("Alpha", param.alpha);
        Editor("Unk1", param.unk1);
        Editor("Unk2", param.unk2);
        Editor("Unk3", param.unk3);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::MinMaxParam& param) {
    if(ImGui::TreeNode(label)){
        Editor("Min Penumbra Scale", param.minMaxPenumbraScale[0]);
        Editor("Max Penumbra Scale", param.minMaxPenumbraScale[1]);
        Editor("Bokeh Scale", param.bokehScale);
        Editor("Min DOF Opacity Scale", param.minMaxDOFOpacityScale[0]);
        Editor("Max DOF Opacity Scale", param.minMaxDOFOpacityScale[1]);
        Editor("Min Opacity Scale", param.minMaxOpacityScale[0]);
        Editor("Max Opacity Scale", param.minMaxOpacityScale[1]);
        Editor("Min Opacity Distance", param.minMaxOpacityDist[0]);
        Editor("Max Opacity Distance", param.minMaxOpacityDist[1]);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementLookAtIK::Data::Object& obj) {
    if(ImGui::TreeNode(label)){
        Editor("Unk0", obj.unk1);
        Editor("GUID", obj.guid);
        Editor("Offset", obj.offset);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementAura::Data::AuraNode& node) {
    if(ImGui::TreeNode(label)){
        float color[4] = {static_cast<float>(node.color[1])/255.0f,static_cast<float>(node.color[2])/255.0f,static_cast<float>(node.color[3])/255.0f,static_cast<float>(node.color[0])/255.0f};
        if(ImGui::ColorEdit4("Color", color)){
            node.color[0] = static_cast<unsigned int>(color[3]*255.0f);
            node.color[1] = static_cast<unsigned int>(color[0]*255.0f);
            node.color[2] = static_cast<unsigned int>(color[1]*255.0f);
            node.color[3] = static_cast<unsigned int>(color[2]*255.0f);
        }
        Editor("Distance", node.distance);
        Editor("Noise Texture Scroll Speed", node.noiseTextureScrollSpeed);
        Editor("Blur Scale", node.blurScale);
        Editor("Color Gain", node.colorGain);
        Editor("Noise Gain", node.noiseGain);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data) {
    if (ImGui::TreeNode(label)) {
        Editor("Position", data.position);
        Editor("Target Position", data.targetPosition);
        Editor("FOV", data.fov);
        Editor("Direction", data.direction);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data, csl::ut::Bitset<hh::dv::DvElementCameraParams::Data::Flags>& flags, bool finishParams = false) {
    if (ImGui::TreeNode(label)) {
        if(flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_POSITION : hh::dv::DvElementCameraParams::Data::Flags::POSITION))
            Editor("Position", data.position);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_TARGET_POSITION : hh::dv::DvElementCameraParams::Data::Flags::TARGET_POSITION))
            Editor("Target Position", data.targetPosition);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_FOV : hh::dv::DvElementCameraParams::Data::Flags::FOV))
            Editor("FOV", data.fov);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Data::Flags::FINISH_ROTATION : hh::dv::DvElementCameraParams::Data::Flags::ROTATION))
            Editor("Rotation", data.direction);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, hh::dv::DvElementPathInterpolation::Data::Interpolation& data) {
    if (ImGui::TreeNode(label)) {
        Editor("Position", data.position);
        Editor("Rotation", data.rotation);
        Editor("Scale", data.scale);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementVariablePointLight::Data::Parameters& data) {
    if (ImGui::TreeNode(label)) {
        Editor("Range", data.range);
        Editor("Intensity", data.intensity);
        Editor("Falloff", data.falloff);
        Editor("Angle", data.angle);
        ImGui::TreePop();
        return true;
    }
    return false;
}

bool Editor(const char* label, app::dv::DvElementBloomParam::Data::Parameters& data) {
    if (ImGui::TreeNode(label)) {
        Editor("Bloom", data.bloom);
        Editor("Sample Radius", data.sampleRadius);
        Editor("Blur Quality", data.blurQuality);
        ImGui::TreePop();
        return true;
    }
    return false;
}
#endif
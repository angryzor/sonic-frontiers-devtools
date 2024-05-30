#pragma once
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>

template<typename Rfl> static void RenderFxParamManagerInterpolatorInspector(const char* name, app::gfx::FxParamManager::InterpolatorT<Rfl>& interpolator) {
    if (ImGui::TreeNode(name)) {
        ImGui::Text("Enabled group bits: %x", interpolator.interpolationGroupEnabledBits);

        unsigned short i{ 0 };
        for (auto& interpolationJob : interpolator.interpolationJobs) {
            char nodename[200];
            snprintf(nodename, sizeof(nodename), "Job %d", i++);

            if (ImGui::TreeNode(nodename)) {
                ImGui::Text("Active on interpolation groups %x", interpolationJob.interpolationGroupMemberBits);
                ImGui::Text("Owner hash: %zx", interpolationJob.ownerId);
                ImGui::Text("Priority: %d", interpolationJob.priority);
                Editor("Unk6", interpolationJob.unk6);
                ImGui::Text("Tween duration: %f", 1 / interpolationJob.tweenPositionIncrementPerSecond);
                ImGui::SliderFloat("Current time", &interpolationJob.currentTweenPosition, 0.0f, 1.0f);
                ImGui::SeparatorText("Target parameters");
                Editor("Target parameters", interpolationJob.parameters);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

void RenderGameServiceInspector(app::gfx::FxParamManager& service);

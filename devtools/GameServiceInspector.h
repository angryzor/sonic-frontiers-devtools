#pragma once
#include "common/ReflectionEditor.h"

class GameServiceInspector
{
public:
    static void Render();
    static void RenderServiceInspector(hh::game::GameService& service);
    static void RenderFxParamManagerInspector(app::gfx::FxParamManager& service);
    static void RenderObjectWorldInspector(hh::game::ObjectWorld& service);
    static void RenderUnknownServiceInspector(hh::game::GameService& service);
    template<typename Rfl> static void RenderFxParamManagerInterpolatorInspector(const char* name, app::gfx::FxParamManager::Interpolator<Rfl>& interpolator) {
        if (ImGui::TreeNode(name)) {
            ImGui::Text("Interpolation group count: %d", interpolator.interpolationGroupCount);
            ImGui::Text("Enabled group bits: %x", interpolator.interpolationGroupEnabledBits);

            unsigned short i{ 0 };
            for (auto& interpolationActor : interpolator.interpolationActors) {
                char nodename[200];
                snprintf(nodename, sizeof(nodename), "Actor %d", i++);

                if (ImGui::TreeNode(nodename)) {
                    ImGui::Text("Wants to interpolate groups %x", interpolationActor.wantsToInterpolateGroupBits);
                    ImGui::Text("Delta: %f", interpolationActor.delta);
                    ReflectionEditor::Render(interpolationActor.parameters);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
};

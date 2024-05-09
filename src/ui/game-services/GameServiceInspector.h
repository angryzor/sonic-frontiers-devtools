#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/editors/Reflection.h>

class GameServiceInspector : public StandaloneWindow
{
public:
    GameServiceInspector(csl::fnd::IAllocator* allocator);
    virtual void RenderContents() override;
    static void RenderServiceInspector(hh::game::GameService& service);
    static void RenderFxParamManagerInspector(app::gfx::FxParamManager& service);
    static void RenderObjectWorldInspector(hh::game::ObjectWorld& service);
    static void RenderStageInfoInspector(app::level::StageInfo& service);
    static void RenderLevelInfoInspector(app::level::LevelInfo& service);
    static void RenderGameModeResourceManagerInspector(app::game::GameModeResourceManager& service);
    static void RenderTerrainManagerInspector(app::trr::TerrainManager& service);
    static void RenderCameraManagerInspector(hh::game::CameraManager& service);
    static void RenderCameraServiceInspector(app::camera::CameraService& service);
    static void RenderUnknownServiceInspector(hh::game::GameService& service);
    template<typename Rfl> static void RenderFxParamManagerInterpolatorInspector(const char* name, app::gfx::FxParamManager::Interpolator<Rfl>& interpolator) {
        if (ImGui::TreeNode(name)) {
            ImGui::Text("Interpolation group count: %d", interpolator.interpolationGroupCount);
            ImGui::Text("Enabled group bits: %x", interpolator.interpolationGroupEnabledBits);

            unsigned short i{ 0 };
            for (auto& interpolationJob : interpolator.interpolationJobs) {
                char nodename[200];
                snprintf(nodename, sizeof(nodename), "Job %d", i++);

                if (ImGui::TreeNode(nodename)) {
                    ImGui::Text("Active on interpolation groups %x", interpolationJob.interpolationGroupMemberBits);
                    ImGui::Text("Owner hash: %zx", interpolationJob.ownerId);
                    ImGui::Text("Priority: %d", interpolationJob.priority);
                    ImGui::Text("Unk6: %d", interpolationJob.unk6);
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
};

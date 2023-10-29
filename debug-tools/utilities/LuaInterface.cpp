#include "../Pch.h"
#include "LuaInterface.h"
#include "lua.h"

LuaInterface::LuaInterface(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{ allocator }
{
}

void LuaInterface::Render() {
    bool run_script;
    ImGui::Begin("Lua Script");
    if (ImGui::BeginMenuBar()) {
        ImGui::MenuItem("Run", NULL, &run_script);
        ImGui::EndMenuBar();
    }
    ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);
    ImGui::End();

    if (run_script) {
        luaL_dostring()
    }
}

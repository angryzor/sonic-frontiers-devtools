#pragma once

class LuaInterface : public hh::fnd::ReferencedObject {
    // csl::ut::VariableString script;
    char script[8192];
public:
    LuaInterface(csl::fnd::IAllocator* allocator);
    void Render();
};

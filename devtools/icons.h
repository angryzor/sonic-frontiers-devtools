#pragma once

#include "imgui/imgui.h"
#include "Desktop.h"
#include "Textures.h"

struct IconUV {
    float x;
    float y;
    float w;
    float h;
};

template<typename Id, size_t Count>
struct IconAtlas {
    typedef Id Id_t;

    float width;
    float height;
    const char* atlasFile;
    IconUV uvs[Count];
};

enum class IconId {
    TEXTURE,
    LIGHTING,
    SPRITE,
    AI,
    INPUT,
    UI,
    TRANSFORM,
    PHYSICS,
    ANIMATION,
    GEOMETRY,
    COLLISION,
    SOUND,
    PHOTO,
    CAMERA,
    STATE,
    PARAMETER,
    OBJECT,
    SCRIPTING,
    EVENT,
};

constexpr IconAtlas<IconId, 19> componentIcons = { 67, 84, "angryzor_devtools_component_icons", {
    { 0, 0, 16, 16 },
    { 17, 0, 16, 16 },
    { 34, 0, 16, 16 },
    { 51, 0, 16, 16 },
    { 0, 17, 16, 16 },
    { 17, 17, 16, 16 },
    { 34, 17, 16, 16 },
    { 51, 17, 16, 16 },
    { 0, 34, 16, 16 },
    { 17, 34, 16, 16 },
    { 34, 34, 16, 16 },
    { 51, 34, 16, 16 },
    { 0, 51, 16, 16 },
    { 17, 51, 16, 16 },
    { 34, 51, 16, 16 },
    { 51, 51, 16, 16 },
    { 0, 68, 16, 16 },
    { 17, 68, 16, 16 },
    { 34, 68, 16, 16 },
} };

enum class AssetIconId {
    SURFRIDE_PROJECT,
    ATOM_CONFIG,
    ATOM_PLAYLIST,
    LIGHTING,
    MATERIAL,
    TERRAIN_MODEL,
    MODEL,
    COMPUTE_SHADER,
    FRAGMENT_SHADER,
    VERTEX_SHADER,
    TEXTURE,
    LEVEL,
    MASTER_LEVEL,
    BITMAP_FONT,
    SCALABLE_FONT,
    REFLECTION,
    PACKFILE_LEVELS,
    PACKFILE,
    PARTICLESYSTEM,
    PHYSICS_BONE,
    SOFTBODY,
    HELPER_BONE,
    ORCA,
    UI,
    ANIMATION,
    SKELETON,
    DVSCENE,
    SCRIPTING,
};

constexpr IconAtlas<AssetIconId, 28> assetIcons = { 324, 389, "angryzor_devtools_asset_icons", {
    { 0, 0, 64, 64 },
    { 65, 0, 64, 64 },
    { 130, 0, 64, 64 },
    { 195, 0, 64, 64 },
    { 260, 0, 64, 64 },
    { 0, 65, 64, 64 },
    { 65, 65, 64, 64 },
    { 130, 65, 64, 64 },
    { 195, 65, 64, 64 },
    { 260, 65, 64, 64 },
    { 0, 130, 64, 64 },
    { 65, 130, 64, 64 },
    { 130, 130, 64, 64 },
    { 195, 130, 64, 64 },
    { 260, 130, 64, 64 },
    { 0, 195, 64, 64 },
    { 65, 195, 64, 64 },
    { 130, 195, 64, 64 },
    { 195, 195, 64, 64 },
    { 260, 195, 64, 64 },
    { 0, 260, 64, 64 },
    { 65, 260, 64, 64 },
    { 130, 260, 64, 64 },
    { 195, 260, 64, 64 },
    { 260, 260, 64, 64 },
    { 0, 325, 64, 64 },
    { 65, 325, 64, 64 },
    { 130, 325, 16, 16 },
} };

template<typename Atlas>
void RenderIcon(const Atlas& atlas, typename Atlas::Id_t id, ImVec2 size) {
    auto& uv = atlas.uvs[static_cast<size_t>(id)];
    auto texture = (*rangerssdk::bootstrap::GetAddress(&hh::fnd::ResourceManager::instance))->GetResource<hh::gfnd::ResTexture>(atlas.atlasFile);

    ImGui::Image(GetTextureIDFromResTexture(texture), size, ImVec2(uv.x / atlas.width, uv.y / atlas.height), ImVec2((uv.x + uv.w) / atlas.width, (uv.y + uv.h) / atlas.height));
}

template<typename Atlas>
void RenderIcon(const Atlas& atlas, typename Atlas::Id_t id) {
    auto& uv = atlas.uvs[static_cast<size_t>(id)];
    RenderIcon(atlas, id, ImVec2(uv.w, uv.h));
}

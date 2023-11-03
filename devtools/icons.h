#pragma once

#include "imgui/imgui.h"

struct IconUV {
    float x;
    float y;
    float w;
    float h;
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

constexpr float atlasWidth = 67;
constexpr float atlasHeight = 84;

constexpr IconUV iconUVs[] = {
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
};

class Desktop;
void RenderIcon(IconId id, Desktop& desktop);
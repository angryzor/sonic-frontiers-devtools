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

constexpr IconUV iconUVs[] = {
    { 0.0f, 0.0f, 16.0f, 16.0f },
    { 17.0f, 0.0f, 16.0f, 16.0f },
    { 34.0f, 0.0f, 16.0f, 16.0f },
    { 51.0f, 0.0f, 16.0f, 16.0f },
    { 0.0f, 17.0f, 16.0f, 16.0f },
    { 17.0f, 17.0f, 16.0f, 16.0f },
    { 34.0f, 17.0f, 16.0f, 16.0f },
    { 51.0f, 17.0f, 16.0f, 16.0f },
    { 0.0f, 34.0f, 16.0f, 16.0f },
    { 17.0f, 34.0f, 16.0f, 16.0f },
    { 34.0f, 34.0f, 16.0f, 16.0f },
    { 51.0f, 34.0f, 16.0f, 16.0f },
    { 0.0f, 51.0f, 16.0f, 16.0f },
    { 17.0f, 51.0f, 16.0f, 16.0f },
    { 34.0f, 51.0f, 16.0f, 16.0f },
    { 51.0f, 51.0f, 16.0f, 16.0f },
    { 0.0f, 68.0f, 16.0f, 16.0f },
    { 17.0f, 68.0f, 16.0f, 16.0f },
    { 34.0f, 68.0f, 16.0f, 16.0f },
};

class Desktop;
void RenderIcon(IconId id, Desktop& desktop);
#include "icons.h"
#include "stb_image.h"
#include <filesystem>
#include <imgui_internal.h>

int rectIds[sizeof(icons) / sizeof(IconMap)]{};
ImgIcon imgIcons[sizeof(icons) / sizeof(IconMap)]{};
char glyphs[sizeof(icons) / sizeof(IconMap)][5]{ "" };

ImgIcon CreateImgIcon(size_t id) {
    auto& io = ImGui::GetIO();

    const ImFontAtlasCustomRect* rect = io.Fonts->GetCustomRectByIndex(rectIds[id]);

    float tw = io.Fonts->TexWidth;
    float th = io.Fonts->TexHeight;
    float rx = rect->X;
    float ry = rect->Y;
    float rw = rect->Width;
    float rh = rect->Height;

    return { io.Fonts->TexID, ImVec2(rx / tw, ry / th), ImVec2((rx + rw) / tw, (ry + rh) / th), ImVec2(rw, rh) };
}

void RegisterIconGlyphs(ImFont* font) {
    auto& io = ImGui::GetIO();

    for (size_t i = 0; i < sizeof(icons) / sizeof(IconMap); i++) {
        int x, y, comp;
        std::filesystem::path filename{ DEVTOOLS_MOD_FOLDER };
        filename /= "icons";
        filename /= icons[i].filename;
        filename += ".png";
        std::string filenameStr = filename.generic_string();

        stbi_info(filenameStr.c_str(), &x, &y, &comp);

        rectIds[i] = io.Fonts->AddCustomRectFontGlyph(font, icons[i].glyph, x, y, static_cast<float>(x + 1));
        ImTextCharToUtf8(glyphs[i], icons[i].glyph);
    }
}

void AddIconTextures() {
    auto& io = ImGui::GetIO();
    unsigned char* pixels = nullptr;
    int texW, texH;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &texW, &texH);
    
    for (size_t i = 0; i < sizeof(icons) / sizeof(IconMap); i++) {
        if (const ImFontAtlasCustomRect* rect = io.Fonts->GetCustomRectByIndex(rectIds[i])) {
            int w, h, channels;
            std::filesystem::path filename{ DEVTOOLS_MOD_FOLDER };
            filename /= "icons";
            filename /= icons[i].filename;
            filename += ".png";
            std::string filenameStr = filename.generic_string();

            auto* data = stbi_load(filenameStr.c_str(), &w, &h, &channels, STBI_rgb_alpha);

            assert(w == rect->Width);
            assert(h == rect->Height);

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    ImU32* dst = (ImU32*)pixels + (rect->Y + y) * texW + (rect->X) + x;
                    ImU32* src = (ImU32*)data + y * w + x;

                    *dst = *src;
                }
            }

            stbi_image_free(data);
        }

        imgIcons[i] = CreateImgIcon(i);
    }
}

void CreateImgIcons() {
    for (size_t i = 0; i < sizeof(icons) / sizeof(IconMap); i++)
        imgIcons[i] = CreateImgIcon(i);
}

const char* GetIconGlyph(IconId id) {
    return glyphs[static_cast<size_t>(id)];
}

ImgIcon GetIconAsImage(IconId id) {
    auto& ico = imgIcons[static_cast<size_t>(id)];

    return { ImGui::GetIO().Fonts->TexID, ico.uv1, ico.uv2, ico.size };
}

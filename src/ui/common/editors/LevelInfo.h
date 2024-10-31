#pragma once

bool Editor(const char* label, app::level::StageData& obj);

#ifndef DEVTOOLS_TARGET_SDK_wars
bool Editor(const char* label, app::level::PlayerInformation& playerInfo);
#endif

#pragma once

bool Editor(const char* label, app::level::StageData& obj);

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::level::PlayerInformation& playerInfo);
#endif

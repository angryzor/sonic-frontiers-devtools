#include <ui/game-services/game-service-inspectors/RespawnTimeManager.h>

bool Editor(const char* label, app::game::RespawnTimeManager::ObjectRespawnInfo& objectRespawnInfo);

#include <ui/common/editors/TimeService.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

const char* respawnTimeTypeNames[] = { "TIME_SERVICE_MINUTES", "GLOBAL_TIME_SECONDS" };
const char* respawnGroupId[] = { "DEFAULT", "W1", "W2", "W3", "W4" };

bool Editor(const char* label, app::game::RespawnTimeManager::ObjectRespawnInfo& objectRespawnInfo) {
    bool edited{};

    Viewer("Object ID", objectRespawnInfo.objectId);
    edited |= Editor("Death timestamp", objectRespawnInfo.timestamp);
    edited |= Editor("Respawn time", objectRespawnInfo.respawnTime);
    edited |= Editor("Unk1", objectRespawnInfo.unk1);
    edited |= Editor("Unk2", objectRespawnInfo.unk2);
    edited |= Editor("Unk3", objectRespawnInfo.unk3);
    edited |= ComboEnum("Respawn time type", objectRespawnInfo.respawnTimeType, respawnTimeTypeNames);

    return edited;
}

void RenderGameServiceInspector(app::game::RespawnTimeManager& service) {
    ComboEnum("Current respawn group", service.currentRespawnGroup, respawnGroupId);
    Editor("Respawn groups", service.respawnGroups);
}

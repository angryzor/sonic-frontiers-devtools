#include "API.h"
#include <ucsl-reflection/traversals/traversal.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <rip/util/memory.h>
#include <rip/util/object-id-guids.h>
#include <rip/binary/serialization/JsonSerializer.h>
#include <sstream>

namespace json_rfls {
	struct Empty {};

	struct Vector3 {
		float x{}; float y{}; float z{};
	};

	struct DebugCameraSettings {
		Vector3 origin{};
		Vector3 position{};
		float yaw{};
		float pitch{};
		float roll{};
		float zoom{};
		float nearClip{};
		float farClip{};
		float fov{};
		float speed{};
	};

	struct DebugCamera {
		bool enabled{};
		std::optional<DebugCameraSettings> settings{};
	};

	struct ObjectWorldChunk {
		std::string id{};
	};

	struct ObjectWorldChunkLayer {
		std::string id{};
		bool enabled{};
	};

	struct ObjectWorldComponentData {
		std::string type{};
		rfl::Generic parameters{};
	};

	struct ObjectWorldObjectData {
		std::string id{};
		std::string parentId{};
		std::string name{};
		std::string type{};
		Vector3 position{};
		Vector3 rotation{};
		std::vector<ObjectWorldComponentData> components{};
		rfl::Generic parameters{};
	};
}

inline rfl::Object<rfl::Generic> getRflClassSerialization(void* obj, const typename he2sdk::ucsl::GameInterface::RflSystem::RflClass* rflClass) {
	yyjson_mut_doc* doc = yyjson_mut_doc_new(nullptr);

	rip::binary::JsonSerializer serializer{ doc };
	yyjson_mut_val* json = serializer.serialize(obj, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));

	yyjson_mut_doc_set_root(doc, json);

	yyjson_doc* idoc = yyjson_mut_doc_imut_copy(doc, nullptr);
	yyjson_val* iroot = yyjson_doc_get_root(idoc);

	auto result = rfl::json::read<rfl::Object<rfl::Generic>>(iroot);

	yyjson_doc_free(idoc);
	yyjson_mut_doc_free(doc);

	return result.value();
}

hh::game::ObjectWorld* getObjectWorld() {
	auto* objWorld = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjectWorld>();

	if (!objWorld) {
		throw API::ErrorResponse{
			.status = "400 Bad Request",
			.message = "ObjectWorld service is not active",
		};
	}

	return objWorld;
}

hh::game::ObjectWorldChunk* getObjectWorldChunk(hh::game::ObjectWorld* objWorld, const std::string& chunkId) {
	size_t iChunkId;

	std::istringstream{ chunkId } >> iChunkId;

	if (iChunkId > objWorld->GetWorldChunks().size()) {
		throw API::ErrorResponse{
			.status = "404 Not Found",
			.message = "chunkId: invalid index",
		};
	}

	return objWorld->GetWorldChunks()[iChunkId];
}

hh::game::ObjectWorldChunkLayer* getObjectWorldChunkLayer(hh::game::ObjectWorldChunk* chunk, const std::string& layerId) {
	auto* layer = chunk->GetLayerByName(layerId.c_str());

	if (!layer) {
		throw API::ErrorResponse{
			.status = "404 Not Found",
			.message = "layerId: invalid id",
		};
	}

	return layer;
}

hh::game::ObjectData* getObjectData(hh::game::ObjectWorldChunkLayer* layer, const std::string& objectId) {
	auto objId = rip::util::fromGUID<hh::game::ObjectId>(objectId.c_str());

	auto& objects = layer->GetResource()->GetObjects();
	auto object_it = std::find_if(objects.begin(), objects.end(), [objId](auto* obj) { return obj->id == objId; });

	if (object_it == objects.end()) {
		throw API::ErrorResponse{
			.status = "404 Not Found",
			.message = "objectId: invalid id",
		};
	}

	return *object_it;
}

API::API(csl::fnd::IAllocator* allocator, const char* host, unsigned short port) : Module{ allocator }, httpThread{ [this, host, port]() {
	bool running{};
	auto app = uWS::App()
		.get("/debug-camera", [this](auto* res, auto* req) {
			apiCall(res, []() {
				auto* debugCameraMgr = hh::game::DebugCameraManager::GetInstance();

				if (!debugCameraMgr->isActive)
					return json_rfls::DebugCamera{ .enabled = false };

#ifdef DEVTOOLS_TARGET_SDK_wars
				auto& camCtrl = *((hh::game::FreeCameraController*)&*debugCameraMgr->GetCamera()->controller);
#else
				auto& camCtrl = *((hh::game::DefaultFreeCameraController*)&*debugCameraMgr->GetCamera()->controller)->padController;
#endif

				return json_rfls::DebugCamera{
					.enabled = true,
					.settings = json_rfls::DebugCameraSettings{
						.origin = {.x = camCtrl.unk3.camera.origin.x(), .y = camCtrl.unk3.camera.origin.y(), .z = camCtrl.unk3.camera.origin.z() },
						.position = {.x = camCtrl.unk3.camera.position.x(), .y = camCtrl.unk3.camera.position.y(), .z = camCtrl.unk3.camera.position.z() },
						.yaw = camCtrl.unk3.camera.yaw,
						.pitch = camCtrl.unk3.camera.pitch,
						.roll = camCtrl.unk3.camera.roll,
						.zoom = camCtrl.unk3.camera.zoom,
						.nearClip = camCtrl.unk3.viewport.nearClip,
						.farClip = camCtrl.unk3.viewport.farClip,
						.fov = camCtrl.unk3.viewport.fov,
						.speed = camCtrl.currentSpeed,
					},
				};
			});
		})
		.put("/debug-camera", [this](auto* res, auto* req) {
			auto input = std::make_shared<std::ostringstream>();
			auto isAborted = std::make_shared<bool>();

			res->onData([this, input, isAborted, res](std::string_view chunk, bool isFin) {
				if (*isAborted)
					return;

				*input << chunk;

				if (!isFin)
					return;

				apiCall(res, [body = input->str()]() {
					auto* debugCameraMgr = hh::game::DebugCameraManager::GetInstance();

					auto parsedBody = rfl::json::read<json_rfls::DebugCamera>(body);

					if (!parsedBody) {
						throw ErrorResponse{
							.status = "400 Bad Request",
							.message = parsedBody.error().value().what(),
						};
					}

					auto params = parsedBody.value();

					if (debugCameraMgr->isActive != params.enabled) {
						if (params.enabled)
							debugCameraMgr->ActivateDebugCamera({ 1, 0, 1, 0 });
						else
							debugCameraMgr->DeactivateDebugCamera();
					}

					if (debugCameraMgr->isActive && params.settings.has_value()) {
#ifdef DEVTOOLS_TARGET_SDK_wars
						auto& camCtrl = *((hh::game::FreeCameraController*)&*debugCameraMgr->GetCamera()->controller);
#else
						auto& camCtrl = *((hh::game::DefaultFreeCameraController*)&*debugCameraMgr->GetCamera()->controller)->padController;
#endif

						auto& settings = params.settings.value();

						camCtrl.unk3.camera.origin = { settings.origin.x, settings.origin.y, settings.origin.z };
						camCtrl.unk3.camera.position = { settings.position.x, settings.position.y, settings.position.z };
						camCtrl.unk3.camera.yaw = settings.yaw;
						camCtrl.unk3.camera.pitch = settings.pitch;
						camCtrl.unk3.camera.roll = settings.roll;
						camCtrl.unk3.camera.zoom = settings.zoom;
						camCtrl.unk3.viewport.nearClip = settings.nearClip;
						camCtrl.unk3.viewport.farClip = settings.farClip;
						camCtrl.unk3.viewport.fov = settings.fov;
						camCtrl.currentSpeed = settings.speed;
					}

					return json_rfls::Empty{};
				});
			});
			res->onAborted([isAborted]() {
				*isAborted = true;
			});
		})
		.get("/object-world/chunks", [this](auto* res, auto* req) {
			apiCall(res, []() {
				auto* objWorld = getObjectWorld();

				std::vector<json_rfls::ObjectWorldChunk> results{};

				for (size_t i = 0; i < objWorld->GetWorldChunks().size(); i++) {
					std::ostringstream oss{};
					oss << i;
					results.push_back({ .id = oss.str() });
				}

				return results;
			});
		})
		.get("/object-world/chunks/:chunkId", [this](auto* res, auto* req) {
			apiCall(res, [chunkId = std::string{ req->getParameter("chunkId") }]() {
				auto* objWorld = getObjectWorld();
				getObjectWorldChunk(objWorld, chunkId);

				return json_rfls::ObjectWorldChunk{ .id = chunkId };
			});
		})
		.get("/object-world/chunks/:chunkId/layers", [this](auto* res, auto* req) {
			apiCall(res, [chunkId = std::string{ req->getParameter("chunkId") }, layerId = std::string{ req->getParameter("layerId") }]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);

				std::vector<json_rfls::ObjectWorldChunkLayer> results{};

				for (auto* layer : chunk->GetLayers()) {
					results.push_back({
						.id = layer->GetName(),
						.enabled = layer->IsEnable(),
					});
				}

				return results;
			});
		})
		.get("/object-world/chunks/:chunkId/layers/:layerId", [this](auto* res, auto* req) {
			apiCall(res, [chunkId = std::string{ req->getParameter("chunkId") }, layerId = std::string{ req->getParameter("layerId") }]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* layer = getObjectWorldChunkLayer(chunk, layerId);

				return json_rfls::ObjectWorldChunkLayer{
					.id = layerId,
					.enabled = layer->IsEnable(),
				};
			});
		})
		.get("/object-world/chunks/:chunkId/layers/:layerId/objects", [this](auto* res, auto* req) {
			apiCall(res, [chunkId = std::string{ req->getParameter("chunkId") }, layerId = std::string{ req->getParameter("layerId") }]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* layer = getObjectWorldChunkLayer(chunk, layerId);

				std::vector<json_rfls::ObjectWorldObjectData> results{};

				for (auto* object : layer->GetResource()->GetObjects()) {
					std::vector<json_rfls::ObjectWorldComponentData> components{};

					for (auto* component : object->componentData) {
						components.push_back({
							.type = component->type,
							.parameters = getRflClassSerialization(component->data, hh::game::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(component->type)->GetSpawnerDataClass()),
						});
					}

					results.push_back({
						.id = rip::util::toGUID(object->id),
						.parentId = rip::util::toGUID(object->parentID),
#ifdef DEVTOOLS_TARGET_SDK_wars
						.name = object->name,
#else
						.name = object->name.c_str(),
#endif
						.type = object->gameObjectClass,
						.position = { object->localTransform.position.x(), object->localTransform.position.y(), object->localTransform.position.z() },
						.rotation = { object->localTransform.rotation.x(), object->localTransform.rotation.y(), object->localTransform.rotation.z() },
						.components = components,
						.parameters = getRflClassSerialization(object->spawnerData, hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(object->gameObjectClass)->GetSpawnerDataClass()),
					});
				}

				return results;
			});
		})
		.get("/object-world/chunks/:chunkId/layers/:layerId/objects/:objectId", [this](auto* res, auto* req) {
			apiCall(res, [chunkId = std::string{ req->getParameter("chunkId") }, layerId = std::string{ req->getParameter("layerId") }, objectId = std::string{ req->getParameter("objectId") }]() {
				auto* objWorld = getObjectWorld();
				auto* chunk = getObjectWorldChunk(objWorld, chunkId);
				auto* layer = getObjectWorldChunkLayer(chunk, layerId);
				auto* object = getObjectData(layer, objectId);

				std::vector<json_rfls::ObjectWorldComponentData> components{};

				for (auto* component : object->componentData) {
					components.push_back({
						.type = component->type,
						.parameters = getRflClassSerialization(component->data, hh::game::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(component->type)->GetSpawnerDataClass()),
				});
				}
				
				return json_rfls::ObjectWorldObjectData{
					.id = rip::util::toGUID(object->id),
					.parentId = rip::util::toGUID(object->parentID),
#ifdef DEVTOOLS_TARGET_SDK_wars
					.name = object->name,
#else
					.name = object->name.c_str(),
#endif
					.type = object->gameObjectClass,
					.position = { object->localTransform.position.x(), object->localTransform.position.y(), object->localTransform.position.z() },
					.rotation = { object->localTransform.rotation.x(), object->localTransform.rotation.y(), object->localTransform.rotation.z() },
					.components = components,
					.parameters = getRflClassSerialization(object->spawnerData, hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(object->gameObjectClass)->GetSpawnerDataClass()),
				};
			});
		})
		.listen(host, port, [&running](auto* socket) {
			running = socket != nullptr;
		});
	pApp = &app;
	loop = running ? app.getLoop() : nullptr;
	app.run();
} }
{
	InitializeCriticalSection(&mutex);
}

API::~API() {
	DeleteCriticalSection(&mutex);
	if (loop)
		loop->defer([this]() { pApp->close(); });
	httpThread.join();
}

unsigned int API::GetId() const {
	return id;
}

void API::Update() {
	while (requests.size() > 0) {
		EnterCriticalSection(&mutex);
		auto t = std::move(requests.front());
		requests.pop();
		LeaveCriticalSection(&mutex);
		t();
	}
}

void API::GameServiceAddedCallback(hh::game::GameService* gameService) {
}

void API::GameServiceRemovedCallback(hh::game::GameService* gameService) {
}

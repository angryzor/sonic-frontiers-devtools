#include "DebugCamera.h"
#include "Common.h"

namespace devtools::api::game_manager {
	struct GOCTransformProperties {
		Transform transform{};
		bool transformPosition{};
		bool transformRotation{};
	};

	struct Component {
		unsigned int handle{};
		unsigned int nameHash{};
		std::string type{};
		std::optional<std::variant<GOCTransformProperties>> properties{};
	};

	struct Object {
		unsigned int handle{};
		std::string name{};
		std::string type{};
		std::vector<unsigned int> children{};
		std::vector<Component> components{};
	};

	GOCTransformProperties buildGOCTransformProperties(hh::game::GOCTransform* component) {
		return GOCTransformProperties {
			.transform = fromTransform(component->GetTransform()),
			.transformPosition = component->GetFrame().flags.test(hh::fnd::HFrame::Flag::TRANSFORM_POSITION),
			.transformRotation = component->GetFrame().flags.test(hh::fnd::HFrame::Flag::TRANSFORM_ROTATION),
		};
	}

	std::optional<std::variant<GOCTransformProperties>> buildComponentProperties(hh::game::GOComponent* component) {
		if (component->pStaticClass == hh::game::GOCTransform::GetClass())
			return buildGOCTransformProperties((hh::game::GOCTransform*)component);
		else
			return std::nullopt;
	}

	Component buildComponent(hh::game::GOComponent* component) {
		return {
			.handle = fromHandle(hh::fnd::Handle{ component }),
			.nameHash = component->nameHash,
			.type = component->pStaticClass->pName,
			.properties = buildComponentProperties(component),
		};
	}

	Object buildObject(hh::game::GameObject* object) {
		std::vector<unsigned int> children{};

		for (auto child : object->children)
			children.push_back(fromHandle(child));

		std::vector<Component> components{};

		for (auto* component : object->components)
			components.push_back(buildComponent(component));

		return Object{
			.handle = fromHandle(hh::fnd::Handle{ object }),
			.name = object->name.c_str(),
			.type = object->objectClass->GetName(),
			.children = children,
			.components = components,
		};
	}

	hh::game::GameObject* getObject(unsigned int handle) {
		auto* object = hh::game::GameObjectSystem::GetGameObjectByHandle(toHandle<hh::game::GameObject>(handle));
				
		if (!object) {
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "Unknown object handle.",
			};
		}

		return object;
	}

	void RegisterRoutes(APIContext& ctx) {
		ctx.app->get("/game-manager/objects", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, []() {
				auto* gameManager = hh::game::GameManager::GetInstance();

				std::vector<Object> results{};

				for (auto* object : gameManager->objects)
					results.push_back(buildObject(object));

				return results;
			});
		});
		ctx.app->get("/game-manager/objects/:objectHandle", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [objectHandle = ctx.getParameter<unsigned int>(req, "objectHandle")]() {
				return buildObject(getObject(objectHandle));
			});
		});
	}
}

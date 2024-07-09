#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/math/MathUtils.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include <optional>

template<typename T>
class MousePickingBehaviorBase : public OperationModeBehavior {
public:
	csl::ut::MoveArray<T> pickedObjects{ GetAllocator() };
	ImGuiMouseButton mouseButton{};
	bool picked{};

	static constexpr unsigned int id = 3;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;
};

template<typename T, typename V>
class MousePickingBehavior : public MousePickingBehaviorBase<T> {
public:
	struct Operations {
		virtual void GetAllClickResults(const ImVec2& position, csl::ut::MoveArray<T>& results) = 0;
		virtual void GetBestClickResult(const ImVec2& position, csl::ut::MoveArray<T>& results, V& location, bool& pickedLocation) = 0;
		virtual void GetDragResults(const ImVec2& start, const ImVec2 end, csl::ut::MoveArray<T>& results) = 0;
		virtual const char* GetObjectName(T object) = 0;
	};

protected:
	Operations& operations;
	csl::ut::MoveArray<T> intermediateResults{ this->GetAllocator() };
	ImGuiMouseButton intermediateMouseButton{};
	bool draggingRightPrevFrame{};

public:
	V pickedLocation{};
	bool locationPicked{};

	struct LocationPickInfo {
		V location;
		ImGuiMouseButton button;
	};

	using ObjectsPickedAction = Action<ActionId::OBJECTS_PICKED, ImGuiMouseButton>;
	using LocationPickedAction = Action<ActionId::LOCATION_PICKED, LocationPickInfo>;

	MousePickingBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode, Operations& operations)
		: MousePickingBehaviorBase<T>{ allocator, operationMode }, operations{ operations } {}

	virtual void Render() override {
		auto& io = ImGui::GetIO();

		this->picked = false;
		this->locationPicked = false;
		this->pickedObjects.clear();

		if (this->operationMode.CanTakeMouseControl(this)) {
			if (this->operationMode.IsDragging(this))
				HandleDragSelect();
			else if (!ImGui::GetIO().WantCaptureMouse && !ImGui::IsAnyItemHovered()) {
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					this->operationMode.BeginSingleFrameExclusiveMouseControl(this);
					HandleClickSelect(ImGuiMouseButton_Left);
				}
				else if (!draggingRightPrevFrame && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
					this->operationMode.BeginSingleFrameExclusiveMouseControl(this);
					HandleClickSelect(ImGuiMouseButton_Right);
				}
			}

			this->operationMode.ToggleDragging(this);
		}

		draggingRightPrevFrame = ImGui::IsMouseDragging(ImGuiMouseButton_Right);

		HandlePopupSelect();
	}

	void HandleDragSelect() {
		auto mouseStart = ImGui::GetMousePos() - ImGui::GetMouseDragDelta();
		auto mouseEnd = ImGui::GetMousePos();

		if (ImGui::Begin("Overlay")) {
			ImGui::GetWindowDrawList()->AddRectFilled(mouseStart, mouseEnd, 0x40FFFFFF);
		}
		ImGui::End();

		operations.GetDragResults(mouseStart, mouseEnd, this->pickedObjects);

		this->mouseButton = ImGuiMouseButton_Left;
		this->picked = true;

		this->Dispatch(ObjectsPickedAction{ ImGuiMouseButton_Left });
	}

	void HandleClickSelect(ImGuiMouseButton button) {
		auto mousePos = ImGui::GetMousePos();

		if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) {
			this->intermediateResults.clear();

			operations.GetAllClickResults(mousePos, this->intermediateResults);

			if (intermediateResults.size() > 0) {
				intermediateMouseButton = button;
				ImGui::OpenPopup("Picker results");
				return;
			}
		}
		else {
			operations.GetBestClickResult(mousePos, this->pickedObjects, this->pickedLocation, this->locationPicked);
		}
		
		this->mouseButton = button;
		this->picked = true;

		this->Dispatch(ObjectsPickedAction{ button });

		if (this->locationPicked && this->pickedObjects.size() == 0)
			this->Dispatch(LocationPickedAction{ { this->pickedLocation, button } });
	}

	void HandlePopupSelect() {
		if (ImGui::BeginPopup("Picker results")) {
			for (auto& object : intermediateResults) {
				if (ImGui::Selectable(operations.GetObjectName(object))) {
					this->pickedObjects.push_back(object);
					this->mouseButton = intermediateMouseButton;
					this->picked = true;

					this->Dispatch(ObjectsPickedAction{ intermediateMouseButton });
				}
			}
			ImGui::EndPopup();
		}
	}
};

class MousePicking3DCameraProvider {
public:
	virtual bool GetCameraMatrix(Eigen::Matrix4f& inverseCameraMatrix) = 0;
};

class MousePicking3DCameraManagerCameraProvider : public MousePicking3DCameraProvider {
public:
	virtual bool GetCameraMatrix(Eigen::Matrix4f& inverseCameraMatrix) override {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraSrv = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraSrv->GetTopComponent(0)) {
			inverseCameraMatrix = camera->viewportData.GetInverseViewMatrix() * camera->viewportData.projMatrix.inverse();
			return true;
		}

		return false;
	}
};

template<typename T, typename V>
class MousePicking3DBehavior : public MousePickingBehavior<T, V> {
public:
	struct Operations {
		virtual void GetAllRaycastResults(const Ray3f& ray, csl::ut::MoveArray<T>& results) = 0;
		virtual void GetBestRaycastResult(const Ray3f& ray, csl::ut::MoveArray<T>& results, V& location, bool& pickedLocation) = 0;
		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<T>& results) = 0;
		virtual const char* GetObjectName(T object) = 0;
	};

private:
	struct OperationsAdapter : MousePickingBehavior<T, V>::Operations {
		Operations& operations;
		MousePicking3DCameraProvider& cameraProvider;

		OperationsAdapter(Operations& operations, MousePicking3DCameraProvider& cameraProvider) : operations{ operations }, cameraProvider{ cameraProvider } {}

		template<typename F>
		void WithCameraMatrix(F f) {
			Eigen::Matrix4f inverseCameraMatrix;

			if (!cameraProvider.GetCameraMatrix(inverseCameraMatrix))
				return;

			f(inverseCameraMatrix);
		}

		virtual void GetAllClickResults(const ImVec2& position, csl::ut::MoveArray<T>& results) override {
			return WithCameraMatrix([&](const Eigen::Matrix4f& inverseCameraMatrix) {
				operations.GetAllRaycastResults(ScreenPosToWorldRay(position, inverseCameraMatrix), results);
			});
		}

		virtual void GetBestClickResult(const ImVec2& position, csl::ut::MoveArray<T>& results, V& location, bool& pickedLocation) override {
			return WithCameraMatrix([&](const Eigen::Matrix4f& inverseCameraMatrix) {
				operations.GetBestRaycastResult(ScreenPosToWorldRay(position, inverseCameraMatrix), results, location, pickedLocation);
			});
		}

		virtual void GetDragResults(const ImVec2& start, const ImVec2 end, csl::ut::MoveArray<T>& results) override {
			return WithCameraMatrix([&](const Eigen::Matrix4f& inverseCameraMatrix) {
				operations.GetFrustumResults(ScreenRectToFrustum(start, end, inverseCameraMatrix), results);
			});
		}

		virtual const char* GetObjectName(T object) override {
			return operations.GetObjectName(object);
		}
	};

	OperationsAdapter operationsAdapter;

public:
	MousePicking3DBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode, Operations& operations, MousePicking3DCameraProvider& cameraProvider)
		: operationsAdapter{ operations, cameraProvider }, MousePickingBehavior<T, V>{ allocator, operationMode, operationsAdapter } {}
};

template<typename T>
class MousePickingPhysicsBehavior : public MousePicking3DBehavior<T, csl::math::Vector3> {
public:
	struct Operations {
		virtual T GetObjectForGameObject(hh::game::GameObject* gameObject) = 0;
		virtual bool IsSelectable(hh::game::GameObject* gameObject) = 0;
		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<T>& results) = 0;
		virtual const char* GetObjectName(T object) = 0;
	};

private:
	struct OperationsAdapter : MousePicking3DBehavior<T, csl::math::Vector3>::Operations {
		Operations& operations;

		OperationsAdapter(Operations& operations) : operations{ operations } {}

		virtual void GetAllRaycastResults(const Ray3f& ray, csl::ut::MoveArray<T>& results) {
			if (auto* gameManager = hh::game::GameManager::GetInstance())
			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				csl::ut::MoveArray<hh::physics::PhysicsQueryResult> physicsResults{ hh::fnd::MemoryRouter::GetTempAllocator() };

				if (physicsWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, physicsResults)) {
					csl::ut::PointerMap<T, bool> unique{ hh::fnd::MemoryRouter::GetTempAllocator() };

					for (auto& result : physicsResults) {
						if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
							auto* gameObject = collider->GetOwnerGameObject();

							if (!operations.IsSelectable(gameObject))
								continue;

							auto* object = operations.GetObjectForGameObject(gameObject);

							if (unique.Find(object) == unique.end())
								results.push_back(object);

							unique.Insert(object, true);
						}
					}
				}
			}
		}

		virtual void GetBestRaycastResult(const Ray3f& ray, csl::ut::MoveArray<T>& results, csl::math::Vector3& location, bool& pickedLocation) {
			if (auto* gameManager = hh::game::GameManager::GetInstance())
			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				csl::ut::MoveArray<hh::physics::PhysicsQueryResult> physicsResults{ hh::fnd::MemoryRouter::GetTempAllocator() };

				if (physicsWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, physicsResults)) {
					for (auto& result : physicsResults) {
						if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
							auto* gameObject = collider->GetOwnerGameObject();

							if (Desktop::selectionColliderFilters[gameObject->layer][collider->filterCategory]) {
								if (!operations.IsSelectable(gameObject))
									continue;

								results.push_back(operations.GetObjectForGameObject(gameObject));
								location = result.hitLocation;
								pickedLocation = true;
								return;
							}
						}
					}
				}
			}
		}

		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<T>& results) {
			operations.GetFrustumResults(frustum, results);
		}

		virtual const char* GetObjectName(T object) {
			return operations.GetObjectName(object);
		}
	};

	OperationsAdapter operationsAdapter;
	MousePicking3DCameraManagerCameraProvider cameraProvider;

public:
	MousePickingPhysicsBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode, Operations& operations)
		: operationsAdapter{ operations }, cameraProvider{}, MousePicking3DBehavior<T, csl::math::Vector3>{ allocator, operationMode, operationsAdapter, cameraProvider } {}
};

template<typename T, typename V>
class MousePicking3DRecursiveRaycastBehavior : public MousePicking3DBehavior<T, V> {
public:
	struct Operations {
		virtual void GetRootObjects(csl::ut::MoveArray<T>& rootObjects) = 0;
		virtual void GetChildren(T object, csl::ut::MoveArray<T>& children) = 0;
		virtual bool IsSelectable(T object) = 0;
		virtual bool Intersects(T object, const Ray3f& ray) = 0;
		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<T>& results) = 0;
		virtual const char* GetObjectName(T object) = 0;
	};

private:
	struct OperationsAdapter : MousePicking3DBehavior<T, V>::Operations {
		Operations& operations;

		OperationsAdapter(Operations& operations) : operations{ operations } {}

		virtual void GetAllRaycastResults(const Ray3f& ray, csl::ut::MoveArray<T>& results) {
			csl::ut::MoveArray<T> rootObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };
			operations.GetRootObjects(rootObjects);

			for (auto object : rootObjects)
				if (auto pick = Raycast(object, ray))
					results.push_back(*pick);
		}

		virtual void GetBestRaycastResult(const Ray3f& ray, csl::ut::MoveArray<T>& results, V& location, bool& pickedLocation) {
			csl::ut::MoveArray<T> rootObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };
			operations.GetRootObjects(rootObjects);

			for (auto object : rootObjects)
				if (auto pick = Raycast(object, ray)) {
					results.push_back(*pick);
					return;
				}
		}

		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<T>& results) {
			operations.GetFrustumResults(frustum, results);
		}

		virtual const char* GetObjectName(T object) {
			return operations.GetObjectName(object);
		}

		std::optional<T> Raycast(T object, const Ray3f& ray)
		{
			if (!operations.IsSelectable(object))
				return std::nullopt;

			csl::ut::MoveArray<T> children{ hh::fnd::MemoryRouter::GetTempAllocator() };
			operations.GetChildren(object, children);

			for (size_t i = children.size(); i > 0; i--)
				if (auto childPick = Raycast(children[i - 1], ray))
					return childPick;

			if (operations.Intersects(object, ray))
				return std::make_optional(object);

			return std::nullopt;
		}
	};

	OperationsAdapter operationsAdapter;


public:
	MousePicking3DRecursiveRaycastBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode, Operations& operations, MousePicking3DCameraProvider& cameraProvider)
		: operationsAdapter{ operations }, MousePicking3DBehavior<T, V>{ allocator, operationMode, operationsAdapter, cameraProvider } {}
};


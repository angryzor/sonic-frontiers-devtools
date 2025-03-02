#include "ArrayTool.h"
#include "Actions.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>

namespace ui::operation_modes::modes::level_editor {
	const char* replicationTypeNames[] = { "Grid", "Ellipsoid", "Geodesic polyhedron", "Path" };
	const char* complexSourceNames[] = { "Spawn new object", "Copy selection" };
	const char* autoFillTypeNames[] = { "None", "Repeat until filled (auto repeat)", "Spread to fill (auto distance)", "Spread to fill without endpoints (auto distance)" };

	ArrayTool::ArrayTool(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode, false } {
		InitializeReplicationType();
		InitializeComplexSource();
	}

	ArrayTool::~ArrayTool() {
		ClearTempObjects();
	}

	PanelTraits ArrayTool::GetPanelTraits() const {
		return { "Array tool", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void ArrayTool::ProcessAction(const ActionBase& action) {
		switch (action.id) {
		case OpenArrayToolAction::id:
			isOpen = true;
			break;
		case ObjectClassToPlaceChangedAction::id:
			RebuildTempObjects();
			break;
		case PathsUpdatedAction::id:
			UpdateTempObjects();
			break;
		}
	}

	void ArrayTool::RenderPanel() {
		if (RenderSettings() && GetContext().placementTargetLayer != nullptr)
			UpdateTempObjects();

		if (ImGui::Button("Save"))
			tempObjects.clear();

		if (!isOpen)
			ClearTempObjects();
	}

	bool ArrayTool::RenderSettings() {
		bool edited{};
		
		edited |= ComboEnum("Replication type", replicationType, replicationTypeNames);

		if (edited)
			InitializeReplicationType();

		bool complexSourceEdited = ComboEnum("Object source", complexSource, complexSourceNames);
		edited |= complexSourceEdited;

		if (complexSourceEdited)
			InitializeComplexSource();

		edited |= RenderComplexSourceSettings();

		edited |= Editor("Object limit", maxObjects);
		edited |= Editor("Local position", localPosition);
		edited |= Editor("Local rotation", localRotation);
		edited |= Editor("Snap to ground", snapToGround);

		edited |= RenderReplicationTypeSettings();

		return edited;
	}

	bool ArrayTool::RenderComplexSourceSettings() {
		switch (complexSource) {
		case ComplexSource::CLIPBOARD: return RenderClipboardComplexSourceSettings();
		default: return false;
		}
	}

	bool ArrayTool::RenderClipboardComplexSourceSettings() {
		bool edited{};
		if (ImGui::Button("Copy selection")) {
			auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();
					
			ClearTempObjects();
			clipboard.Copy(selection);
			complexElementTransforms.clear();

			if (selection.size() > 0) {
				auto* aabbBehavior = GetBehavior<SelectionAabbBehavior<Context>>();
				auto base = aabbBehavior->HaveAabb() ? aabbBehavior->GetAabb().Center() : csl::math::Vector3{ selection[0]->transform.position };
					
				for (auto* obj : selection)
					complexElementTransforms.push_back(Eigen::Translation3f{ base }.inverse() * ObjectTransformDataToAffine3f(obj->transform));
			}

			edited = true;
		}
		ImGui::SameLine();
		ImGui::Text("%d objects in clipboard.", clipboard.Size());
		return edited;
	}

	bool ArrayTool::RenderReplicationTypeSettings() {
		switch (replicationType) {
		case ReplicationType::GRID: return RenderGridReplicationTypeSettings();
		case ReplicationType::ELLIPSOID: return RenderEllipsoidReplicationTypeSettings();
		case ReplicationType::GEODESIC_POLYHEDRON: return RenderGeodesicPolyhedronReplicationTypeSettings();
		case ReplicationType::PATH: return RenderPathReplicationTypeSettings();
		default: return false;
		}
	}

	bool ArrayTool::RenderGridReplicationTypeSettings() {
		bool edited{};
		edited |= Editor("Base location", replicationSettings.grid.baseLocation);
		edited |= UnboundedDimensionEditor("X", replicationSettings.grid.x);
		edited |= UnboundedDimensionEditor("Y", replicationSettings.grid.y);
		edited |= UnboundedDimensionEditor("Z", replicationSettings.grid.z);
		return edited;
	}

	bool ArrayTool::RenderEllipsoidReplicationTypeSettings() {
		bool edited{};
		edited |= Editor("Base location", replicationSettings.ellipsoid.baseLocation);
		edited |= Editor("Scale", replicationSettings.ellipsoid.scale);
		edited |= Editor("Rotate to normal", replicationSettings.ellipsoid.normalRotation);
		edited |= DimensionEditor("Azimuthal angle", replicationSettings.ellipsoid.azimuthalAngle);
		edited |= DimensionEditor("Polar angle", replicationSettings.ellipsoid.polarAngle);
		edited |= UnboundedDimensionEditor("Radius", replicationSettings.ellipsoid.radius);
		return edited;
	}

	bool ArrayTool::RenderGeodesicPolyhedronReplicationTypeSettings() {
		bool edited{};
		edited |= Editor("Base location", replicationSettings.geodesicPolyhedron.baseLocation);
		edited |= Editor("Subdivisions", replicationSettings.geodesicPolyhedron.subdivisions);
		edited |= Editor("Rotate to normal", replicationSettings.geodesicPolyhedron.normalRotation);
		edited |= UnboundedDimensionEditor("Radius", replicationSettings.geodesicPolyhedron.radius);
		return edited;
	}

	bool ArrayTool::RenderPathReplicationTypeSettings() {
		bool edited{};
		edited |= Editor("Path name", pathName);
		edited |= Editor("Apply path rotation", replicationSettings.path.applyRotation);
		edited |= DimensionEditor("Distance along path", replicationSettings.path.distance);
		return edited;
	}

	bool ArrayTool::DimensionEditor(const char* label, ArrayTool::DimensionSettings& settings) {
		bool edited{};
		ImGui::PushID(label);
		ImGui::SeparatorText(label);
		edited |= ComboEnum("Autofill type", settings.autoFillType, autoFillTypeNames);

		if (edited)
			InitializeAutoFillType(settings);

		switch (settings.autoFillType) {
		case AutoFillType::NONE:
			edited |= Editor("Repeat count", settings.autoFillSettings.manual.repeat);
			edited |= Editor("Distance between items", settings.autoFillSettings.manual.interval);
			break;
		case AutoFillType::REPEAT:
			edited |= Editor("Distance between items", settings.autoFillSettings.repeat.interval);
			break;
		case AutoFillType::SPREAD_INCLUSIVE:
		case AutoFillType::SPREAD_EXCLUSIVE:
			edited |= Editor("Repeat count", settings.autoFillSettings.spread.repeat);
			break;
		}

		edited |= Editor("Offset", settings.offset);

		ImGui::PopID();
		return edited;
	}

	bool ArrayTool::UnboundedDimensionEditor(const char* label, UnboundedDimensionSettings& settings) {
		bool edited{};
		ImGui::PushID(label);
		edited |= DimensionEditor(label, settings);

		if (settings.autoFillType != AutoFillType::NONE)
			edited |= Editor("Maximum", settings.max);
		else
			settings.max = 200.0f;

		ImGui::PopID();
		return edited;
	}

	void ArrayTool::GenerateGrid(ObjectPool& pool) {
		RepeatDimension(replicationSettings.grid.x, [&](float x) {
			RepeatDimension(replicationSettings.grid.y, [&](float y) {
				RepeatDimension(replicationSettings.grid.z, [&](float z) {
					MaybeSpawnObject(pool, [&]() {
						return Eigen::Translation3f{ replicationSettings.grid.baseLocation + csl::math::Vector3{ x, y, z } };
					});
				});
			});
		});
	}

	void ArrayTool::GenerateEllipsoid(ObjectPool& pool) {
		RepeatDimension(replicationSettings.ellipsoid.azimuthalAngle, 2 * EIGEN_PI, [&](float psi) {
			RepeatDimension(replicationSettings.ellipsoid.polarAngle, EIGEN_PI, [&](float theta) {
				RepeatDimension(replicationSettings.ellipsoid.radius, [&](float r) {
					MaybeSpawnObject(pool, [&]() {
						Eigen::Vector3f localPos{
							replicationSettings.ellipsoid.scale.x() * r * sinf(theta) * cosf(psi),
							replicationSettings.ellipsoid.scale.y() * r * cosf(theta),
							replicationSettings.ellipsoid.scale.z() * r * sinf(theta) * sinf(psi),
						};

						return Eigen::Translation3f{ replicationSettings.ellipsoid.baseLocation + localPos } * (replicationSettings.ellipsoid.normalRotation ? Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f{ 0.0f, 1.0f, 0.0f }, localPos) : Eigen::Quaternionf::Identity());
					});
				});
			});
		}, true);
	}

	void ArrayTool::GenerateGeodesicPolyhedron(ObjectPool& pool) {
		RepeatDimension(replicationSettings.geodesicPolyhedron.radius, [&](float r) {
			hh::fnd::Geometry box{ GetAllocator() };

			box.CreateIcoSphere(replicationSettings.geodesicPolyhedron.baseLocation, r, replicationSettings.geodesicPolyhedron.subdivisions);

			for (auto& vertex : box.vertices) {
				MaybeSpawnObject(pool, [&]() {
					return Eigen::Translation3f{ vertex } * (replicationSettings.geodesicPolyhedron.normalRotation ? Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f{ 0.0f, 1.0f, 0.0f }, vertex - replicationSettings.geodesicPolyhedron.baseLocation) : Eigen::Quaternionf::Identity());
				});
			}
		});
	}

	void ArrayTool::GeneratePath(ObjectPool& pool) {
		if (auto* pathMgr = hh::game::GameManager::GetInstance()->GetService<hh::path::PathManager>())
		if (auto* path = pathMgr->GetPathObject(pathName.c_str())) {
			hh::path::PathEvaluator pathEvaluator{ path };

			RepeatDimension(replicationSettings.path.distance, pathEvaluator.GetLength(), [&](float d) {
				MaybeSpawnObject(pool, [&]() {
					Eigen::Quaternionf rotation{ Eigen::Quaternionf::Identity() };

					csl::math::Vector3 position, normal, tangent;
					pathEvaluator.GetPNT(d, &position, &normal, &tangent);

					if (replicationSettings.path.applyRotation) {
						Eigen::Matrix3f rotMat{};
						rotMat.col(0) = normal.cross(tangent);
						rotMat.col(1) = normal;
						rotMat.col(2) = tangent;

						rotation = Eigen::Quaternionf{ rotMat };
					}

					return Eigen::Translation3f{ position } * rotation;
				});
			});
		}
	}

	ArrayTool::ObjectComplex ArrayTool::SpawnComplex(const csl::math::Vector3& location) {
		csl::ut::MoveArray<hh::game::ObjectData*> objs{ GetAllocator() };

		switch (complexSource) {
		case ComplexSource::SPAWN:
			if (GetContext().placementTargetLayer && GetContext().objectClassToPlace)
				objs.push_back(GetContext().SpawnObject(location));
			break;
		case ComplexSource::CLIPBOARD:
			clipboard.Paste(objs);
			break;
		}

		return objs;
	}

	void ArrayTool::PositionComplex(const Eigen::Affine3f& transform, const ObjectComplex& complex) {
		size_t i{};

		for (auto* obj : complex)
			UpdateAbsoluteTransform(transform * complexElementTransforms[i++], *obj);
	}

	void ArrayTool::DeleteComplex(const ObjectComplex& complex) {
		GetBehavior<SelectionBehavior<Context>>()->Deselect(complex);
		GetContext().DeleteObjects(complex);
	}

	void ArrayTool::InitializeReplicationType() {
		pathName.Set("");

		switch (replicationType) {
		case ReplicationType::GRID: replicationSettings.grid = {}; break;
		case ReplicationType::ELLIPSOID: replicationSettings.ellipsoid = {}; break;
		case ReplicationType::GEODESIC_POLYHEDRON: replicationSettings.geodesicPolyhedron = {}; break;
		case ReplicationType::PATH: replicationSettings.path = {}; break;
		}
	}

	void ArrayTool::InitializeComplexSource() {
		clipboard.Clear();
		complexElementTransforms.clear();

		switch (complexSource) {
		case ComplexSource::SPAWN:
			complexElementTransforms.push_back(Eigen::Affine3f::Identity());
			break;
		}
	}

	void ArrayTool::InitializeAutoFillType(ArrayTool::DimensionSettings& settings) {
		switch (settings.autoFillType) {
		case AutoFillType::NONE: settings.autoFillSettings.manual = {}; break;
		case AutoFillType::REPEAT: settings.autoFillSettings.repeat = {}; break;
		case AutoFillType::SPREAD_INCLUSIVE: settings.autoFillSettings.spread = {}; break;
		case AutoFillType::SPREAD_EXCLUSIVE: settings.autoFillSettings.spread = {}; break;
		}
	}

	void ArrayTool::ResetCurObjects() {
		curObjects = 0;
	}

	void ArrayTool::UpdateTempObjects() {
		ObjectPool pool{ GetAllocator(), *this, std::move(tempObjects) };

		ResetCurObjects();
			
		switch (replicationType) {
		case ReplicationType::GRID: GenerateGrid(pool); break;
		case ReplicationType::ELLIPSOID: GenerateEllipsoid(pool); break;
		case ReplicationType::GEODESIC_POLYHEDRON: GenerateGeodesicPolyhedron(pool); break;
		case ReplicationType::PATH: GeneratePath(pool); break;
		}

		for (auto& complex : tempObjects)
			GetContext().RecalculateDependentTransforms(complex);

		Dispatch(SceneChangedAction{});
	}

	void ArrayTool::ClearTempObjects() {
		if (!GetContext().GetFocusedChunk())
			return;

		for (auto& complex : tempObjects)
			DeleteComplex(complex);

		tempObjects.clear();
	}

	void ArrayTool::RebuildTempObjects() {
		ClearTempObjects();
		UpdateTempObjects();
	}

	ArrayTool::ObjectPool::ObjectPool(csl::fnd::IAllocator* allocator, ArrayTool& arrayTool, csl::ut::MoveArray<ArrayTool::ObjectComplex>&& objects) : CompatibleObject{ allocator }, arrayTool{ arrayTool }, pool{ std::move(objects) } {}

	ArrayTool::ObjectPool::~ObjectPool() {
		for (auto& complex : pool)
			arrayTool.DeleteComplex(complex);
	}

	ArrayTool::ObjectComplex ArrayTool::ObjectPool::Spawn(const Eigen::Affine3f& transform) {
		if (pool.empty()) {
			auto complex = arrayTool.SpawnComplex(transform.translation());
			arrayTool.PositionComplex(transform, complex);
			return complex;
		}
		else {
			auto complex = std::move(pool[pool.size() - 1]);
			pool.remove(pool.size() - 1);
			arrayTool.PositionComplex(transform, complex);
			return complex;
		}
	}
}

#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::level_editor {
	class ObjectLibrary;
	class ArrayTool : public Panel<Context> {
		enum class ReplicationType {
			GRID,
			ELLIPSOID,
			GEODESIC_POLYHEDRON,
			PATH,
		};

		enum class AutoFillType {
			NONE,
			REPEAT,
			SPREAD_INCLUSIVE,
			SPREAD_EXCLUSIVE,
		};

		struct ManualFillSettings {
			unsigned int repeat{ 1 };
			float interval{ 1.0f };
		};

		struct AutoRepeatFillSettings {
			float interval{ 1.0f };
		};

		struct AutoSpreadFillSettings {
			unsigned int repeat{ 1 };
		};

		union AutoFillSettings {
			ManualFillSettings manual{};
			AutoRepeatFillSettings repeat;
			AutoSpreadFillSettings spread;
		};

		struct DimensionSettings {
			AutoFillType autoFillType{};
			AutoFillSettings autoFillSettings{};
			float offset{ 0.0f };
		};

		struct UnboundedDimensionSettings : DimensionSettings {
			float max{ 200.0f };
		};

		struct FixedReplicationSettings {
			csl::math::Vector3 baseLocation{};
		};

		struct GridReplicationSettings : FixedReplicationSettings {
			UnboundedDimensionSettings x{};
			UnboundedDimensionSettings y{};
			UnboundedDimensionSettings z{};
		};

		struct EllipsoidReplicationSettings : FixedReplicationSettings {
			csl::math::Vector3 scale{ 1.0f, 1.0f, 1.0f };
			DimensionSettings azimuthalAngle{};
			DimensionSettings polarAngle{};
			UnboundedDimensionSettings radius{};
			bool normalRotation{};
		};

		struct GeodesicPolyhedronSettings : FixedReplicationSettings {
			unsigned int subdivisions{};
			UnboundedDimensionSettings radius{};
			bool normalRotation{};
		};

		struct PathReplicationSettings {
			DimensionSettings distance{};
			bool applyRotation{ true };
		};

		union ReplicationSettings {
			GridReplicationSettings grid{};
			EllipsoidReplicationSettings ellipsoid;
			GeodesicPolyhedronSettings geodesicPolyhedron;
			PathReplicationSettings path;
		};
		
		enum class ComplexSource {
			SPAWN,
			CLIPBOARD,
		};

		using ObjectComplex = csl::ut::MoveArray<hh::game::ObjectData*>;

		class ObjectPool : CompatibleObject {
			ArrayTool& arrayTool;
			csl::ut::MoveArray<ObjectComplex> pool{ GetAllocator() };
		public:
			ObjectPool(csl::fnd::IAllocator* allocator, ArrayTool& arrayTool, csl::ut::MoveArray<ObjectComplex>&& objects);
			virtual ~ObjectPool();

			ObjectComplex Spawn(const Eigen::Affine3f& transform);
		};

		Clipboard<Context> clipboard{ GetAllocator(), GetContext() };
		csl::ut::MoveArray<ObjectComplex> tempObjects{ GetAllocator() };
		csl::ut::MoveArray<Eigen::Affine3f> complexElementTransforms{ GetAllocator() };
		csl::ut::VariableString pathName{ GetAllocator() };
		ComplexSource complexSource{};
		ReplicationType replicationType{};
		ReplicationSettings replicationSettings{};
		csl::math::Vector3 localPosition{};
		csl::math::Quaternion localRotation{};
		unsigned int maxObjects{ 200 };
		unsigned int curObjects{};
		bool snapToGround{};
		bool useClipboard{};

		bool RenderSettings();

		bool RenderComplexSourceSettings();
		bool RenderClipboardComplexSourceSettings();

		bool RenderReplicationTypeSettings();
		bool RenderGridReplicationTypeSettings();
		bool RenderEllipsoidReplicationTypeSettings();
		bool RenderGeodesicPolyhedronReplicationTypeSettings();
		bool RenderPathReplicationTypeSettings();
		static bool DimensionEditor(const char* label, DimensionSettings& settings);
		static bool UnboundedDimensionEditor(const char* label, UnboundedDimensionSettings& settings);

		void ResetCurObjects();
		void UpdateTempObjects();
		void ClearTempObjects();
		void RebuildTempObjects();

		template<typename F>
		void MaybeSpawnObject(ObjectPool& pool, F getTransform) {
			if (curObjects++ >= maxObjects)
				return;

			Eigen::Affine3f transform = getTransform() * Eigen::Translation3f{ localPosition } * localRotation;
			Eigen::Vector3f loc{ transform.translation() };

			if (snapToGround) {
				auto* physWorld = hh::game::GameManager::GetInstance()->GetService<hh::physics::PhysicsWorld>();

				hh::physics::PhysicsQueryResult result{};

				csl::ut::MoveArray<hh::physics::PhysicsQueryResult> results{ hh::fnd::MemoryRouter::GetTempAllocator() };

				if (physWorld->RayCastAllHits(loc, loc + Eigen::Vector3f{ 0.0f, -1000.0f, 0.0f }, 0xFFFFFFFF, results)) {
					for (auto& result : results) {
						if (result.collider->filterCategory == 10) {
							transform = Eigen::Translation3f{ result.hitLocation - loc } * transform;
							break;
						}
					}
				}
			}

			tempObjects.push_back(pool.Spawn(transform));
		}

		template<typename F>
		static void RepeatDimension(const DimensionSettings& settings, float max, F f, bool cyclic = false) {
			unsigned int repeat{ 1 };
			float interval{ 1.0f };
			float offset{ settings.offset };

			switch (settings.autoFillType) {
			case AutoFillType::NONE: repeat = settings.autoFillSettings.manual.repeat; interval = settings.autoFillSettings.manual.interval; break;
			case AutoFillType::REPEAT: interval = settings.autoFillSettings.repeat.interval; repeat = interval == 0.0f ? 1 : static_cast<unsigned int>(max / interval); break;
			case AutoFillType::SPREAD_INCLUSIVE: repeat = settings.autoFillSettings.spread.repeat; interval = repeat <= 1 ? 0.0f : max / static_cast<float>(repeat - (cyclic ? 0 : 1)); break;
			case AutoFillType::SPREAD_EXCLUSIVE: repeat = settings.autoFillSettings.spread.repeat; interval = max / static_cast<float>(repeat + (cyclic ? 0 : 1)); offset += interval; break;
			}

			for (unsigned int i = 0; i < repeat; i++)
				f(offset + static_cast<float>(i) * interval);
		}

		template<typename F>
		static void RepeatDimension(const UnboundedDimensionSettings& settings, F f, bool cyclic = false) {
			RepeatDimension(settings, settings.max, std::forward<F>(f), cyclic);
		}

		void GenerateGrid(ObjectPool& pool);
		void GenerateEllipsoid(ObjectPool& pool);
		void GenerateGeodesicPolyhedron(ObjectPool& pool);
		void GeneratePath(ObjectPool& pool);

		ObjectComplex SpawnComplex(const csl::math::Vector3& location);
		void PositionComplex(const Eigen::Affine3f& transform, const ObjectComplex& complex);
		void DeleteComplex(const ObjectComplex& complex);

		void InitializeReplicationType();
		void InitializeComplexSource();

		static void InitializeAutoFillType(ArrayTool::DimensionSettings& settings);

	public:
		ArrayTool(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode);
		~ArrayTool();
		virtual PanelTraits GetPanelTraits() const override;
		virtual void RenderPanel() override;
		virtual void ProcessAction(const ActionBase& action) override;
	};
}

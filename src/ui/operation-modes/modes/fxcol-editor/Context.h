#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::fxcol_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		//hh::fnd::Reference<app::gfx::ResFxCol2File> fxColFile;
		app::gfx::FxColBoundingVolumeData* placementVolume{};

		static app::gfx::FxColBoundingVolumeData* GetBoundingVolumeOfShape(app::gfx::FxColCollisionShapeData* collisionShape);
		void AddBoundingVolume();
		void RemoveBoundingVolume(app::gfx::FxColBoundingVolumeData* boundingVolume);
		app::gfx::FxColCollisionShapeData* AddCollisionShape(app::gfx::FxColBoundingVolumeData* boundingVolume, const Eigen::Vector3f& position);
		void RemoveCollisionShape(app::gfx::FxColCollisionShapeData* collisionShape);

		void RecalculateBoundingVolume(app::gfx::FxColBoundingVolumeData* boundingVolume);
		void RecalculateKdTree();

		static csl::geom::Aabb CalculateAabb(const app::gfx::FxColCollisionShapeData* collisionShape);
		static csl::geom::Aabb CalculateAabbForObb(const csl::math::Position& position, const csl::math::Rotation& rotation, const csl::math::Vector3& halfExtents);
	};
}

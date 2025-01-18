#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::fxcol_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		//hh::fnd::Reference<app::gfx::ResFxCol2File> fxColFile;
		ucsl::resources::fxcol::v1::KdTreeLeafData* placementVolume{};

		static ucsl::resources::fxcol::v1::KdTreeLeafData* GetBoundingVolumeOfShape(ucsl::resources::fxcol::v1::ShapeData* collisionShape);
		void AddBoundingVolume();
		void RemoveBoundingVolume(ucsl::resources::fxcol::v1::KdTreeLeafData* boundingVolume);
		ucsl::resources::fxcol::v1::ShapeData* AddCollisionShape(ucsl::resources::fxcol::v1::KdTreeLeafData* boundingVolume, const Eigen::Vector3f& position);
		void RemoveCollisionShape(ucsl::resources::fxcol::v1::ShapeData* collisionShape);

		void RecalculateBoundingVolume(ucsl::resources::fxcol::v1::KdTreeLeafData* boundingVolume);
		void RecalculateKdTree();

		static csl::geom::Aabb CalculateAabb(const app::gfx::FxColCollisionShapeData* collisionShape);
		static csl::geom::Aabb CalculateAabbForObb(const csl::math::Position& position, const csl::math::Rotation& rotation, const csl::math::Vector3& halfExtents);
	};
}

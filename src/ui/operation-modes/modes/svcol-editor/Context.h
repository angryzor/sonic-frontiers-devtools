#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::svcol_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<app::ResSvCol2> resource{};

		ucsl::resources::svcol::v1::ShapeData* AddShape(const Eigen::Vector3f& position);
		void RemoveShape(ucsl::resources::svcol::v1::ShapeData& shape);

		void RecalculateBoundingVolume(ucsl::resources::svcol::v1::ShapeData& shape);

		static csl::geom::Aabb CalculateAabb(const ucsl::resources::svcol::v1::ShapeData& shape);
		static csl::geom::Aabb CalculateAabbForObb(const csl::math::Position& position, const csl::math::Rotation& rotation, const csl::math::Vector3& halfExtents);
	};
}

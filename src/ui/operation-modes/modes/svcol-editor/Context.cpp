#include "Context.h"
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/BoundingBoxes.h>
#include <algorithm>

namespace ui::operation_modes::modes::svcol_editor {
	using namespace ucsl::resources::svcol::v1;

	ucsl::resources::svcol::v1::ShapeData* Context::AddShape(const Eigen::Vector3f& position) {
		auto& svColData = *resource->svColData;

		resources::ManagedCArray<ShapeData, unsigned int> shapes{ resource, svColData.shapes, svColData.shapeCount };

		auto& shape = shapes.emplace_back();
		shape.name = "Shape";
		shape.depth = 100.0f;
		shape.width = 100.0f;
		shape.height = 100.0f;
		shape.position = position;
		shape.rotation = csl::math::Rotation::Identity();

		RecalculateBoundingVolume(shape);

		return &shape;
	}

	void Context::RemoveShape(ucsl::resources::svcol::v1::ShapeData& shape) {
		auto& svColData = *resource->svColData;

		resources::ManagedCArray<ShapeData, unsigned int> shapes{ resource, svColData.shapes, svColData.shapeCount };

		shapes.remove(&shape - svColData.shapes);
	}

	void Context::RecalculateBoundingVolume(ucsl::resources::svcol::v1::ShapeData& shape) {
		auto aabb = CalculateAabb(shape);
		shape.aabbMin = aabb.min;
		shape.aabbMax = aabb.max;
	}

	csl::geom::Aabb Context::CalculateAabbForObb(const csl::math::Position& position, const csl::math::Rotation& rotation, const csl::math::Vector3& halfExtents) {
		csl::geom::Aabb aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
		Eigen::Affine3f transform{ Eigen::Translation3f{ position } *rotation };

		aabb.AddPoint(transform * -halfExtents);
		aabb.AddPoint(transform * halfExtents);
		aabb.AddPoint(transform * -csl::math::Vector3{ halfExtents.x(), halfExtents.y(), -halfExtents.z() });
		aabb.AddPoint(transform * csl::math::Vector3{ halfExtents.x(), halfExtents.y(), -halfExtents.z() });
		aabb.AddPoint(transform * -csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), halfExtents.z() });
		aabb.AddPoint(transform * csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), halfExtents.z() });
		aabb.AddPoint(transform * -csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), -halfExtents.z() });
		aabb.AddPoint(transform * csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), -halfExtents.z() });

		return aabb;
	}

	csl::geom::Aabb Context::CalculateAabb(const ShapeData& shape) {
		switch (shape.type) {
		case Shape::OBB: {
			return CalculateAabbForObb(shape.position, shape.rotation, { shape.width / 2, shape.height / 2, shape.depth / 2 });
		}
		default: return {};
		}
	}
}

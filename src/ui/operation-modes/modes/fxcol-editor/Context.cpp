#include "Context.h"
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/BoundingBoxes.h>
#include <algorithm>

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace ucsl::resources::fxcol::v1;

	KdTreeLeafData* Context::GetBoundingVolumeOfShape(ShapeData* collisionShape) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<KdTreeLeafData, unsigned int> kdTreeLeaves{ fxColManager->resource, fxColData->kdTreeLeaves, fxColData->kdTreeLeafCount };

		auto shapeIdx = collisionShape - fxColData->shapes;

		for (auto& volume : kdTreeLeaves)
			if (shapeIdx >= volume.shapeOffset && shapeIdx < volume.shapeOffset + static_cast<int>(volume.shapeCount))
				return &volume;

		return nullptr;
	}

	void Context::AddBoundingVolume() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<KdTreeLeafData, unsigned int> kdTreeLeaves{ fxColManager->resource, fxColData->kdTreeLeaves, fxColData->kdTreeLeafCount };

		kdTreeLeaves.push_back(KdTreeLeafData{
			0,
			static_cast<int>(fxColData->kdTreeLeafCount),
			{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() },
			{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() }
		});
	}

	void Context::RemoveBoundingVolume(KdTreeLeafData* kdTreeLeaf) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<KdTreeLeafData, unsigned int> kdTreeLeaves{ fxColManager->resource, fxColData->kdTreeLeaves, fxColData->kdTreeLeafCount };
		resources::ManagedCArray<ShapeData, unsigned int> shapes{ fxColManager->resource, fxColData->shapes, fxColData->shapeCount };

		for (unsigned int i = 0; i < kdTreeLeaf->shapeCount; i++)
			shapes.remove(kdTreeLeaf->shapeOffset);

		auto volumeIdx = kdTreeLeaf - fxColData->kdTreeLeaves;

		if (kdTreeLeaf->shapeCount > 0)
			for (auto& volume : kdTreeLeaves)
				if (volume.shapeOffset >= kdTreeLeaf->shapeOffset + static_cast<int>(kdTreeLeaf->shapeCount))
					volume.shapeOffset -= kdTreeLeaf->shapeCount;

		kdTreeLeaves.remove(volumeIdx);
	}

	ShapeData* Context::AddCollisionShape(KdTreeLeafData* kdTreeLeaf, const Eigen::Vector3f& position) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<KdTreeLeafData, unsigned int> kdTreeLeaves{ fxColManager->resource, fxColData->kdTreeLeaves, fxColData->kdTreeLeafCount };
		resources::ManagedCArray<ShapeData, unsigned int> shapes{ fxColManager->resource, fxColData->shapes, fxColData->shapeCount };

		auto shapeIdx = static_cast<unsigned int>(kdTreeLeaf->shapeOffset) + kdTreeLeaf->shapeCount;

		shapes.emplace_back();
		std::shift_right(shapes.begin() + shapeIdx, shapes.end(), 1);

		auto& shape = shapes[shapeIdx];
		shape.name = "Shape";
		shape.shape = ShapeData::Shape::SPHERE;
		shape.type = ShapeData::Type::SCENE_PARAMETER_INDEX;
		shape.unk1 = 0;
		shape.priority = 0;
		shape.extents.sphere.radius = 10.0f;
		shape.extents.sphere.borderThickness = 1.0f;
		shape.parameters.sceneParameterIndex.sceneParameterIndex = 0;
		shape.parameters.sceneParameterIndex.interpolationTime = 1.0f;
		shape.unk2 = nullptr;
		shape.position = position;
		shape.rotation = Eigen::Quaternionf::Identity();

		for (auto& volume : kdTreeLeaves)
			if (volume.shapeOffset >= shapeIdx && &volume != kdTreeLeaf)
				volume.shapeOffset++;

		kdTreeLeaf->shapeCount++;

		RecalculateBoundingVolume(kdTreeLeaf);

		return &shape;
	}

	void Context::RemoveCollisionShape(ShapeData* collisionShape) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<KdTreeLeafData, unsigned int> kdTreeLeaves{ fxColManager->resource, fxColData->kdTreeLeaves, fxColData->kdTreeLeafCount };
		resources::ManagedCArray<ShapeData, unsigned int> shapes{ fxColManager->resource, fxColData->shapes, fxColData->shapeCount };

		auto shapeIdx = collisionShape - fxColData->shapes;

		resources::ManagedMemoryRegistry::instance->GetManagedAllocator(fxColManager->resource).Free(const_cast<char*>(collisionShape->name));

		shapes.remove(shapeIdx);

		for (auto& volume : kdTreeLeaves)
			if (volume.shapeOffset > shapeIdx)
				volume.shapeOffset--;

		auto* kdTreeLeaf = GetBoundingVolumeOfShape(collisionShape);

		kdTreeLeaf->shapeCount--;

		RecalculateBoundingVolume(kdTreeLeaf);
	}

	void Context::RecalculateBoundingVolume(KdTreeLeafData* kdTreeLeaf) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		if (kdTreeLeaf->shapeCount == 0) {
			// I prefer to set it as inverse infinities but this is what the game initializes an empty aabb to.
			kdTreeLeaf->aabbMin = { 0.0f, 0.0f, 0.0f };
			kdTreeLeaf->aabbMax = { 0.0f, 0.0f, 0.0f };
		}
		else {
			csl::geom::Aabb aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

			for (int i = kdTreeLeaf->shapeOffset; i < kdTreeLeaf->shapeOffset + kdTreeLeaf->shapeCount; i++)
				AddAabb(aabb, CalculateAabb(&fxColData->shapes[i]));

			kdTreeLeaf->aabbMin = aabb.min;
			kdTreeLeaf->aabbMax = aabb.max;
		}

		RecalculateKdTree();
	}

	void Context::RecalculateKdTree() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();

		auto* oldResource = fxColManager->collisionTreeResource;

		auto* newTree = fxColManager->CreateKdTree(fxColManager->resource);
		auto* newResource = static_cast<hh::ut::ResKdTreeData*>(fxColManager->GetAllocator()->Alloc(hh::ut::KdTreeUtil::CalcMemorySize(*newTree), 16));
		hh::ut::KdTreeUtil::BuildResource(*newTree, newResource);

		fxColManager->collisionTree = newTree;
		fxColManager->collisionTreeResource = newResource;

		fxColManager->GetAllocator()->Free(oldResource);
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

	csl::geom::Aabb Context::CalculateAabb(const ShapeData* shape) {
		switch (shape->shape) {
		case ShapeData::Shape::SPHERE: {
			csl::geom::Aabb aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
			auto& extents = shape->extents.sphere;
			aabb.AddPoint(shape->position + csl::math::Vector3{ extents.radius, extents.radius, extents.radius });
			aabb.AddPoint(shape->position - csl::math::Vector3{ extents.radius, extents.radius, extents.radius });
			return aabb;
		}
		case ShapeData::Shape::CYLINDER: {
			auto& extents = shape->extents.cylinder;
			return CalculateAabbForObb(shape->position, shape->rotation, { extents.radius, extents.halfHeight, extents.radius });
		}
		case ShapeData::Shape::ANISOTROPIC_OBB: {
			auto& extents = shape->extents.anisotropicObb;
			return CalculateAabbForObb(shape->position, shape->rotation, { extents.width / 2, extents.height / 2, extents.depth / 2 });
			break;
		}
		case ShapeData::Shape::ISOTROPIC_OBB: {
			auto& extents = shape->extents.isotropicObb;
			return CalculateAabbForObb(shape->position, shape->rotation, { extents.width / 2, extents.height / 2, extents.depth / 2 });
		}
		}
	}
}

#include "Context.h"
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/BoundingBoxes.h>
#include <algorithm>

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace app::gfx;

	FxColBoundingVolume* Context::GetBoundingVolumeOfShape(FxColCollisionShape* collisionShape) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<FxColBoundingVolume, unsigned int> boundingVolumes{ fxColManager->resource, fxColData->boundingVolumes, fxColData->boundingVolumeCount };

		auto shapeIdx = collisionShape - fxColData->collisionShapes;

		for (auto& volume : boundingVolumes)
			if (shapeIdx >= volume.shapeStartIdx && shapeIdx < volume.shapeStartIdx + static_cast<int>(volume.shapeCount))
				return &volume;

		return nullptr;
	}

	void Context::AddBoundingVolume() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<FxColBoundingVolume, unsigned int> boundingVolumes{ fxColManager->resource, fxColData->boundingVolumes, fxColData->boundingVolumeCount };

		boundingVolumes.push_back(FxColBoundingVolume{
			0,
			static_cast<int>(fxColData->boundingVolumeCount),
			{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() },
			{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() }
		});
	}

	void Context::RemoveBoundingVolume(FxColBoundingVolume* boundingVolume) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<FxColBoundingVolume, unsigned int> boundingVolumes{ fxColManager->resource, fxColData->boundingVolumes, fxColData->boundingVolumeCount };
		resources::ManagedCArray<FxColCollisionShape, unsigned int> collisionShapes{ fxColManager->resource, fxColData->collisionShapes, fxColData->collisionShapeCount };

		for (unsigned int i = 0; i < boundingVolume->shapeCount; i++)
			collisionShapes.remove(boundingVolume->shapeStartIdx);

		auto volumeIdx = boundingVolume - fxColData->boundingVolumes;

		if (boundingVolume->shapeCount > 0)
			for (auto& volume : boundingVolumes)
				if (volume.shapeStartIdx >= boundingVolume->shapeStartIdx + static_cast<int>(boundingVolume->shapeCount))
					volume.shapeStartIdx -= boundingVolume->shapeCount;

		boundingVolumes.remove(volumeIdx);
	}

	FxColCollisionShape* Context::AddCollisionShape(FxColBoundingVolume* boundingVolume, const Eigen::Vector3f& position) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<FxColBoundingVolume, unsigned int> boundingVolumes{ fxColManager->resource, fxColData->boundingVolumes, fxColData->boundingVolumeCount };
		resources::ManagedCArray<FxColCollisionShape, unsigned int> collisionShapes{ fxColManager->resource, fxColData->collisionShapes, fxColData->collisionShapeCount };

		auto shapeIdx = static_cast<unsigned int>(boundingVolume->shapeStartIdx) + boundingVolume->shapeCount;

		collisionShapes.emplace_back();
		std::shift_right(collisionShapes.begin() + shapeIdx, collisionShapes.end(), 1);

		auto& shape = collisionShapes[shapeIdx];
		shape.name = "Shape";
		shape.shape = FxColCollisionShape::Shape::SPHERE;
		shape.type = FxColCollisionShape::Type::SCENE_PARAMETER_INDEX;
		shape.unk1 = 0;
		shape.priority = 0;
		shape.extents.sphere.radius = 10.0f;
		shape.extents.sphere.borderThickness = 1.0f;
		shape.parameters.sceneParameterIndex.sceneParameterIndex = 0;
		shape.parameters.sceneParameterIndex.interpolationTime = 1.0f;
		shape.unk2 = nullptr;
		shape.position = position;
		shape.rotation = Eigen::Quaternionf::Identity();

		for (auto& volume : boundingVolumes)
			if (volume.shapeStartIdx >= shapeIdx && &volume != boundingVolume)
				volume.shapeStartIdx++;

		boundingVolume->shapeCount++;

		RecalculateBoundingVolume(boundingVolume);

		return &shape;
	}

	void Context::RemoveCollisionShape(FxColCollisionShape* collisionShape) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		resources::ManagedCArray<FxColBoundingVolume, unsigned int> boundingVolumes{ fxColManager->resource, fxColData->boundingVolumes, fxColData->boundingVolumeCount };
		resources::ManagedCArray<FxColCollisionShape, unsigned int> collisionShapes{ fxColManager->resource, fxColData->collisionShapes, fxColData->collisionShapeCount };

		auto shapeIdx = collisionShape - fxColData->collisionShapes;

		resources::ManagedMemoryRegistry::instance->GetManagedAllocator(fxColManager->resource).Free(const_cast<char*>(collisionShape->name));

		collisionShapes.remove(shapeIdx);

		for (auto& volume : boundingVolumes)
			if (volume.shapeStartIdx > shapeIdx)
				volume.shapeStartIdx--;

		auto* boundingVolume = GetBoundingVolumeOfShape(collisionShape);

		boundingVolume->shapeCount--;

		RecalculateBoundingVolume(boundingVolume);
	}

	void Context::RecalculateBoundingVolume(app::gfx::FxColBoundingVolume* boundingVolume) {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();
		auto* fxColData = fxColManager->resource->fxColData;

		if (boundingVolume->shapeCount == 0) {
			// I prefer to set it as inverse infinities but this is what the game initializes an empty aabb to.
			boundingVolume->aabbMin = { 0.0f, 0.0f, 0.0f };
			boundingVolume->aabbMax = { 0.0f, 0.0f, 0.0f };
		}
		else {
			csl::geom::Aabb aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

			for (int i = boundingVolume->shapeStartIdx; i < boundingVolume->shapeStartIdx + boundingVolume->shapeCount; i++)
				AddAabb(aabb, CalculateAabb(&fxColData->collisionShapes[i]));

			boundingVolume->aabbMin = aabb.min;
			boundingVolume->aabbMax = aabb.max;
		}

		RecalculateKdTree();
	}

	void Context::RecalculateKdTree() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();

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

	csl::geom::Aabb Context::CalculateAabb(const app::gfx::FxColCollisionShape* shape) {
		switch (shape->shape) {
		case FxColCollisionShape::Shape::SPHERE: {
			csl::geom::Aabb aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
			auto& extents = shape->extents.sphere;
			aabb.AddPoint(shape->position + csl::math::Vector3{ extents.radius, extents.radius, extents.radius });
			aabb.AddPoint(shape->position - csl::math::Vector3{ extents.radius, extents.radius, extents.radius });
			return aabb;
		}
		case FxColCollisionShape::Shape::CYLINDER: {
			auto& extents = shape->extents.cylinder;
			return CalculateAabbForObb(shape->position, shape->rotation, { extents.radius, extents.halfHeight, extents.radius });
		}
		case FxColCollisionShape::Shape::ANISOTROPIC_OBB: {
			auto& extents = shape->extents.anisotropicObb;
			return CalculateAabbForObb(shape->position, shape->rotation, { extents.width / 2, extents.height / 2, extents.depth / 2 });
			break;
		}
		case FxColCollisionShape::Shape::ISOTROPIC_OBB: {
			auto& extents = shape->extents.isotropicObb;
			return CalculateAabbForObb(shape->position, shape->rotation, { extents.width / 2, extents.height / 2, extents.depth / 2 });
		}
		}
	}
}

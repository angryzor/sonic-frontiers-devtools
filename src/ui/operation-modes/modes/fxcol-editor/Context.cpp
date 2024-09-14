#include "Context.h"
#include <resources/managed-memory/ManagedCArray.h>
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
		shape.unk11 = 0;
		shape.unk12 = 0;
		shape.unk13 = 0;
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

		boundingVolume->aabbMin = { -INFINITY, -INFINITY, -INFINITY };
		boundingVolume->aabbMax = { INFINITY, INFINITY, INFINITY };
	}
}

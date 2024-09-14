#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::fxcol_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		//hh::fnd::Reference<app::gfx::ResFxCol2File> fxColFile;
		app::gfx::FxColBoundingVolume* placementVolume{};

		static app::gfx::FxColBoundingVolume* GetBoundingVolumeOfShape(app::gfx::FxColCollisionShape* collisionShape);
		void AddBoundingVolume();
		void RemoveBoundingVolume(app::gfx::FxColBoundingVolume* boundingVolume);
		app::gfx::FxColCollisionShape* AddCollisionShape(app::gfx::FxColBoundingVolume* boundingVolume, const Eigen::Vector3f& position);
		void RemoveCollisionShape(app::gfx::FxColCollisionShape* collisionShape);

		void RecalculateBoundingVolume(app::gfx::FxColBoundingVolume* boundingVolume);
	};
}

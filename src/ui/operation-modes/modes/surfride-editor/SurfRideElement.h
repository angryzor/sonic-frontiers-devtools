#pragma once

namespace ui::operation_modes::modes::surfride_editor {
	struct SurfRideElement {
		enum class Type {
			SCENE,
			CAMERA_DATA,
			LAYER,
			CAST,
		};

		Type type;
		union {
			SurfRide::Scene* scene;
			SurfRide::SRS_CAMERA* cameraData;
			SurfRide::Layer* layer;
			SurfRide::Cast* cast;
		};

		SurfRideElement(Type type, SurfRide::Scene* scene);
		SurfRideElement(Type type, SurfRide::SRS_CAMERA* cameraData);
		SurfRideElement(Type type, SurfRide::Layer* layer);
		SurfRideElement(Type type, SurfRide::Cast* cast);

		bool operator==(const SurfRideElement& other) const;
		const char* GetName() const;
	};
}

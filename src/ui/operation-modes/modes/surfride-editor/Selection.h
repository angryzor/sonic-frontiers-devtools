#pragma once

struct SurfRideSelection {
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

	SurfRideSelection(Type type, SurfRide::Scene* scene);
	SurfRideSelection(Type type, SurfRide::SRS_CAMERA* cameraData);
	SurfRideSelection(Type type, SurfRide::Layer* layer);
	SurfRideSelection(Type type, SurfRide::Cast* cast);

	bool operator==(const SurfRideSelection& other) const;
	const char* GetName() const;
};

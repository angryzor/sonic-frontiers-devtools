#include "Selection.h"

using namespace SurfRide;

SurfRideSelection::SurfRideSelection(Type type, Scene* scene) : type{ type }, scene{ scene } {}
SurfRideSelection::SurfRideSelection(Type type, SRS_CAMERA* cameraData) : type{ type }, cameraData{ cameraData } {}
SurfRideSelection::SurfRideSelection(Type type, Layer* layer) : type{ type }, layer{ layer } {}
SurfRideSelection::SurfRideSelection(Type type, Cast* cast) : type{ type }, cast{ cast } {}

bool SurfRideSelection::operator==(const SurfRideSelection& other) const
{
	if (type != other.type)
		return false;

	switch (type) {
	case Type::SCENE: return scene == other.scene;
	case Type::CAMERA_DATA: return cameraData == other.cameraData;
	case Type::LAYER: return layer == other.layer;
	case Type::CAST: return cast == other.cast;
	default: return true;
	}
}

const char* SurfRideSelection::GetName() const
{
	switch (type) {
	case Type::SCENE: return scene->sceneData->name;
	case Type::CAMERA_DATA: return cameraData->name;
	case Type::LAYER: return layer->layerData->name;
	case Type::CAST: return cast->castData->name;
	default: return nullptr;
	}
}

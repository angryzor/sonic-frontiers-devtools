#include "SurfRideElement.h"

namespace ui::operation_modes::modes::surfride_editor {
	using namespace SurfRide;

	SurfRideElement::SurfRideElement(Type type, Scene* scene) : type{ type }, scene{ scene } {}
	SurfRideElement::SurfRideElement(Type type, SRS_CAMERA* cameraData) : type{ type }, cameraData{ cameraData } {}
	SurfRideElement::SurfRideElement(Type type, Layer* layer) : type{ type }, layer{ layer } {}
	SurfRideElement::SurfRideElement(Type type, Cast* cast) : type{ type }, cast{ cast } {}
	SurfRideElement::SurfRideElement(Type type, SurfRide::SRS_TEXTURE* texture) : type{ type }, texture{ texture } {}

	bool SurfRideElement::operator==(const SurfRideElement& other) const
	{
		if (type != other.type)
			return false;

		switch (type) {
		case Type::SCENE: return scene == other.scene;
		case Type::CAMERA_DATA: return cameraData == other.cameraData;
		case Type::LAYER: return layer == other.layer;
		case Type::CAST: return cast == other.cast;
		case Type::TEXTURE: return texture == other.texture;
		default: return true;
		}
	}

	const char* SurfRideElement::GetName() const
	{
		switch (type) {
		case Type::SCENE: return scene->sceneData->name;
		case Type::CAMERA_DATA: return cameraData->name;
		case Type::LAYER: return layer->layerData->name;
		case Type::CAST: return cast->castData->name;
		case Type::TEXTURE: return texture->name;
		default: return nullptr;
		}
	}
}

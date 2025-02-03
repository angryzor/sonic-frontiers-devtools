#include "SurfRideElement.h"

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::v6;

	SurfRideElement::SurfRideElement(Type type, unsigned int id) : type{ type }, id{ id } {}
	SurfRideElement::SurfRideElement(const SRS_SCENE& scene) : SurfRideElement{ Type::SCENE, scene.id } {}
	SurfRideElement::SurfRideElement(const SRS_CAMERA& cameraData) : SurfRideElement{ Type::CAMERA, cameraData.id } {}
	SurfRideElement::SurfRideElement(const SRS_LAYER& layer) : SurfRideElement{ Type::LAYER, layer.id } {}
	SurfRideElement::SurfRideElement(const SRS_CASTNODE& cast) : SurfRideElement{ Type::CAST, cast.id } {}
	//SurfRideElement::SurfRideElement(const SRS_TEXTURE& texture) : SurfRideElement{ Type::TEXTURE, texture.id } {}

	bool SurfRideElement::operator==(const SurfRideElement& other) const {
		return type == other.type && id == other.id;
	}
}

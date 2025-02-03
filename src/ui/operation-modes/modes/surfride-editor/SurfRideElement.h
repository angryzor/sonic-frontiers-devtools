#pragma once

namespace ui::operation_modes::modes::surfride_editor {
	struct SurfRideElement {
		enum class Type {
			SCENE,
			CAMERA,
			LAYER,
			CAST,
			//TEXTURE,
		};

		Type type;
		unsigned int id;
		
		SurfRideElement(Type type, unsigned int id);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_SCENE& scene);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_CAMERA& cameraData);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_LAYER& layer);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		//SurfRideElement(const ucsl::resources::swif::v6::SRS_TEXTURE& texture);

		bool operator==(const SurfRideElement& other) const;
	};
}

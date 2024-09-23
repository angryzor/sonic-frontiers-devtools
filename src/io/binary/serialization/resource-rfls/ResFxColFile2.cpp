#include "ResFxColFile2.h"

namespace hh::fnd {
	//template<> struct RflClassTraits<app::gfx::FxColCollisionShape::SphereExtents> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::SphereExtents> {};
	//template<> struct RflClassTraits<app::gfx::FxColCollisionShape::CylinderExtents> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::CylinderExtents> {};
	template<> struct RflClassTraits<app::gfx::FxColCollisionShape::AnisotropicObbExtents> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::AnisotropicObbExtents> {};
	//template<> struct RflClassTraits<app::gfx::FxColCollisionShape::IsotropicObbExtents> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::IsotropicObbExtents> {};
	//template<> struct RflClassTraits<app::gfx::FxColCollisionShape::SceneParameterIndexParameters> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::SceneParameterIndexParameters> {};
	//template<> struct RflClassTraits<app::gfx::FxColCollisionShape::LightParameterIndexParameters> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::LightParameterIndexParameters> {};
	//template<> struct RflClassTraits<app::gfx::FxColCollisionShape::HeatHazeParameters> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::HeatHazeParameters> {};
	template<> struct RflClassTraits<app::gfx::FxColCollisionShape::CameraParameters> : RflClassTraitsImpl<app::gfx::FxColCollisionShape::CameraParameters> {};
	template<> struct RflClassTraits<app::gfx::FxColCollisionShape> : RflClassTraitsImpl<app::gfx::FxColCollisionShape> {};
	template<> struct RflClassTraits<app::gfx::FxColBoundingVolume> : RflClassTraitsImpl<app::gfx::FxColBoundingVolume> {};
	template<> struct RflClassTraits<app::gfx::FxColUnk1> : RflClassTraitsImpl<app::gfx::FxColUnk1> {};
}

#include <reflection/RflClassGen.h>

using namespace hh::fnd;
using namespace app::gfx;

namespace reflection::serialization::resource_rfls::fx_col_file {
	//auto sphereExtentsMembers = CreateRflClassMembers<void,
	//	float,
	//	float
	//>(
	//	"radius",
	//	"borderThickness"
	//);

	//auto cylinderExtentsMembers = CreateRflClassMembers<void,
	//	float,
	//	float,
	//	float
	//>(
	//	"radius",
	//	"halfHeight",
	//	"borderThickness"
	//);

	auto anisotropicObbExtentsMembers = CreateRflClassMembers<void,
		float,
		float,
		float,
		float,
		float,
		float
	>(
		"depth",
		"width",
		"height",
		"maybeWidthAndHeightBorderThickness",
		"positiveDepthBorderThickness",
		"negativeDepthBorderThickness"
	);

	//auto isotropicObbExtentsMembers = CreateRflClassMembers<void,
	//	float,
	//	float,
	//	float,
	//	float
	//>(
	//	"depth",
	//	"width",
	//	"height",
	//	"borderThickness"
	//);

	//auto sceneParameterIndexParametersMembers = CreateRflClassMembers<void, unsigned int, float>("sceneParameterIndex", "interpolationTime");
	//auto lightParameterIndexParametersMembers = CreateRflClassMembers<void, unsigned int>("lightParameterIndex");
	//auto heatHazeParametersMembers = CreateRflClassMembers<void, float, float>("heatHazeMaxHeight", "interpolationTime");

	auto cameraParametersMembers = CreateRflClassMembers<void,
		uint32_t,
		uint32_t,
		float,
		float,
		uint32_t,
		float,
		float,
		float
	>(
		"unk1",
		"unk2",
		"bloomScale",
		"autoExposureMiddleGray",
		"unk3",
		"luminance",
		"autoExposureAdaptedRatio",
		"interpolationTime"
	);

	auto fxColCollisionShape = CreateRflClassMembers<void,
		const char*,
		uint8_t,
		uint8_t,
		uint8_t,
		uint8_t,
		FxColCollisionShape::AnisotropicObbExtents,
		FxColCollisionShape::CameraParameters,
		const char*,
		csl::math::Position,
		float,
		float,
		float,
		float
	>(
		"name",
		"shape",
		"type",
		"unk1",
		"priority",
		"extents",
		"parameters",
		"unk2",
		"position",
		"rotationX",
		"rotationY",
		"rotationW",
		"rotationZ"
	);

	auto fxColBoundingVolumeMembers = CreateRflClassMembers<void,
		unsigned int,
		int,
		csl::math::Position,
		csl::math::Position
	>(
		"shapeCount",
		"shapeStartIdx",
		"aabbMin",
		"aabbMax"
	);

	auto fxColUnk1Members = CreateRflClassMembers<void,
		int,
		int
	>(
		"unk1",
		"unk2"
	);

	auto fxColDataMembers = CreateRflClassMembers<void,
		unsigned int,
		unsigned int,
		unsigned int,
		RflDynamicPointerArray<FxColCollisionShape, [](const void* parent) -> size_t { return static_cast<const FxColData*>(parent)->collisionShapeCount; }>,
		unsigned int,
		RflDynamicPointerArray<FxColBoundingVolume, [](const void* parent) -> size_t { return static_cast<const FxColData*>(parent)->boundingVolumeCount; }>,
		unsigned int,
		RflDynamicPointerArray<FxColUnk1, [](const void* parent) -> size_t { return static_cast<const FxColData*>(parent)->unk1Count; }>
	>(
		"magic",
		"version",
		"collisionShapeCount",
		"collisionShapes",
		"boundingVolumeCount",
		"boundingVolumes",
		"unk1Count",
		"unk1s"
	);
}

using namespace reflection::serialization::resource_rfls::fx_col_file;

//const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::SphereExtents>::rflClass{ "FxColSphereExtents", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::SphereExtents)), nullptr, 0, sphereExtentsMembers.data(), static_cast<uint32_t>(sphereExtentsMembers.size()), nullptr };
//const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::CylinderExtents>::rflClass{ "FxColCylinderExtents", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::CylinderExtents)), nullptr, 0, cylinderExtentsMembers.data(), static_cast<uint32_t>(cylinderExtentsMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::AnisotropicObbExtents>::rflClass{ "FxColAnisotropicObbExtents", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::AnisotropicObbExtents)), nullptr, 0, anisotropicObbExtentsMembers.data(), static_cast<uint32_t>(anisotropicObbExtentsMembers.size()), nullptr };
//const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::IsotropicObbExtents>::rflClass{ "FxColIsotropicObbExtents", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::IsotropicObbExtents)), nullptr, 0, isotropicObbExtentsMembers.data(), static_cast<uint32_t>(isotropicObbExtentsMembers.size()), nullptr };
//const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::SceneParameterIndexParameters>::rflClass{ "FxColSceneParameterIndexParameters", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::SceneParameterIndexParameters)), nullptr, 0, sceneParameterIndexParametersMembers.data(), static_cast<uint32_t>(sceneParameterIndexParametersMembers.size()), nullptr };
//const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::LightParameterIndexParameters>::rflClass{ "FxColLightParameterIndexParameters", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::LightParameterIndexParameters)), nullptr, 0, lightParameterIndexParametersMembers.data(), static_cast<uint32_t>(lightParameterIndexParametersMembers.size()), nullptr };
//const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::HeatHazeParameters>::rflClass{ "FxColHeatHazeParameters", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::HeatHazeParameters)), nullptr, 0, heatHazeParametersMembers.data(), static_cast<uint32_t>(heatHazeParametersMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape::CameraParameters>::rflClass{ "FxColCameraParameters", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape::CameraParameters)), nullptr, 0, cameraParametersMembers.data(), static_cast<uint32_t>(cameraParametersMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<app::gfx::FxColCollisionShape>::rflClass{ "FxColCollisionShape", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColCollisionShape)), nullptr, 0, fxColCollisionShape.data(), static_cast<uint32_t>(fxColCollisionShape.size()), nullptr };
const RflClass RflClassTraitsImpl<app::gfx::FxColBoundingVolume>::rflClass{ "FxColBoundingVolume", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColBoundingVolume)), nullptr, 0, fxColBoundingVolumeMembers.data(), static_cast<uint32_t>(fxColBoundingVolumeMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<app::gfx::FxColUnk1>::rflClass{ "FxColUnk1", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColUnk1)), nullptr, 0, fxColUnk1Members.data(), static_cast<uint32_t>(fxColUnk1Members.size()), nullptr };
const RflClass RflClassTraitsImpl<app::gfx::FxColData>::rflClass{ "FxColData", nullptr, static_cast<uint32_t>(sizeof(app::gfx::FxColData)), nullptr, 0, fxColDataMembers.data(), static_cast<uint32_t>(fxColDataMembers.size()), nullptr };

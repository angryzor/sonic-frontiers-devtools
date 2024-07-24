#include "ResSurfRideProject.h"

namespace SurfRide {
	struct SRS_DATA_BOOL : SRS_DATA {};
	struct SRS_DATA_INT : SRS_DATA {};
	struct SRS_DATA_UINT : SRS_DATA {};
	struct SRS_DATA_FLOAT : SRS_DATA {};
	struct SRS_DATA_STRING : SRS_DATA {};
}

namespace hh::fnd {
	template<> struct RflClassTraits<SurfRide::Rotation3> : RflClassTraitsImpl<SurfRide::Rotation3> {};
	template<> struct RflClassTraits<SurfRide::SRS_SCENE> : RflClassTraitsImpl<SurfRide::SRS_SCENE> {};
	template<> struct RflClassTraits<SurfRide::SRS_CAMERA> : RflClassTraitsImpl<SurfRide::SRS_CAMERA> {};
	template<> struct RflClassTraits<SurfRide::SRS_LAYER> : RflClassTraitsImpl<SurfRide::SRS_LAYER> {};
	template<> struct RflClassTraits<SurfRide::SRS_TRS_BASE> : RflClassTraitsImpl<SurfRide::SRS_TRS_BASE> {};
	template<> struct RflClassTraits<SurfRide::SRS_TRS2D> : RflClassTraitsImpl<SurfRide::SRS_TRS2D> {};
	template<> struct RflClassTraits<SurfRide::SRS_TRS3D> : RflClassTraitsImpl<SurfRide::SRS_TRS3D> {};
	template<> struct RflClassTraits<SurfRide::SRS_SLICE> : RflClassTraitsImpl<SurfRide::SRS_SLICE> {};
	template<> struct RflClassTraits<SurfRide::SRS_TEXTDATA> : RflClassTraitsImpl<SurfRide::SRS_TEXTDATA> {};
	template<> struct RflClassTraits<SurfRide::SRS_CASTNODE> : RflClassTraitsImpl<SurfRide::SRS_CASTNODE> {};
	template<> struct RflClassTraits<SurfRide::SRS_IMAGECAST> : RflClassTraitsImpl<SurfRide::SRS_IMAGECAST> {};
	template<> struct RflClassTraits<SurfRide::SRS_REFERENCECAST> : RflClassTraitsImpl<SurfRide::SRS_REFERENCECAST> {};
	template<> struct RflClassTraits<SurfRide::SRS_SLICECAST> : RflClassTraitsImpl<SurfRide::SRS_SLICECAST> {};
	template<> struct RflClassTraits<SurfRide::SRS_CROP> : RflClassTraitsImpl<SurfRide::SRS_CROP> {};
	template<> struct RflClassTraits<SurfRide::SRS_CROPREF> : RflClassTraitsImpl<SurfRide::SRS_CROPREF> {};
	template<> struct RflClassTraits<SurfRide::SRS_TEXTURE> : RflClassTraitsImpl<SurfRide::SRS_TEXTURE> {};
	template<> struct RflClassTraits<SurfRide::SRS_KEYFRAME> : RflClassTraitsImpl<SurfRide::SRS_KEYFRAME> {};
	template<> struct RflClassTraits<SurfRide::KeyLinear<float>> : RflClassTraitsImpl<SurfRide::KeyLinear<float>> {};
	//template<> struct RflClassTraits<SurfRide::KeyLinear<double>> : RflClassTraitsImpl<SurfRide::KeyLinear<double>> {};
	template<> struct RflClassTraits<SurfRide::KeyLinear<int>> : RflClassTraitsImpl<SurfRide::KeyLinear<int>> {};
	template<> struct RflClassTraits<SurfRide::KeyLinear<bool>> : RflClassTraitsImpl<SurfRide::KeyLinear<bool>> {};
	template<> struct RflClassTraits<SurfRide::KeyLinear<SurfRide::Color>> : RflClassTraitsImpl<SurfRide::KeyLinear<SurfRide::Color>> {};
	template<> struct RflClassTraits<SurfRide::KeyHermite<float>> : RflClassTraitsImpl<SurfRide::KeyHermite<float>> {};
	//template<> struct RflClassTraits<SurfRide::KeyHermite<double>> : RflClassTraitsImpl<SurfRide::KeyHermite<double>> {};
	template<> struct RflClassTraits<SurfRide::KeyHermite<int>> : RflClassTraitsImpl<SurfRide::KeyHermite<int>> {};
	template<> struct RflClassTraits<SurfRide::KeyHermite<bool>> : RflClassTraitsImpl<SurfRide::KeyHermite<bool>> {};
	template<> struct RflClassTraits<SurfRide::KeyHermite<SurfRide::Color>> : RflClassTraitsImpl <SurfRide::KeyHermite<SurfRide::Color>> {};
	template<> struct RflClassTraits<SurfRide::KeyIndividual<float>> : RflClassTraitsImpl<SurfRide::KeyIndividual<float>> {};
	//template<> struct RflClassTraits<SurfRide::KeyIndividual<double>> : RflClassTraitsImpl<SurfRide::KeyIndividual<double>> {};
	template<> struct RflClassTraits<SurfRide::KeyIndividual<int>> : RflClassTraitsImpl<SurfRide::KeyIndividual<int>> {};
	template<> struct RflClassTraits<SurfRide::KeyIndividual<bool>> : RflClassTraitsImpl<SurfRide::KeyIndividual<bool>> {};
	template<> struct RflClassTraits<SurfRide::KeyIndividual<SurfRide::Color>> : RflClassTraitsImpl <SurfRide::KeyIndividual<SurfRide::Color>> {};
	template<> struct RflClassTraits<SurfRide::SRS_TRACK> : RflClassTraitsImpl<SurfRide::SRS_TRACK> {};
	template<> struct RflClassTraits<SurfRide::SRS_MOTION> : RflClassTraitsImpl<SurfRide::SRS_MOTION> {};
	template<> struct RflClassTraits<SurfRide::SRS_ANIMATION> : RflClassTraitsImpl<SurfRide::SRS_ANIMATION> {};
	template<> struct RflClassTraits<SurfRide::SRS_FONT> : RflClassTraitsImpl<SurfRide::SRS_FONT> {};
	template<> struct RflClassTraits<SurfRide::SRS_CHARACTER_MAPPING> : RflClassTraitsImpl<SurfRide::SRS_CHARACTER_MAPPING> {};
	template<> struct RflClassTraits<SurfRide::SRS_DATA> : RflClassTraitsImpl<SurfRide::SRS_DATA> {};
	template<> struct RflClassTraits<SurfRide::SRS_DATA_BOOL> : RflClassTraitsImpl<SurfRide::SRS_DATA_BOOL> {};
	template<> struct RflClassTraits<SurfRide::SRS_DATA_INT> : RflClassTraitsImpl<SurfRide::SRS_DATA_INT> {};
	template<> struct RflClassTraits<SurfRide::SRS_DATA_UINT> : RflClassTraitsImpl<SurfRide::SRS_DATA_UINT> {};
	template<> struct RflClassTraits<SurfRide::SRS_DATA_FLOAT> : RflClassTraitsImpl<SurfRide::SRS_DATA_FLOAT> {};
	template<> struct RflClassTraits<SurfRide::SRS_DATA_STRING> : RflClassTraitsImpl<SurfRide::SRS_DATA_STRING> {};
	template<> struct RflClassTraits<SurfRide::SRS_USERDATA> : RflClassTraitsImpl<SurfRide::SRS_USERDATA> {};
	template<> struct RflClassTraits<SurfRide::SRS_BLUR> : RflClassTraitsImpl<SurfRide::SRS_BLUR> {};
	template<> struct RflClassTraits<SurfRide::SRS_BLUR3D> : RflClassTraitsImpl<SurfRide::SRS_BLUR3D> {};
	template<> struct RflClassTraits<SurfRide::SRS_REFLECT> : RflClassTraitsImpl<SurfRide::SRS_REFLECT> {};
	template<> struct RflClassTraits<SurfRide::SRS_REFLECT3D> : RflClassTraitsImpl<SurfRide::SRS_REFLECT3D> {};
}

#include <utilities/RflClassGen.h>

using namespace hh::fnd;
using namespace SurfRide;

namespace reflection::serialization::resource_rfls::surfride {
	auto surfRideRotation3Members = CreateRflClassMembers<void, int, int, int>("x", "y", "z");

	auto srsProjectMembers = CreateRflClassMembers<void,
		const char*,
		unsigned short,
		unsigned short,
		unsigned short,
		unsigned short,
		RflDynamicPointerArray<SRS_SCENE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_PROJECT*>(parent)->sceneCount; }>,
		RflDynamicPointerArray<SRS_TEXTURELIST, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_PROJECT*>(parent)->textureListCount; }>,
		RflDynamicPointerArray<SRS_FONT, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_PROJECT*>(parent)->fontCount; }>,
		SRS_CAMERA,
		int,
		int,
		float,
		SRS_USERDATA*
	>(
		"name",
		"sceneCount",
		"unk1",
		"textureListCount",
		"fontCount",
		"scenes",
		"textureLists",
		"fonts",
		"camera",
		"startFrame",
		"endFrame",
		"frameRate",
		"userData"
	);

	auto srsSceneMembers = CreateRflClassMembers<void,
		const char*,
		int,
		int,
		int,
		unsigned int,
		RflDynamicPointerArray<SRS_LAYER, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_SCENE*>(parent)->layerCount; }>,
		unsigned short,
		unsigned short,
		RflDynamicPointerArray<SRS_CAMERA, [](const void* parent) -> size_t {  return static_cast<const SurfRide::SRS_SCENE*>(parent)->cameraCount; }>,
		int,
		Vector2,
		SRS_USERDATA*
	>(
		"name",
		"id",
		"flags",
		"unk1",
		"layerCount",
		"layers",
		"cameraCount",
		"currentCameraIndex",
		"cameras",
		"backgroundColor",
		"resolution",
		"userData"
	);

	auto srsCameraMembers = CreateRflClassMembers<void,
		const char*,
		int,
		csl::math::Vector3,
		csl::math::Vector3,
		bool,
		int,
		float,
		float,
		float,
		uint64_t
	>(
		"name",
		"id",
		"position",
		"target",
		"isOrthogonal",
		"fov",
		"nearClip",
		"farClip",
		"unk1",
		"unk2"
	);

	auto srsLayerMembers = CreateRflClassMembers<void,
		const char*,
		int,
		int,
		unsigned int,
		RflDynamicPointerArray<SRS_CASTNODE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_LAYER*>(parent)->castCount; }>,
		RflDynamicStructPointerArray<
			[](const void* parent) { return static_cast<const SurfRide::SRS_LAYER*>(parent)->flags & 0x1 ? &RflClassTraits<SurfRide::SRS_TRS3D>::rflClass : &RflClassTraits<SurfRide::SRS_TRS2D>::rflClass; },
			[](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_LAYER*>(parent)->castCount; }
		>,
		int,
		RflDynamicPointerArray<SRS_ANIMATION, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_LAYER*>(parent)->animationCount; }>,
		int,
		SRS_USERDATA*
	>(
		"name",
		"id",
		"flags",
		"castCount",
		"casts",
		"transforms",
		"animationCount",
		"animations",
		"currentAnimationIndex",
		"userData"
	);

	auto srsTrsBaseMembers = CreateRflClassMembers<void,
		SurfRide::Color,
		SurfRide::Color,
		bool
	>(
		"materialColor",
		"illuminationColor",
		"display"
	);

	auto srsTrs2DMembers = CreateRflClassMembers<SurfRide::SRS_TRS_BASE,
		Vector2,
		uint32_t,
		Vector2,
		uint32_t,
		uint32_t
	>(
		"position",
		"rotation",
		"scale",
		"unkX",
		"unkY"
	);

	auto srsTrs3DMembers = CreateRflClassMembers<SurfRide::SRS_TRS_BASE,
		Vector3,
		Rotation3,
		Vector3
	>(
		"position",
		"rotation",
		"scale"
	);

	auto srsSliceMembers = CreateRflClassMembers<void,
		uint32_t,
		Vector2,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		short,
		short
	>(
		"flags",
		"fixedSize",
		"materialColor",
		"illuminationColor",
		"vertexColorTopLeft",
		"vertexColorBottomLeft",
		"vertexColorTopRight",
		"vertexColorBottomRight",
		"cropIndex0",
		"cropIndex1"
	);

	auto srsCastNodeMembers = CreateRflClassMembers<void,
		const char*,
		int,
		int,
		RflDynamicStructPointer<[](const void* parent) -> const hh::fnd::RflClass* {
			switch (static_cast<const SurfRide::SRS_CASTNODE*>(parent)->GetType()) {
			case SurfRide::SRS_CASTNODE::Type::NORMAL: return nullptr;
			case SurfRide::SRS_CASTNODE::Type::IMAGE: return &RflClassTraits<SurfRide::SRS_IMAGECAST>::rflClass;
			case SurfRide::SRS_CASTNODE::Type::SLICE: return &RflClassTraits<SurfRide::SRS_SLICECAST>::rflClass;
			case SurfRide::SRS_CASTNODE::Type::REFERENCE: return &RflClassTraits<SurfRide::SRS_REFERENCECAST>::rflClass;
			default: assert(false); return nullptr;
			};
		}>,
		short,
		short,
		SRS_USERDATA*
	>(
		"name",
		"id",
		"flags",
		"data",
		"childIndex",
		"siblingIndex",
		"userData"
	);

	auto srsTextDataMembers = CreateRflClassMembers<void,
		unsigned int,
		unsigned int,
		const char*,
		Vector2,
		short,
		short,
		short,
		short,
		short,
		short,
		SRS_FONT*
	>(
		"flags",
		"fontIndex",
		"str",
		"scale",
		"paddingLeft",
		"paddingRight",
		"paddingTop",
		"paddingBottom",
		"tracking",
		"lineHeight",
		"font"
	);

	auto srsImageCastMembers = CreateRflClassMembers<void,
		uint32_t,
		Vector2,
		Vector2,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		short,
		short,
		short,
		short,
		RflDynamicPointerArray<SRS_CROPREF, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_IMAGECAST*>(parent)->cropRef0Count; }>,
		RflDynamicPointerArray<SRS_CROPREF, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_IMAGECAST*>(parent)->cropRef1Count; }>,
		SRS_TEXTDATA*,
		uint32_t,
		RflDynamicStructPointer<[](const void* parent) -> const hh::fnd::RflClass* {
			switch (static_cast<const SurfRide::SRS_IMAGECAST*>(parent)->effectType) {
			case SurfRide::SRE_EFFECT_TYPE::BLUR: return &RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass;
			case SurfRide::SRE_EFFECT_TYPE::REFLECT: return &RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass;
			default: assert(false); return nullptr;
			}
		}>
	>(
		"flags",
		"size",
		"pivot",
		"vertexColorTopLeft",
		"vertexColorBottomLeft",
		"vertexColorTopRight",
		"vertexColorBottomRight",
		"cropIndex0",
		"cropIndex1",
		"cropRef0Count",
		"cropRef1Count",
		"cropRef0Offset",
		"cropRef1Offset",
		"textData",
		"effectType",
		"effectData"
	);

	auto srsReferenceCastMembers = CreateRflClassMembers<void,
		SRS_LAYER*,
		int,
		uint32_t,
		uint32_t,
		int
	>(
		"layer",
		"flags",
		"animationId",
		"animationFrame",
		"unk2"
	);

	auto srsSliceCastMembers = CreateRflClassMembers<void,
		uint32_t,
		Vector2,
		Vector2,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		Vector2,
		short,
		short,
		short,
		short,
		short,
		short,
		RflDynamicPointerArray<SRS_CROPREF, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_SLICECAST*>(parent)->cropRef0Count; }>,
		uint32_t,
		RflDynamicStructPointer<[](const void* parent) -> const hh::fnd::RflClass* {
			switch (static_cast<const SurfRide::SRS_SLICECAST*>(parent)->effectType) {
			case SurfRide::SRE_EFFECT_TYPE::BLUR: return &RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass;
			case SurfRide::SRE_EFFECT_TYPE::REFLECT: return &RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass;
			default: assert(false); return nullptr;
			}
		}>,
		RflDynamicInlineArray<SRS_SLICE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_SLICECAST*>(parent)->sliceHorizontalCount * static_cast<const SurfRide::SRS_SLICECAST*>(parent)->sliceVerticalCount; }>
	>(
		"flags",
		"size",
		"pivot",
		"vertexColorTopLeft",
		"vertexColorBottomLeft",
		"vertexColorTopRight",
		"vertexColorBottomRight",
		"fixedSize",
		"sliceHorizontalCount",
		"sliceVerticalCount",
		"horizontalFixedCount",
		"verticalFixedCount",
		"cropRef0Count",
		"cropRef1Count",
		"cropRef0Offset",
		"effectType",
		"effectData",
		"slices"
	);

	auto srsCropMembers = CreateRflClassMembers<void,
		float,
		float,
		float,
		float
	>(
		"left",
		"top",
		"right",
		"bottom"
	);

	auto srsCroprefMembers = CreateRflClassMembers<void,
		short,
		short,
		short
	>(
		"textureListIndex",
		"textureIndex",
		"cropIndex"
	);

	auto srsTextureMembers = CreateRflClassMembers<void,
		const char*,
		const char*,
		unsigned int,
		unsigned short,
		unsigned short,
		unsigned int,
		unsigned int,
		RflDynamicPointerArray<SRS_CROP, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_TEXTURE*>(parent)->cropCount; }>,
		SRS_USERDATA*
	>(
		"name",
		"fileName",
		"id",
		"width",
		"height",
		"flags",
		"cropCount",
		"crops",
		"userData"
	);

	auto srsTextureListMembers = CreateRflClassMembers<void,
		const char*,
		unsigned int,
		unsigned int,
		RflDynamicPointerArray<SRS_TEXTURE, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_TEXTURELIST*>(parent)->textureCount; }>,
		SRS_USERDATA*
	>(
		"name",
		"unk1",
		"textureCount",
		"textures",
		"userData"
	);

	auto srsKeyframeMembers = CreateRflClassMembers<void,
		unsigned int
	>(
		"frame"
	);

	auto srsKeyframeKeyLinearFloatMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, float>("value");
	//auto srsKeyframeKeyLinearDoubleMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, double>("value");
	auto srsKeyframeKeyLinearIntMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, int>("value");
	auto srsKeyframeKeyLinearBoolMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, bool>("value");
	auto srsKeyframeKeyLinearColorMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, SurfRide::Color>("value");
	auto srsKeyframeKeyHermiteFloatMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, float, float, float>("value", "derivativeIn", "derivativeOut");
	//auto srsKeyframeKeyHermiteDoubleMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, double, double, double>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyHermiteIntMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, int, int, int>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyHermiteBoolMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, bool, bool, bool>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyHermiteColorMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, SurfRide::Color, SurfRide::Color, SurfRide::Color>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyIndividualFloatMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, float, float, float, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	//auto srsKeyframeKeyIndividualDoubleMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, double, double, double, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	auto srsKeyframeKeyIndividualIntMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, int, int, int, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	auto srsKeyframeKeyIndividualBoolMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, bool, bool, bool, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	auto srsKeyframeKeyIndividualColorMembers = CreateRflClassMembers<SurfRide::SRS_KEYFRAME, SurfRide::Color, SurfRide::Color, SurfRide::Color, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");

	auto srsTrackMembers = CreateRflClassMembers<void,
		SRE_CURVE_TYPE,
		uint16_t,
		uint16_t,
		uint32_t,
		uint32_t,
		RflDynamicStructPointerArray<
			[](const void* parent) -> const hh::fnd::RflClass* {
				auto* track = static_cast<const SurfRide::SRS_TRACK*>(parent);

				switch (track->GetInterpolationType()) {
				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::CONSTANT:
				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::LINEAR:
					switch (track->GetDataType()) {
					case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyLinear<float>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::INDEX: return &RflClassTraits<SurfRide::KeyLinear<int>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyLinear<int>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyLinear<bool>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyLinear<SurfRide::Color>>::rflClass;
					default: assert(false); return nullptr;
					}
				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::HERMITE:
					switch (track->GetDataType()) {
					case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyHermite<float>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::INDEX: return &RflClassTraits<SurfRide::KeyHermite<int>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyHermite<int>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyHermite<bool>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyHermite<SurfRide::Color>>::rflClass;
					default: assert(false); return nullptr;
					}
				case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::INDIVIDUAL:
					switch (track->GetDataType()) {
					case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyIndividual<float>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::INDEX: return &RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyIndividual<bool>>::rflClass;
					case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyIndividual<SurfRide::Color>>::rflClass;
					default: assert(false); return nullptr;
					}
				default: assert(false); return nullptr;
				}
			},
			[](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_TRACK*>(parent)->keyCount; }
		>
	>(
		"trackType",
		"keyCount",
		"flags",
		"firstFrame",
		"lastFrame",
		"keyFrame"
	);

	auto srsMotionMembers = CreateRflClassMembers<void,
		uint16_t,
		uint16_t,
		RflDynamicPointerArray<SRS_TRACK, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_MOTION*>(parent)->trackCount; }>
	>(
		"castId",
		"trackCount",
		"tracks"
	);

	auto srsAnimationMembers = CreateRflClassMembers<void,
		const char*,
		uint32_t,
		uint32_t,
		uint32_t,
		RflDynamicPointerArray<SRS_MOTION, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_ANIMATION*>(parent)->motionCount; }>,
		SRS_USERDATA*,
		bool
	>(
		"name",
		"id",
		"motionCount",
		"frameCount",
		"motions",
		"userData",
		"isLooping"
	);

	auto srsFontMembers = CreateRflClassMembers<void,
		const char*,
		int,
		unsigned int,
		short,
		unsigned short,
		unsigned short,
		RflDynamicPointerArray<SRS_CHARACTER_MAPPING, [](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_FONT*>(parent)->characterCount; }>,
		uint64_t,
		SRS_USERDATA*
	>(
		"name",
		"id",
		"unk1",
		"unk2",
		"characterCount",
		"unk3",
		"characters",
		"unk4",
		"userData"
	);

	auto srsCharacterMappingMembers = CreateRflClassMembers<void,
		unsigned short,
		short,
		short,
		short
	>(
		"character",
		"textureListIndex",
		"textureIndex",
		"cropIndex"
	);

	auto srsDataBoolMembers = CreateRflClassMembers<void,
		const char*,
		SRS_DATA::Type,
		bool*
	>(
		"name",
		"type",
		"value"
	);

	auto srsDataIntMembers = CreateRflClassMembers<void,
		const char*,
		SRS_DATA::Type,
		int*
	>(
		"name",
		"type",
		"value"
	);

	auto srsDataUintMembers = CreateRflClassMembers<void,
		const char*,
		SRS_DATA::Type,
		unsigned int*
	>(
		"name",
		"type",
		"value"
	);

	auto srsDataFloatMembers = CreateRflClassMembers<void,
		const char*,
		SRS_DATA::Type,
		float*
	>(
		"name",
		"type",
		"value"
	);

	auto srsDataStringMembers = CreateRflClassMembers<void,
		const char*,
		SRS_DATA::Type,
		const char*
	>(
		"name",
		"type",
		"value"
	);

	auto srsUserDataMembers = CreateRflClassMembers<void,
		unsigned int,
		RflDynamicStructSelfPointerArray<
			[](const void* self) -> const hh::fnd::RflClass* {
				switch (static_cast<const SurfRide::SRS_DATA*>(self)->type) {
				case SurfRide::SRS_DATA::Type::BOOL: return &RflClassTraits<SurfRide::SRS_DATA_BOOL>::rflClass;
				case SurfRide::SRS_DATA::Type::INT: return &RflClassTraits<SurfRide::SRS_DATA_INT>::rflClass;
				case SurfRide::SRS_DATA::Type::UINT: return &RflClassTraits<SurfRide::SRS_DATA_UINT>::rflClass;
				case SurfRide::SRS_DATA::Type::FLOAT: return &RflClassTraits<SurfRide::SRS_DATA_FLOAT>::rflClass;
				case SurfRide::SRS_DATA::Type::STRING: return &RflClassTraits<SurfRide::SRS_DATA_STRING>::rflClass;
				default: assert(false); return nullptr;
				}
			},
			[](const void* parent) -> size_t { return static_cast<const SurfRide::SRS_USERDATA*>(parent)->count; }
		>
	>(
		"itemCount",
		"items"
	);

	auto srsBlurMembers = CreateRflClassMembers<void,
		uint32_t,
		uint32_t,
		uint32_t,
		uint32_t,
		uint32_t,
		uint32_t
	>(
		"field00",
		"field04",
		"cropCount",
		"steps",
		"unk1",
		"flags"
	);

	auto srsReflectMembers = CreateRflClassMembers<void,
		uint32_t,
		uint32_t,
		uint32_t,
		float,
		float,
		float,
		float,
		uint32_t,
		SurfRide::Color
	>(
		"unk1",
		"unk2",
		"unk3",
		"unk4",
		"unk5",
		"unk6",
		"unk7",
		"flags",
		"color"
	);
}

using namespace reflection::serialization::resource_rfls::surfride;

const RflClass RflClassTraits<SurfRide::Rotation3>::rflClass{ "SurfRideRotation3", nullptr, sizeof(SurfRide::Rotation3), nullptr, 0, surfRideRotation3Members.data(), surfRideRotation3Members.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_PROJECT>::rflClass{ "SRS_PROJECT", nullptr, sizeof(SurfRide::SRS_PROJECT), nullptr, 0, srsProjectMembers.data(), srsProjectMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_SCENE>::rflClass{ "SRS_SCENE", nullptr, sizeof(SurfRide::SRS_SCENE), nullptr, 0, srsSceneMembers.data(), srsSceneMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CAMERA>::rflClass{ "SRS_CAMERA", nullptr, sizeof(SurfRide::SRS_CAMERA), nullptr, 0, srsCameraMembers.data(), srsCameraMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_LAYER>::rflClass{ "SRS_LAYER", nullptr, sizeof(SurfRide::SRS_LAYER), nullptr, 0, srsLayerMembers.data(), srsLayerMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRS_BASE>::rflClass{ "SRS_TRS_BASE", nullptr, sizeof(SurfRide::SRS_TRS_BASE), nullptr, 0, srsTrsBaseMembers.data(), srsTrsBaseMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRS2D>::rflClass{ "SRS_TRS2D", &RflClassTraits<SurfRide::SRS_TRS_BASE>::rflClass, sizeof(SurfRide::SRS_TRS2D), nullptr, 0, srsTrs2DMembers.data(), srsTrs2DMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRS3D>::rflClass{ "SRS_TRS3D", &RflClassTraits<SurfRide::SRS_TRS_BASE>::rflClass, sizeof(SurfRide::SRS_TRS3D), nullptr, 0, srsTrs3DMembers.data(), srsTrs3DMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_SLICE>::rflClass{ "SRS_SLICE", nullptr, sizeof(SurfRide::SRS_SLICE), nullptr, 0, srsSliceMembers.data(), srsSliceMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TEXTDATA>::rflClass{ "SRS_TEXTDATA", nullptr, sizeof(SurfRide::SRS_TEXTDATA), nullptr, 0, srsTextDataMembers.data(), srsTextDataMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CASTNODE>::rflClass{ "SRS_CASTNODE", nullptr, sizeof(SurfRide::SRS_CASTNODE), nullptr, 0, srsCastNodeMembers.data(), srsCastNodeMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_IMAGECAST>::rflClass{ "SRS_IMAGECAST", nullptr, sizeof(SurfRide::SRS_IMAGECAST), nullptr, 0, srsImageCastMembers.data(), srsImageCastMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_REFERENCECAST>::rflClass{ "SRS_REFERENCECAST", nullptr, sizeof(SurfRide::SRS_REFERENCECAST), nullptr, 0, srsReferenceCastMembers.data(), srsReferenceCastMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_SLICECAST>::rflClass{ "SRS_SLICECAST", nullptr, sizeof(SurfRide::SRS_SLICECAST), nullptr, 0, srsSliceCastMembers.data(), srsSliceCastMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CROP>::rflClass{ "SRS_CROP", nullptr, sizeof(SurfRide::SRS_CROP), nullptr, 0, srsCropMembers.data(), srsCropMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CROPREF>::rflClass{ "SRS_CROPREF", nullptr, sizeof(SurfRide::SRS_CROPREF), nullptr, 0, srsCroprefMembers.data(), srsCroprefMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TEXTURE>::rflClass{ "SRS_TEXTURE", nullptr, sizeof(SurfRide::SRS_TEXTURE), nullptr, 0, srsTextureMembers.data(), srsTextureMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TEXTURELIST>::rflClass{ "SRS_TEXTURELIST", nullptr, sizeof(SurfRide::SRS_TEXTURELIST), nullptr, 0, srsTextureListMembers.data(), srsTextureListMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass{ "SRS_KEYFRAME", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeMembers.data(), srsKeyframeMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<float>>::rflClass{ "SRS_KEYFRAME_LINEAR_FLOAT", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyLinear<float>), nullptr, 0, srsKeyframeKeyLinearFloatMembers.data(), srsKeyframeKeyLinearFloatMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<int>>::rflClass{ "SRS_KEYFRAME_LINEAR_INT", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyLinear<int>), nullptr, 0, srsKeyframeKeyLinearIntMembers.data(), srsKeyframeKeyLinearIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<bool>>::rflClass{ "SRS_KEYFRAME_LINEAR_BOOL", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyLinear<bool>), nullptr, 0, srsKeyframeKeyLinearBoolMembers.data(), srsKeyframeKeyLinearBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<SurfRide::Color>>::rflClass{ "SRS_KEYFRAME_LINEAR_COLOR", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyLinear<SurfRide::Color>), nullptr, 0, srsKeyframeKeyLinearColorMembers.data(), srsKeyframeKeyLinearColorMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<float>>::rflClass{ "SRS_KEYFRAME_HERMITE_FLOAT", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyHermite<float>), nullptr, 0, srsKeyframeKeyHermiteFloatMembers.data(), srsKeyframeKeyHermiteFloatMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<int>>::rflClass{ "SRS_KEYFRAME_HERMITE_INT", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyHermite<int>), nullptr, 0, srsKeyframeKeyHermiteIntMembers.data(), srsKeyframeKeyHermiteIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<bool>>::rflClass{ "SRS_KEYFRAME_HERMITE_BOOL", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyHermite<bool>), nullptr, 0, srsKeyframeKeyHermiteBoolMembers.data(), srsKeyframeKeyHermiteBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<SurfRide::Color>>::rflClass{ "SRS_KEYFRAME_HERMITE_COLOR", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyHermite<SurfRide::Color>), nullptr, 0, srsKeyframeKeyHermiteColorMembers.data(), srsKeyframeKeyHermiteColorMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<float>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_FLOAT", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyIndividual<float>), nullptr, 0, srsKeyframeKeyIndividualFloatMembers.data(), srsKeyframeKeyIndividualFloatMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_INT", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyIndividual<int>), nullptr, 0, srsKeyframeKeyIndividualIntMembers.data(), srsKeyframeKeyIndividualIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<bool>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_BOOL", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyIndividual<bool>), nullptr, 0, srsKeyframeKeyIndividualBoolMembers.data(), srsKeyframeKeyIndividualBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<SurfRide::Color>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_COLOR", &RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass, sizeof(SurfRide::KeyIndividual<SurfRide::Color>), nullptr, 0, srsKeyframeKeyIndividualColorMembers.data(), srsKeyframeKeyIndividualColorMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRACK>::rflClass{ "SRS_TRACK", nullptr, sizeof(SurfRide::SRS_TRACK), nullptr, 0, srsTrackMembers.data(), srsTrackMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_MOTION>::rflClass{ "SRS_MOTION", nullptr, sizeof(SurfRide::SRS_MOTION), nullptr, 0, srsMotionMembers.data(), srsMotionMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_ANIMATION>::rflClass{ "SRS_ANIMATION", nullptr, sizeof(SurfRide::SRS_ANIMATION), nullptr, 0, srsAnimationMembers.data(), srsAnimationMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_FONT>::rflClass{ "SRS_FONT", nullptr, sizeof(SurfRide::SRS_FONT), nullptr, 0, srsFontMembers.data(), srsFontMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CHARACTER_MAPPING>::rflClass{ "SRS_CHARACTER_MAPPING", nullptr, sizeof(SurfRide::SRS_CHARACTER_MAPPING), nullptr, 0, srsCharacterMappingMembers.data(), srsCharacterMappingMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_DATA_BOOL>::rflClass{ "SRS_DATA_BOOL", nullptr, sizeof(SurfRide::SRS_DATA), nullptr, 0, srsDataBoolMembers.data(), srsDataBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_DATA_INT>::rflClass{ "SRS_DATA_INT", nullptr, sizeof(SurfRide::SRS_DATA), nullptr, 0, srsDataIntMembers.data(), srsDataIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_DATA_UINT>::rflClass{ "SRS_DATA_UINT", nullptr, sizeof(SurfRide::SRS_DATA), nullptr, 0, srsDataUintMembers.data(), srsDataUintMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_DATA_FLOAT>::rflClass{ "SRS_DATA_FLOAT", nullptr, sizeof(SurfRide::SRS_DATA), nullptr, 0, srsDataFloatMembers.data(), srsDataFloatMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_DATA_STRING>::rflClass{ "SRS_DATA_STRING", nullptr, sizeof(SurfRide::SRS_DATA), nullptr, 0, srsDataStringMembers.data(), srsDataStringMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_USERDATA>::rflClass{ "SRS_USERDATA", nullptr, sizeof(SurfRide::SRS_USERDATA), nullptr, 0, srsUserDataMembers.data(), srsUserDataMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_BLUR>::rflClass{ "SRS_BLUR", nullptr, sizeof(SurfRide::SRS_BLUR), nullptr, 0, srsBlurMembers.data(), srsBlurMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass{ "SRS_BLUR3D", nullptr, sizeof(SurfRide::SRS_BLUR3D), nullptr, 0, srsBlurMembers.data(), srsBlurMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_REFLECT>::rflClass{ "SRS_REFLECT", nullptr, sizeof(SurfRide::SRS_REFLECT), nullptr, 0, srsReflectMembers.data(), srsReflectMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass{ "SRS_REFLECT3D", nullptr, sizeof(SurfRide::SRS_REFLECT3D), nullptr, 0, srsReflectMembers.data(), srsReflectMembers.size(), nullptr };

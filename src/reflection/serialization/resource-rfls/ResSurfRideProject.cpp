#include "ResourceRfls.h"

namespace hh::fnd {
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
	template<> struct RflClassTraits<SurfRide::SRS_TEXTURE> : RflClassTraitsImpl<SurfRide::SRS_TEXTURE> {};
	template<> struct RflClassTraits<SurfRide::SRS_TEXTURELIST> : RflClassTraitsImpl<SurfRide::SRS_TEXTURELIST> {};
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
	template<> struct RflClassTraits<SurfRide::SRS_USERDATA> : RflClassTraitsImpl<SurfRide::SRS_USERDATA> {};
	template<> struct RflClassTraits<SurfRide::SRS_BLUR> : RflClassTraitsImpl<SurfRide::SRS_BLUR> {};
	template<> struct RflClassTraits<SurfRide::SRS_BLUR3D> : RflClassTraitsImpl<SurfRide::SRS_BLUR3D> {};
	template<> struct RflClassTraits<SurfRide::SRS_REFLECT> : RflClassTraitsImpl<SurfRide::SRS_REFLECT> {};
	template<> struct RflClassTraits<SurfRide::SRS_REFLECT3D> : RflClassTraitsImpl<SurfRide::SRS_REFLECT3D> {};

	template<> struct RflDynamicStructPointer<SurfRide::SRS_TRS_BASE, SurfRide::SRS_LAYER> {
		static const hh::fnd::RflClass* Resolve(void* obj, void* parent) {
			return static_cast<SurfRide::SRS_LAYER*>(parent)->flags & 0x1 ? &RflClassTraits<SurfRide::SRS_TRS3D>::rflClass : &RflClassTraits<SurfRide::SRS_TRS2D>::rflClass;
		}
	};

	template<> struct RflDynamicStructPointer<void, SurfRide::SRS_CASTNODE> {
		static const hh::fnd::RflClass* Resolve(void* obj, void* parent) {
			switch (static_cast<SurfRide::SRS_CASTNODE*>(obj)->GetType()) {
			case SurfRide::SRS_CASTNODE::Type::NORMAL: return nullptr;
			case SurfRide::SRS_CASTNODE::Type::IMAGE: return &RflClassTraits<SurfRide::SRS_IMAGECAST>::rflClass;
			case SurfRide::SRS_CASTNODE::Type::SLICE: return &RflClassTraits<SurfRide::SRS_SLICECAST>::rflClass;
			case SurfRide::SRS_CASTNODE::Type::REFERENCE: return &RflClassTraits<SurfRide::SRS_REFERENCECAST>::rflClass;
			default: assert(false); return nullptr;
			};
		}
	};

	template<> struct RflDynamicStructPointer<SurfRide::SRS_EFFECT, SurfRide::SRS_SLICECAST> {
		static const hh::fnd::RflClass* Resolve(void* obj, void* parent) {
			switch (static_cast<SurfRide::SRS_SLICECAST*>(parent)->effectType) {
			case SurfRide::SRE_EFFECT_TYPE::BLUR: return &RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass;
			case SurfRide::SRE_EFFECT_TYPE::REFLECT: return &RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass;
			default: assert(false); return nullptr;
			}
		}
	};

	template<> struct RflDynamicStructPointer<SurfRide::SRS_EFFECT, SurfRide::SRS_IMAGECAST> {
		static const hh::fnd::RflClass* Resolve(void* obj, void* parent) {
			switch (static_cast<SurfRide::SRS_IMAGECAST*>(parent)->effectType) {
			case SurfRide::SRE_EFFECT_TYPE::BLUR: return &RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass;
			case SurfRide::SRE_EFFECT_TYPE::REFLECT: return &RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass;
			default: assert(false); return nullptr;
			}
		}
	};

	template<> struct RflDynamicStructPointer<SurfRide::SRS_KEYFRAME, SurfRide::SRS_TRACK> {
		static const hh::fnd::RflClass* Resolve(void* obj, void* parent) {
			auto* track = static_cast<SurfRide::SRS_TRACK*>(parent);

			switch (track->GetInterpolationType()) {
			case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::CONSTANT:
			case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::LINEAR:
				switch (track->GetDataType()) {
				case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyLinear<float>>::rflClass;
				//case SurfRide::SRE_TRACK_DATA_TYPE::DOUBLE: return &RflClassTraits<SurfRide::KeyLinear<double>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyLinear<int>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyLinear<bool>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyLinear<SurfRide::Color>>::rflClass;
				default: assert(false); return nullptr;
				}
			case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::HERMITE:
				switch (track->GetDataType()) {
				case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyHermite<float>>::rflClass;
				//case SurfRide::SRE_TRACK_DATA_TYPE::DOUBLE: return &RflClassTraits<SurfRide::KeyHermite<double>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyHermite<int>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyHermite<bool>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyHermite<SurfRide::Color>>::rflClass;
				default: assert(false); return nullptr;
				}
			case SurfRide::SRE_TRACK_INTERPOLATION_TYPE::INDIVIDUAL:
				switch (track->GetDataType()) {
				case SurfRide::SRE_TRACK_DATA_TYPE::FLOAT: return &RflClassTraits<SurfRide::KeyIndividual<float>>::rflClass;
				//case SurfRide::SRE_TRACK_DATA_TYPE::DOUBLE: return &RflClassTraits<SurfRide::KeyIndividual<double>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::INT: return &RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::BOOL: return &RflClassTraits<SurfRide::KeyIndividual<bool>>::rflClass;
				case SurfRide::SRE_TRACK_DATA_TYPE::COLOR: return &RflClassTraits<SurfRide::KeyIndividual<SurfRide::Color>>::rflClass;
				default: assert(false); return nullptr;
				}
			default: assert(false); return nullptr;
			}
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_SCENE, SurfRide::SRS_PROJECT> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_PROJECT*>(parent)->sceneCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_TEXTURELIST, SurfRide::SRS_PROJECT> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_PROJECT*>(parent)->textureListCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_FONT, SurfRide::SRS_PROJECT> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_PROJECT*>(parent)->fontCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_LAYER, SurfRide::SRS_SCENE> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_SCENE*>(parent)->layerCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_CAMERA, SurfRide::SRS_SCENE> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_SCENE*>(parent)->cameraCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_CASTNODE, SurfRide::SRS_LAYER> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_LAYER*>(parent)->castCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_ANIMATION, SurfRide::SRS_LAYER> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_LAYER*>(parent)->animationCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_CROP, SurfRide::SRS_TEXTURE> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_TEXTURE*>(parent)->cropCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_TEXTURE, SurfRide::SRS_TEXTURELIST> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_TEXTURELIST*>(parent)->textureCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_KEYFRAME, SurfRide::SRS_TRACK> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_TRACK*>(parent)->keyCount;
		}
	};

	template<> struct RflDynamicInlineArray<SurfRide::SRS_SLICE, SurfRide::SRS_SLICECAST> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_SLICECAST*>(parent)->sliceHorizontalCount * static_cast<SurfRide::SRS_SLICECAST*>(parent)->sliceVerticalCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_DATA, SurfRide::SRS_USERDATA> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_USERDATA*>(parent)->count;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_CHARACTER_MAPPING, SurfRide::SRS_FONT> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_FONT*>(parent)->characterCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_MOTION, SurfRide::SRS_ANIMATION> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_ANIMATION*>(parent)->motionCount;
		}
	};

	template<> struct RflDynamicPointerArray<SurfRide::SRS_TRACK, SurfRide::SRS_MOTION> {
		static size_t Resolve(void* parent) {
			return static_cast<SurfRide::SRS_ANIMATION*>(parent)->motionCount;
		}
	};
}

#include <utilities/RflClassGen.h>

using namespace hh::fnd;
using namespace SurfRide;

namespace reflection::serialization::resource_rfls::surfride {
	auto srsProjectMembers = CreateRflClassMembers<
		const char*,
		unsigned short,
		short,
		unsigned short,
		short,
		RflDynamicPointerArray<SRS_SCENE, SRS_PROJECT>,
		RflDynamicPointerArray<SRS_TEXTURELIST, SRS_PROJECT>,
		RflDynamicPointerArray<SRS_FONT, SRS_PROJECT>,
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

	auto srsSceneMembers = CreateRflClassMembers<
		const char*,
		int,
		int,
		int,
		unsigned int,
		RflDynamicPointerArray<SRS_LAYER, SRS_SCENE>,
		short,
		short,
		RflDynamicPointerArray<SRS_CAMERA, SRS_SCENE>,
		int,
		csl::math::Vector2,
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

	auto srsCameraMembers = CreateRflClassMembers<
		const char*,
		int,
		csl::math::Vector3,
		csl::math::Vector3,
		int,
		int,
		float,
		float,
		uint64_t
	>(
		"name",
		"id",
		"position",
		"target",
		"flags",
		"fov",
		"nearClip",
		"farClip",
		"unk"
	);

	auto srsLayerMembers = CreateRflClassMembers<
		const char*,
		int,
		int,
		unsigned int,
		RflDynamicPointerArray<SRS_CASTNODE, SRS_LAYER>,
		RflDynamicStructPointer<SRS_TRS_BASE, SRS_LAYER>,
		int,
		RflDynamicPointerArray<SRS_ANIMATION, SRS_LAYER>,
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

	auto srsTrsBaseMembers = CreateRflClassMembers<
		SurfRide::Color,
		SurfRide::Color,
		bool
	>(
		"materialColor",
		"illuminationColor",
		"display"
	);

	auto srsTrs2DMembers = CreateRflClassMembers<
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

	auto srsTrs3DMembers = CreateRflClassMembers<
		Vector3,
		Rotation3,
		Vector3
	>(
		"position",
		"rotation",
		"scale"
	);

	auto srsSliceMembers = CreateRflClassMembers<
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

	auto srsCastNodeMembers = CreateRflClassMembers<
		const char*,
		int,
		int,
		RflDynamicStructPointer<void, SRS_CASTNODE>,
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

	auto srsTextDataMembers = CreateRflClassMembers<
		uint32_t
	>(
		"dummy"
	);

	auto srsImageCastMembers = CreateRflClassMembers<
		uint32_t,
		csl::math::Vector2,
		csl::math::Vector2,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		SurfRide::Color,
		short,
		short,
		short,
		short,
		size_t,
		size_t,
		SRS_TEXTDATA*,
		uint32_t,
		RflDynamicStructPointer<SRS_EFFECT, SRS_IMAGECAST>
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

	auto srsReferenceCastMembers = CreateRflClassMembers<
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

	auto srsSliceCastMembers = CreateRflClassMembers<
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
		size_t,
		uint32_t,
		RflDynamicStructPointer<SRS_EFFECT, SRS_SLICECAST>,
		RflDynamicInlineArray<SRS_SLICE, SRS_SLICECAST>
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

	auto srsCropMembers = CreateRflClassMembers<
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

	auto srsTextureMembers = CreateRflClassMembers<
		const char*,
		const char*,
		unsigned int,
		unsigned short,
		unsigned short,
		unsigned int,
		unsigned int,
		RflDynamicPointerArray<SRS_CROP, SRS_TEXTURE>,
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

	auto srsTextureListMembers = CreateRflClassMembers<
		const char*,
		unsigned int,
		unsigned int,
		RflDynamicPointerArray<SRS_TEXTURE, SRS_TEXTURELIST>,
		SRS_USERDATA*
	>(
		"name",
		"unk1",
		"textureCount",
		"textures",
		"userData"
	);

	auto srsKeyframeMembers = CreateRflClassMembers<
		unsigned int
	>(
		"frame"
	);

	auto srsKeyframeKeyLinearFloatMembers = CreateRflClassMembers<float>("value");
	//auto srsKeyframeKeyLinearDoubleMembers = CreateRflClassMembers<double>("value");
	auto srsKeyframeKeyLinearIntMembers = CreateRflClassMembers<int>("value");
	auto srsKeyframeKeyLinearBoolMembers = CreateRflClassMembers<bool>("value");
	auto srsKeyframeKeyLinearColorMembers = CreateRflClassMembers<SurfRide::Color>("value");
	auto srsKeyframeKeyHermiteFloatMembers = CreateRflClassMembers<float, float, float>("value", "derivativeIn", "derivativeOut");
	//auto srsKeyframeKeyHermiteDoubleMembers = CreateRflClassMembers<double, double, double>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyHermiteIntMembers = CreateRflClassMembers<int, int, int>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyHermiteBoolMembers = CreateRflClassMembers<bool, bool, bool>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyHermiteColorMembers = CreateRflClassMembers<SurfRide::Color, SurfRide::Color, SurfRide::Color>("value", "derivativeIn", "derivativeOut");
	auto srsKeyframeKeyIndividualFloatMembers = CreateRflClassMembers<float, float, float, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	//auto srsKeyframeKeyIndividualDoubleMembers = CreateRflClassMembers<double, double, double, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	auto srsKeyframeKeyIndividualIntMembers = CreateRflClassMembers<int, int, int, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	auto srsKeyframeKeyIndividualBoolMembers = CreateRflClassMembers<bool, bool, bool, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");
	auto srsKeyframeKeyIndividualColorMembers = CreateRflClassMembers<SurfRide::Color, SurfRide::Color, SurfRide::Color, SRE_INTERPOLATION_TYPE>("value", "derivativeIn", "derivativeOut", "interpolationType");

	auto srsTrackMembers = CreateRflClassMembers<
		SRE_CURVE_TYPE,
		uint16_t,
		uint16_t,
		uint32_t,
		uint32_t,
		RflDynamicStructPointer<SRS_KEYFRAME, SRS_TRACK>
	>(
		"trackType",
		"keyCount",
		"flags",
		"firstFrame",
		"lastFrame",
		"keyFrame"
	);

	auto srsMotionMembers = CreateRflClassMembers<
		uint16_t,
		uint16_t,
		RflDynamicPointerArray<SRS_TRACK, SRS_MOTION>
	>(
		"castId",
		"trackCount",
		"tracks"
	);

	auto srsAnimationMembers = CreateRflClassMembers<
		const char*,
		uint32_t,
		uint32_t,
		uint32_t,
		RflDynamicPointerArray<SRS_MOTION, SRS_ANIMATION>,
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

	auto srsFontMembers = CreateRflClassMembers<
		const char*,
		int,
		unsigned int,
		short,
		unsigned short,
		unsigned short,
		RflDynamicPointerArray<SRS_CHARACTER_MAPPING, SRS_FONT>,
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

	auto srsCharacterMappingMembers = CreateRflClassMembers<
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

	auto srsDataMembers = CreateRflClassMembers<
		const char*,
		SRS_DATA::Type,
		uint64_t
	>(
		"name",
		"type",
		"value"
	);

	auto srsUserDataMembers = CreateRflClassMembers<
		unsigned int,
		RflDynamicPointerArray<SRS_DATA, SRS_USERDATA>
	>(
		"itemCount",
		"items"
	);

	auto srsBlurMembers = CreateRflClassMembers<
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

	auto srsReflectMembers = CreateRflClassMembers<
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

const RflClass RflClassTraits<SurfRide::SRS_PROJECT>::rflClass{ "SRS_PROJECT", nullptr, sizeof(SurfRide::SRS_PROJECT), nullptr, 0, srsProjectMembers.data(), srsProjectMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_SCENE>::rflClass{ "SRS_SCENE", nullptr, sizeof(SurfRide::SRS_SCENE), nullptr, 0, srsSceneMembers.data(), srsSceneMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CAMERA>::rflClass{ "SRS_CAMERA", nullptr, sizeof(SurfRide::SRS_CAMERA), nullptr, 0, srsCameraMembers.data(), srsCameraMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_LAYER>::rflClass{ "SRS_LAYER", nullptr, sizeof(SurfRide::SRS_LAYER), nullptr, 0, srsLayerMembers.data(), srsLayerMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRS_BASE>::rflClass{ "SRS_TRS_BASE", nullptr, sizeof(SurfRide::SRS_TRS_BASE), nullptr, 0, srsTrsBaseMembers.data(), srsTrsBaseMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRS2D>::rflClass{ "SRS_TRS2D", nullptr, sizeof(SurfRide::SRS_TRS2D), nullptr, 0, srsTrs2DMembers.data(), srsTrs2DMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRS3D>::rflClass{ "SRS_TRS3D", nullptr, sizeof(SurfRide::SRS_TRS3D), nullptr, 0, srsTrs3DMembers.data(), srsTrs3DMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_SLICE>::rflClass{ "SRS_SLICE", nullptr, sizeof(SurfRide::SRS_SLICE), nullptr, 0, srsSliceMembers.data(), srsSliceMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TEXTDATA>::rflClass{ "SRS_TEXTDATA", nullptr, sizeof(SurfRide::SRS_TEXTDATA), nullptr, 0, srsTextDataMembers.data(), srsTextDataMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CASTNODE>::rflClass{ "SRS_CASTNODE", nullptr, sizeof(SurfRide::SRS_CASTNODE), nullptr, 0, srsCastNodeMembers.data(), srsCastNodeMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_IMAGECAST>::rflClass{ "SRS_IMAGECAST", nullptr, sizeof(SurfRide::SRS_IMAGECAST), nullptr, 0, srsImageCastMembers.data(), srsImageCastMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_REFERENCECAST>::rflClass{ "SRS_REFERENCECAST", nullptr, sizeof(SurfRide::SRS_REFERENCECAST), nullptr, 0, srsReferenceCastMembers.data(), srsReferenceCastMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_SLICECAST>::rflClass{ "SRS_SLICECAST", nullptr, sizeof(SurfRide::SRS_SLICECAST), nullptr, 0, srsSliceCastMembers.data(), srsSliceCastMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CROP>::rflClass{ "SRS_CROP", nullptr, sizeof(SurfRide::SRS_CROP), nullptr, 0, srsCropMembers.data(), srsCropMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TEXTURE>::rflClass{ "SRS_TEXTURE", nullptr, sizeof(SurfRide::SRS_TEXTURE), nullptr, 0, srsTextureMembers.data(), srsTextureMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TEXTURELIST>::rflClass{ "SRS_TEXTURELIST", nullptr, sizeof(SurfRide::SRS_TEXTURELIST), nullptr, 0, srsTextureListMembers.data(), srsTextureListMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_KEYFRAME>::rflClass{ "SRS_KEYFRAME", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeMembers.data(), srsKeyframeMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<float>>::rflClass{ "SRS_KEYFRAME_LINEAR_FLOAT", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyLinearFloatMembers.data(), srsKeyframeKeyLinearFloatMembers.size(), nullptr };
//const RflClass RflClassTraits<SurfRide::KeyLinear<double>>::rflClass{ "SRS_KEYFRAME_LINEAR_DOUBLE", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyLinearDoubleMembers.data(), (srsKeyframeKeyLinearDoubleMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<int>>::rflClass{ "SRS_KEYFRAME_LINEAR_INT", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyLinearIntMembers.data(), srsKeyframeKeyLinearIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<bool>>::rflClass{ "SRS_KEYFRAME_LINEAR_BOOL", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyLinearBoolMembers.data(), srsKeyframeKeyLinearBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyLinear<SurfRide::Color>>::rflClass{ "SRS_KEYFRAME_LINEAR_COLOR", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyLinearColorMembers.data(), srsKeyframeKeyLinearColorMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<float>>::rflClass{ "SRS_KEYFRAME_HERMITE_FLOAT", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyHermiteFloatMembers.data(), srsKeyframeKeyHermiteFloatMembers.size(), nullptr };
//const RflClass RflClassTraits<SurfRide::KeyHermite<double>>::rflClass{ "SRS_KEYFRAME_HERMITE_DOUBLE", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyHermiteDoubleMembers.data(), (srsKeyframeKeyHermiteDoubleMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<int>>::rflClass{ "SRS_KEYFRAME_HERMITE_INT", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyHermiteIntMembers.data(), srsKeyframeKeyHermiteIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<bool>>::rflClass{ "SRS_KEYFRAME_HERMITE_BOOL", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyHermiteBoolMembers.data(), srsKeyframeKeyHermiteBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyHermite<SurfRide::Color>>::rflClass{ "SRS_KEYFRAME_HERMITE_COLOR", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyHermiteColorMembers.data(), srsKeyframeKeyHermiteColorMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<float>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_FLOAT", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyIndividualFloatMembers.data(), srsKeyframeKeyIndividualFloatMembers.size(), nullptr };
//const RflClass RflClassTraits<SurfRide::KeyIndividual<double>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_DOUBLE", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyIndividualDoubleMembers.data(), (srsKeyframeKeyIndividualDoubleMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<int>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_INT", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyIndividualIntMembers.data(), srsKeyframeKeyIndividualIntMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<bool>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_BOOL", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyIndividualBoolMembers.data(), srsKeyframeKeyIndividualBoolMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::KeyIndividual<SurfRide::Color>>::rflClass{ "SRS_KEYFRAME_INDIVIDUAL_COLOR", nullptr, sizeof(SurfRide::SRS_KEYFRAME), nullptr, 0, srsKeyframeKeyIndividualColorMembers.data(), srsKeyframeKeyIndividualColorMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_TRACK>::rflClass{ "SRS_TRACK", nullptr, sizeof(SurfRide::SRS_TRACK), nullptr, 0, srsTrackMembers.data(), srsTrackMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_MOTION>::rflClass{ "SRS_MOTION", nullptr, sizeof(SurfRide::SRS_MOTION), nullptr, 0, srsMotionMembers.data(), srsMotionMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_ANIMATION>::rflClass{ "SRS_ANIMATION", nullptr, sizeof(SurfRide::SRS_ANIMATION), nullptr, 0, srsAnimationMembers.data(), srsAnimationMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_FONT>::rflClass{ "SRS_FONT", nullptr, sizeof(SurfRide::SRS_FONT), nullptr, 0, srsFontMembers.data(), srsFontMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_CHARACTER_MAPPING>::rflClass{ "SRS_CHARACTER_MAPPING", nullptr, sizeof(SurfRide::SRS_CHARACTER_MAPPING), nullptr, 0, srsCharacterMappingMembers.data(), srsCharacterMappingMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_DATA>::rflClass{ "SRS_DATA", nullptr, sizeof(SurfRide::SRS_DATA), nullptr, 0, srsDataMembers.data(), srsDataMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_USERDATA>::rflClass{ "SRS_USERDATA", nullptr, sizeof(SurfRide::SRS_USERDATA), nullptr, 0, srsUserDataMembers.data(), srsUserDataMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_BLUR>::rflClass{ "SRS_BLUR", nullptr, sizeof(SurfRide::SRS_BLUR), nullptr, 0, srsBlurMembers.data(), srsBlurMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_BLUR3D>::rflClass{ "SRS_BLUR3D", nullptr, sizeof(SurfRide::SRS_BLUR3D), nullptr, 0, srsBlurMembers.data(), srsBlurMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_REFLECT>::rflClass{ "SRS_REFLECT", nullptr, sizeof(SurfRide::SRS_REFLECT), nullptr, 0, srsReflectMembers.data(), srsReflectMembers.size(), nullptr };
const RflClass RflClassTraits<SurfRide::SRS_REFLECT3D>::rflClass{ "SRS_REFLECT3D", nullptr, sizeof(SurfRide::SRS_REFLECT3D), nullptr, 0, srsReflectMembers.data(), srsReflectMembers.size(), nullptr };

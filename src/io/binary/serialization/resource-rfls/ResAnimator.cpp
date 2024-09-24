#include "ResFxColFile2.h"

namespace hh::fnd {
	template<> struct RflClassTraits<hh::anim::ClipData::AnimationSettings> : RflClassTraitsImpl<hh::anim::ClipData::AnimationSettings> {};
	template<> struct RflClassTraits<hh::anim::ClipData> : RflClassTraitsImpl<hh::anim::ClipData> {};
	template<> struct RflClassTraits<hh::anim::TransitionData::TransitionInfo> : RflClassTraitsImpl<hh::anim::TransitionData::TransitionInfo> {};
	template<> struct RflClassTraits<hh::anim::TransitionData> : RflClassTraitsImpl<hh::anim::TransitionData> {};
	template<> struct RflClassTraits<hh::anim::StateData> : RflClassTraitsImpl<hh::anim::StateData> {};
	template<> struct RflClassTraits<hh::anim::BlendNodeData> : RflClassTraitsImpl<hh::anim::BlendNodeData> {};
	template<> struct RflClassTraits<hh::anim::TransitionArrayData> : RflClassTraitsImpl<hh::anim::TransitionArrayData> {};
	template<> struct RflClassTraits<hh::anim::EventData> : RflClassTraitsImpl<hh::anim::EventData> {};
	template<> struct RflClassTraits<hh::anim::LayerData> : RflClassTraitsImpl<hh::anim::LayerData> {};
	template<> struct RflClassTraits<hh::anim::BlendMaskData> : RflClassTraitsImpl<hh::anim::BlendMaskData> {};
	template<> struct RflClassTraits<hh::anim::TriggerData> : RflClassTraitsImpl<hh::anim::TriggerData> {};
	template<> struct RflClassTraits<hh::anim::BlendSpaceData::Triangle> : RflClassTraitsImpl<hh::anim::BlendSpaceData::Triangle> {};
	template<> struct RflClassTraits<hh::anim::BlendSpaceData> : RflClassTraitsImpl<hh::anim::BlendSpaceData> {};
	template<> struct RflClassTraits<hh::anim::AsmData> : RflClassTraitsImpl<hh::anim::AsmData> {};
}

#include <reflection/RflClassGen.h>

using namespace hh::fnd;
using namespace hh::anim;

namespace reflection::serialization::resource_rfls::animator {
	auto clipDataAnimationSettingsMembers = CreateRflClassMembers<void,
		const char*,
		float,
		float,
		float,
		uint8_t,
		bool
	>(
		"resourceName",
		"start",
		"end",
		"speed",
		"flags",
		"loops"
	);

	auto clipDataMembers = CreateRflClassMembers<void,
		const char*,
		ClipData::AnimationSettings,
		unsigned short,
		short,
		short,
		unsigned short,
		short
	>(
		"name",
		"animationSettings",
		"triggerCount",
		"triggerOffset",
		"blendMaskIndex",
		"childClipIndexCount",
		"childClipIndexOffset"
	);

	auto transitionDataTransitionInfoMembers = CreateRflClassMembers<void,
		uint8_t,
		uint8_t,
		short,
		float
	>(
		"type",
		"easingType",
		"targetStateIndex",
		"transitionTime"
	);

	auto transitionDataMembers = CreateRflClassMembers<void,
		TransitionData::TransitionInfo,
		short
	>(
		"transitionInfo",
		"transitionTimeVariableIndex"
	);

	auto stateDataMembers = CreateRflClassMembers<void,
		const char*,
		int8_t,
		bool,
		uint8_t,
		int8_t,
		short,
		short,
		float,
		short,
		unsigned short,
		short,
		short,
		TransitionData,
		unsigned short,
		short
	>(
		"name",
		"type",
		"transitImmediately",
		"flags",
		"defaultLayerIndex",
		"rootBlendNodeOrClipIndex",
		"maxCycles",
		"speed",
		"speedVariableIndex",
		"eventCount",
		"eventOffset",
		"transitionArrayIndex",
		"stateEndTransition",
		"flagIndexCount",
		"flagIndexOffset"
	);

	auto blendNodeDataMembers = CreateRflClassMembers<void,
		uint8_t,
		short,
		short,
		float,
		unsigned short,
		short
	>(
		"type",
		"blendSpaceIndex",
		"blendFactorVariableIndex",
		"blendFactorTarget",
		"childNodeArraySize",
		"childNodeArrayOffset"
	);

	auto transitionArrayDataMembers = CreateRflClassMembers<void,
		int,
		int
	>(
		"size",
		"offset"
	);

	auto eventDataMembers = CreateRflClassMembers<void,
		const char*,
		TransitionData
	>(
		"name",
		"transition"
	);

	auto layerDataMembers = CreateRflClassMembers<void,
		const char*,
		unsigned short,
		short
	>(
		"name",
		"layerId",
		"blendMaskIndex"
	);

	auto blendMaskDataMembers = CreateRflClassMembers<void,
		const char*,
		unsigned short,
		short
	>(
		"name",
		"maskBoneCount",
		"maskBoneOffset"
	);

	auto triggerDataMembers = CreateRflClassMembers<void,
		uint8_t,
		float,
		float,
		unsigned short,
		short,
		const char*
	>(
		"type",
		"unknown2",
		"unknown3",
		"triggerTypeIndex",
		"colliderIndex",
		"name"
	);

	auto blendSpaceDataTriangleMembers = CreateRflClassMembers<void,
		short,
		short,
		short,
		short
	>(
		"nodeIndex0",
		"nodeIndex1",
		"nodeIndex2",
		"unused"
	);

	auto blendSpaceDataMembers = CreateRflClassMembers<void,
		short,
		short,
		float,
		float,
		float,
		float,
		unsigned short,
		unsigned short,
		RflDynamicPointerArray<csl::math::Vector2, [](const void* parent) -> size_t { return static_cast<const BlendSpaceData*>(parent)->nodeCount; }>,
		RflDynamicPointerArray<short, [](const void* parent) -> size_t { return static_cast<const BlendSpaceData*>(parent)->nodeCount; }>,
		RflDynamicPointerArray<BlendSpaceData::Triangle, [](const void* parent) -> size_t { return static_cast<const BlendSpaceData*>(parent)->triangleCount; }>
	>(
		"xVariableIndex",
		"yVariableIndex",
		"xMin",
		"xMax",
		"yMin",
		"yMax",
		"nodeCount",
		"triangleCount",
		"nodes",
		"clipIndices",
		"triangles"
	);

	auto asmDataMembers = CreateRflClassMembers<void,
		unsigned int,
		int,
		int,
		RflDynamicPointerArray<ClipData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->clipCount; }>,
		int,
		RflDynamicPointerArray<StateData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->stateCount; }>,
		int,
		RflDynamicPointerArray<BlendNodeData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->blendNodeCount; }>,
		int,
		RflDynamicPointerArray<EventData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->eventCount; }>,
		int,
		RflDynamicPointerArray<TransitionArrayData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->transitionArrayCount; }>,
		int,
		RflDynamicPointerArray<TransitionData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->transitionCount; }>,
		TransitionData,
		int,
		RflDynamicPointerArray<short, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->flagIndexCount; }>,
		int,
		RflDynamicPointerArray<const char*, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->flagCount; }>,
		int,
		RflDynamicPointerArray<const char*, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->variableCount; }>,
		int,
		RflDynamicPointerArray<LayerData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->layerCount; }>,
		int,
		RflDynamicPointerArray<BlendMaskData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->blendMaskCount; }>,
		int,
		RflDynamicPointerArray<const char*, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->maskBoneCount; }>,
		int,
		RflDynamicPointerArray<TriggerData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->triggerCount; }>,
		int,
		RflDynamicPointerArray<const char*, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->triggerTypeCount; }>,
		int,
		RflDynamicPointerArray<const char*, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->colliderCount; }>,
		short,
		int,
		RflDynamicPointerArray<BlendSpaceData, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->blendSpaceCount; }>,
		int,
		RflDynamicPointerArray<short, [](const void* parent) -> size_t { return static_cast<const AsmData*>(parent)->childClipIndexCount; }>
	>(
		"magic",
		"version",
		"clipCount",
		"clips",
		"stateCount",
		"states",
		"blendNodeCount",
		"blendNodes",
		"eventCount",
		"events",
		"transitionArrayCount",
		"transitionArrays",
		"transitionCount",
		"transitions",
		"nullTransition",
		"flagIndexCount",
		"flagIndices",
		"flagCount",
		"flags",
		"variableCount",
		"variables",
		"layerCount",
		"layers",
		"blendMaskCount",
		"blendMasks",
		"maskBoneCount",
		"maskBones",
		"triggerCount",
		"triggers",
		"triggerTypeCount",
		"triggerTypes",
		"colliderCount",
		"colliders",
		"blendTreeRootNodeId",
		"blendSpaceCount",
		"blendSpaces",
		"childClipIndexCount",
		"childClipIndices"
	);
}

using namespace reflection::serialization::resource_rfls::animator;

const RflClass RflClassTraitsImpl<hh::anim::ClipData::AnimationSettings>::rflClass{ "AnimationSettings", nullptr, static_cast<uint32_t>(sizeof(hh::anim::ClipData::AnimationSettings)), nullptr, 0, clipDataAnimationSettingsMembers.data(), static_cast<uint32_t>(clipDataAnimationSettingsMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::ClipData>::rflClass{ "ClipData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::ClipData)), nullptr, 0, clipDataMembers.data(), static_cast<uint32_t>(clipDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::TransitionData::TransitionInfo>::rflClass{ "TransitionInfo", nullptr, static_cast<uint32_t>(sizeof(hh::anim::TransitionData::TransitionInfo)), nullptr, 0, transitionDataTransitionInfoMembers.data(), static_cast<uint32_t>(transitionDataTransitionInfoMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::TransitionData>::rflClass{ "TransitionData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::TransitionData)), nullptr, 0, transitionDataMembers.data(), static_cast<uint32_t>(transitionDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::StateData>::rflClass{ "StateData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::StateData)), nullptr, 0, stateDataMembers.data(), static_cast<uint32_t>(stateDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::BlendNodeData>::rflClass{ "BlendNodeData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::BlendNodeData)), nullptr, 0, blendNodeDataMembers.data(), static_cast<uint32_t>(blendNodeDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::TransitionArrayData>::rflClass{ "TransitionArrayData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::TransitionArrayData)), nullptr, 0, transitionArrayDataMembers.data(), static_cast<uint32_t>(transitionArrayDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::EventData>::rflClass{ "EventData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::EventData)), nullptr, 0, eventDataMembers.data(), static_cast<uint32_t>(eventDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::LayerData>::rflClass{ "LayerData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::LayerData)), nullptr, 0, layerDataMembers.data(), static_cast<uint32_t>(layerDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::BlendMaskData>::rflClass{ "BlendMaskData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::BlendMaskData)), nullptr, 0, blendMaskDataMembers.data(), static_cast<uint32_t>(blendMaskDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::TriggerData>::rflClass{ "TriggerData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::TriggerData)), nullptr, 0, triggerDataMembers.data(), static_cast<uint32_t>(triggerDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::BlendSpaceData::Triangle>::rflClass{ "Triangle", nullptr, static_cast<uint32_t>(sizeof(hh::anim::BlendSpaceData::Triangle)), nullptr, 0, blendSpaceDataTriangleMembers.data(), static_cast<uint32_t>(blendSpaceDataTriangleMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::BlendSpaceData>::rflClass{ "BlendSpaceData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::BlendSpaceData)), nullptr, 0, blendSpaceDataMembers.data(), static_cast<uint32_t>(blendSpaceDataMembers.size()), nullptr };
const RflClass RflClassTraitsImpl<hh::anim::AsmData>::rflClass{ "AsmData", nullptr, static_cast<uint32_t>(sizeof(hh::anim::AsmData)), nullptr, 0, asmDataMembers.data(), static_cast<uint32_t>(asmDataMembers.size()), nullptr };

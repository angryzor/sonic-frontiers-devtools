#include "DvNode.h"
#include <utilities/math/MathUtils.h>
#include <ranges>

namespace ui::operation_modes::modes::dvscene_editor {
	
	constexpr std::pair<size_t, size_t> NodeDataInfo[] = {
		{},
		{sizeof(hh::dv::DvNodePath::Data), offsetof(hh::dv::DvNodePath, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeCamera::Data), offsetof(hh::dv::DvNodeCamera, binaryData)},
		{sizeof(hh::dv::DvNodeCameraMotion::Data), offsetof(hh::dv::DvNodeCameraMotion, binaryData)},
		{sizeof(hh::dv::DvNodeCharacter::Data), offsetof(hh::dv::DvNodeCharacter, binaryData)},
		{sizeof(hh::dv::DvNodeCharacterMotion::Data), offsetof(hh::dv::DvNodeCharacterMotion, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeModel::Data), offsetof(hh::dv::DvNodeModel, binaryData)},
		{},
		{sizeof(hh::dv::DvNodeModelMotion::Data), offsetof(hh::dv::DvNodeModelMotion, binaryData)},
		{sizeof(hh::dv::DvNodeModelNode::Data), offsetof(hh::dv::DvNodeModelNode, binaryData)},
		{sizeof(hh::dv::DvNodeElement::Data), offsetof(hh::dv::DvNodeElement, binaryData)}
	};
	constexpr std::pair<size_t, size_t> ElementDataInfo[] = {
		{},
		{sizeof(hh::dv::DvElementCameraParams::Data), offsetof(hh::dv::DvElementCameraParams, binaryData)},
		{},
		{sizeof(hh::dv::DvElementDrawOff::Data), offsetof(hh::dv::DvElementDrawOff, binaryData)},
		{},
		{sizeof(hh::dv::DvElementPathOffset::Data), offsetof(hh::dv::DvElementPathOffset, binaryData)},
		{sizeof(hh::dv::DvElementCameraShake::Data), offsetof(hh::dv::DvElementCameraShake, binaryData)},
		{sizeof(hh::dv::DvElementCameraShakeLoop::Data), offsetof(hh::dv::DvElementCameraShakeLoop, binaryData)},
		{/*sizeof(hh::dv::DvElementEffect::Data)*/676, offsetof(hh::dv::DvElementEffect, binaryData)}, //due to aligning im hardcoding the size for now
		{sizeof(hh::dv::DvElementDirectionalLight::Data), offsetof(hh::dv::DvElementDirectionalLight, binaryData)},
		{sizeof(hh::dv::DvElementPathInterpolation::Data), offsetof(hh::dv::DvElementPathInterpolation, binaryData)},
		{0, 0},
		{sizeof(hh::dv::DvElementCameraNearFar::Data), offsetof(hh::dv::DvElementCameraNearFar, binaryData)},
		{sizeof(hh::dv::DvElementUVAnim::Data), offsetof(hh::dv::DvElementUVAnim, binaryData)},
		{sizeof(hh::dv::DvElementVisibilityAnim::Data), offsetof(hh::dv::DvElementVisibilityAnim, binaryData)},
		{sizeof(hh::dv::DvElementMaterialAnim::Data), offsetof(hh::dv::DvElementMaterialAnim, binaryData)},
		{sizeof(hh::dv::DvElementMultipleAnim::Data), offsetof(hh::dv::DvElementMultipleAnim, binaryData)},
		{sizeof(hh::dv::DvElementCameraOffset::Data), offsetof(hh::dv::DvElementCameraOffset, binaryData)},
		{sizeof(hh::dv::DvElementModelFade::Data), offsetof(hh::dv::DvElementModelFade, binaryData)},
		{sizeof(hh::dv::DvElementDebugMotion::Data), offsetof(hh::dv::DvElementDebugMotion, binaryData)},
		{sizeof(hh::dv::DvElementCameraHedgehog::Data), offsetof(hh::dv::DvElementCameraHedgehog, binaryData)},
		{sizeof(hh::dv::DvElementCameraInGame::Data), offsetof(hh::dv::DvElementCameraInGame, binaryData)},
		{sizeof(hh::dv::DvElementPointLight::Data), offsetof(hh::dv::DvElementPointLight, binaryData)},
		{sizeof(hh::dv::DvElementVertexAnimationTexture::Data), offsetof(hh::dv::DvElementVertexAnimationTexture, binaryData)},
		{sizeof(hh::dv::DvElementSpotlight::Data), offsetof(hh::dv::DvElementSpotlight, binaryData)},
		{sizeof(hh::dv::DvElementControllerVibration::Data), offsetof(hh::dv::DvElementControllerVibration, binaryData)},
#ifdef DEVTOOLS_TARGET_SDK_rangers
		{sizeof(hh::dv::DvElementSpotlightModel::Data), offsetof(hh::dv::DvElementSpotlightModel, binaryData)}
#elif DEVTOOLS_TARGET_SDK_miller
		{sizeof(hh::dv::DvElementTexturePatternAnim::Data), offsetof(hh::dv::DvElementTexturePatternAnim, binaryData)},
		{sizeof(hh::dv::DvElementMaterialParam::Data), offsetof(hh::dv::DvElementMaterialParam, binaryData)},
#endif
	};

	DvNode::DvNode() {}
	DvNode::DvNode(hh::dv::DvNodeBase* node, Context& ctx) : node{ node }, ctx{ &ctx }
	{
		fileNode = ctx.GetFileNode(node);
		fileResource = ctx.GetResourceByFileNode(fileNode);
	}

	bool DvNode::operator==(const DvNode& other) const
	{
		return memcmp(&fileNode->guid, &other.fileNode->guid, 16) == 0 && strcmp(fileNode->name, other.fileNode->name) == 0;
	}

	const char* DvNode::GetName() const
	{
		return fileNode->name;
	}

	bool DvNode::CanTransform() const 
	{
		constexpr unsigned int transformableElements[] = {
			5, 8, 17, 22, 24, 26, 1036
		};
		if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT)
			return std::ranges::find(transformableElements, static_cast<int>(reinterpret_cast<hh::dv::DvNodeElement*>(node)->binaryData.elementId)) != std::end(transformableElements);
		else
			return node->nodeType == hh::dv::DvNodeBase::NodeType::PATH;
	}

	Eigen::Affine3f DvNode::GetTransform() const 
	{
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(CanTransform())
		{
			Eigen::Affine3f transform = Eigen::Affine3f::Identity();
			switch (node->nodeType) {
			case hh::dv::DvNodeBase::NodeType::PATH:
				transform = Eigen::Affine3f(reinterpret_cast<hh::dv::DvNodePath::Data*>(fileNode->data)->matrix.matrix());
				break;
			case hh::dv::DvNodeBase::NodeType::ELEMENT:
			{
				auto* element = &fileNode->data[sizeof(hh::dv::DvNodeElement::Data)];
				switch (reinterpret_cast<hh::dv::DvNodeElement::Data*>(fileNode->data)->elementId) {
				case hh::dv::DvNodeElement::ElementID::PATH_OFFSET:
					transform = Eigen::Affine3f(reinterpret_cast<hh::dv::DvElementPathOffset::Data*>(element)->offsetMatrix.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::EFFECT:
					transform = Eigen::Affine3f(reinterpret_cast<hh::dv::DvElementEffect::Data*>(element)->effectMatrix.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET:
					transform.translation() = reinterpret_cast<hh::dv::DvElementCameraOffset::Data*>(element)->offsetPosition;
					break;
				case hh::dv::DvNodeElement::ElementID::POINT_LIGHT:
					transform.translation() = reinterpret_cast<hh::dv::DvElementPointLight::Data*>(element)->position;
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT:
					transform.translation() = reinterpret_cast<hh::dv::DvElementSpotlight::Data*>(element)->position;
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT_MODEL:
					transform.translation() = reinterpret_cast<hh::dv::DvElementSpotlightModel::Data*>(element)->position;
					break;
				case hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT:
					transform.translation() = reinterpret_cast<app::dv::DvElementVariablePointLight::Data*>(element)->position;
					break;
				}
				break;
			}
			}
			if(auto* x = node->parent)
				transform = TransformToAffine3f(x->transform) * transform;
			return transform;
		}
		else
#endif
			return Eigen::Affine3f{};
	}
	
    void DvNode::SetTransform(const Eigen::Affine3f &transform)
    {
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(CanTransform())
		{
			auto trans = transform;
			if(auto* x = node->parent)
				trans = TransformToAffine3f(x->transform).inverse() * transform;
			switch (node->nodeType) {
			case hh::dv::DvNodeBase::NodeType::PATH:
				reinterpret_cast<hh::dv::DvNodePath::Data*>(fileNode->data)->matrix = Eigen::Projective3f(trans.matrix());
				break;
			case hh::dv::DvNodeBase::NodeType::ELEMENT:
			{
				auto* element = &fileNode->data[sizeof(hh::dv::DvNodeElement::Data)];
				switch (reinterpret_cast<hh::dv::DvNodeElement::Data*>(fileNode->data)->elementId) {
				case hh::dv::DvNodeElement::ElementID::PATH_OFFSET:
					reinterpret_cast<hh::dv::DvElementPathOffset::Data*>(element)->offsetMatrix = Eigen::Projective3f(trans.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::EFFECT:
					reinterpret_cast<hh::dv::DvElementEffect::Data*>(element)->effectMatrix = Eigen::Projective3f(trans.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET:
					reinterpret_cast<hh::dv::DvElementCameraOffset::Data*>(element)->offsetPosition = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::POINT_LIGHT:
					reinterpret_cast<hh::dv::DvElementPointLight::Data*>(element)->position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT:
					reinterpret_cast<hh::dv::DvElementSpotlight::Data*>(element)->position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT_MODEL:
					reinterpret_cast<hh::dv::DvElementSpotlightModel::Data*>(element)->position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT:
					reinterpret_cast<app::dv::DvElementVariablePointLight::Data*>(element)->position = trans.translation();
					break;
				}
				break;
			}
			}
			UpdateRuntimeNode();
		}
#endif
    }

	DvNode DvNode::GetParent() {
		return LoopThroughParents(node);
	}

	void DvNode::DeleteNode() {
		Delete(node);
		Delete(*fileNode);
	}

	void DvNode::UpdateRuntimeNode() {
		memcpy(node->guid, &fileNode->guid, 16);
		node->nodeName.clear();
		for (char x = 0; x < strlen(fileNode->name) + 1; x++)
			node->nodeName.push_back(fileNode->name[x]);
		memcpy(&reinterpret_cast<char*>(node)[NodeDataInfo[fileNode->category].second], fileNode->data, NodeDataInfo[fileNode->category].first);
		switch (node->nodeType) {
		case hh::dv::DvNodeBase::NodeType::PATH:
			reinterpret_cast<hh::dv::DvNodePath*>(node)->transform = Affine3fToTransform(Eigen::Affine3f(reinterpret_cast<hh::dv::DvNodePath::Data*>(fileNode->data)->matrix.matrix()));
			break;
		/*case hh::dv::DvNodeBase::NodeType::MODEL:
		case hh::dv::DvNodeBase::NodeType::CHARACTER:
			node->Setup(fileNode->data);
			break;*/
		case hh::dv::DvNodeBase::NodeType::ELEMENT: {
			auto* nodeElement = reinterpret_cast<hh::dv::DvNodeElement*>(node);
			auto* nodeElementData = reinterpret_cast<hh::dv::DvNodeElement::Data*>(fileNode->data);
			nodeElement->start = nodeElementData->start * 100;
			nodeElement->end = nodeElementData->end * 100;
			char* elementData;
			size_t elementSize = 0;
			if (static_cast<unsigned int>(nodeElement->binaryData.elementId) >= 1000) {
				auto* appElement = reinterpret_cast<app::dv::AppDvElementBase*>(nodeElement->element);
				elementData = static_cast<char*>(appElement->elementBinaryData);
				elementSize = appElement->elementBinaryDataSize;
#ifdef DEVTOOLS_TARGET_SDK_miller
				if (static_cast<unsigned int>(nodeElement->binaryData.elementId) == 1000)
					elementSize = 172; // i dunno why, but in the create function it's wrong -_-
#endif
			}
			else {
				auto* element = reinterpret_cast<hh::dv::DvElementBase*>(nodeElement->element);
				elementData = &reinterpret_cast<char*>(element)[ElementDataInfo[static_cast<unsigned int>(nodeElement->binaryData.elementId)].second];
				elementSize = ElementDataInfo[static_cast<unsigned int>(nodeElement->binaryData.elementId)].first;
			}
			memcpy(elementData, &fileNode->data[NodeDataInfo[fileNode->category].first], elementSize);
			switch (nodeElement->binaryData.elementId) {
			case hh::dv::DvNodeElement::ElementID::EFFECT:
				nodeElement->element->DeleteData();
				break;
			case hh::dv::DvNodeElement::ElementID::QTE: {
				nodeElement->element->AddCallback(ctx->goDVSC->timeline->preCurrentFrame, nodeElement->transform);
				break;
			}
			}
			break;
		}
		}
	}
}

#include "DvNode.h"
#include <utilities/math/MathUtils.h>
#include <ranges>

namespace ui::operation_modes::modes::dvscene_editor {
	DvNode::DvNode() {}	
	DvNode::DvNode(hh::dv::DvNodeBase* node) : node{ node } {}

	bool DvNode::operator==(const DvNode& other) const
	{
		if (strcmp(node->guid, other.node->guid) == 0)
			return true;

        return false;
	}

	const char* DvNode::GetName() const
	{
		return node->nodeName.c_str();
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
				transform = TransformToAffine3f(reinterpret_cast<hh::dv::DvNodePath*>(node)->transform);
				break;
			case hh::dv::DvNodeBase::NodeType::ELEMENT:
			{
				auto* element = reinterpret_cast<hh::dv::DvNodeElement*>(node);
				switch (element->binaryData.elementId) {
				case hh::dv::DvNodeElement::ElementID::PATH_OFFSET:
					transform = Eigen::Affine3f(reinterpret_cast<hh::dv::DvElementPathOffset*>(element->element)->binaryData.offsetMatrix.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::EFFECT:
					transform = Eigen::Affine3f(reinterpret_cast<hh::dv::DvElementEffect*>(element->element)->binaryData.effectMatrix.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET:
					transform.translation() = reinterpret_cast<hh::dv::DvElementCameraOffset*>(element->element)->binaryData.offsetPosition;
					break;
				case hh::dv::DvNodeElement::ElementID::POINT_LIGHT:
					transform.translation() = reinterpret_cast<hh::dv::DvElementPointLight*>(element->element)->binaryData.position;
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT:
					transform.translation() = reinterpret_cast<hh::dv::DvElementSpotlight*>(element->element)->binaryData.position;
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT_MODEL:
					transform.translation() = reinterpret_cast<hh::dv::DvElementSpotlightModel*>(element->element)->binaryData.position;
					break;
				case hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT:
					transform.translation() = reinterpret_cast<app::dv::DvElementVariablePointLight*>(element->element)->GetData()->position;
					break;
				}
				break;
			}
			}
			if(auto* x = node->parent)
				transform = transform * TransformToAffine3f(x->transform);
			return transform;
		}
		else
#endif
			return Eigen::Affine3f{};
	}
	
    void DvNode::SetTransform(const Eigen::Affine3f &transform) const
    {
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(CanTransform())
		{
			auto trans = transform;
			if(auto* x = node->parent)
				trans = TransformToAffine3f(x->transform).inverse() * transform;
			switch (node->nodeType) {
			case hh::dv::DvNodeBase::NodeType::PATH:
				reinterpret_cast<hh::dv::DvNodePath*>(node)->transform = Affine3fToTransform(trans);
				break;
			case hh::dv::DvNodeBase::NodeType::ELEMENT:
			{
				auto* element = reinterpret_cast<hh::dv::DvNodeElement*>(node);
				switch (element->binaryData.elementId) {
				case hh::dv::DvNodeElement::ElementID::PATH_OFFSET:
					reinterpret_cast<hh::dv::DvElementPathOffset*>(element->element)->binaryData.offsetMatrix = Eigen::Projective3f(trans.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::EFFECT:
					reinterpret_cast<hh::dv::DvElementEffect*>(element->element)->binaryData.effectMatrix = Eigen::Projective3f(trans.matrix());
					break;
				case hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET:
					reinterpret_cast<hh::dv::DvElementCameraOffset*>(element->element)->binaryData.offsetPosition = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::POINT_LIGHT:
					reinterpret_cast<hh::dv::DvElementPointLight*>(element->element)->binaryData.position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT:
					reinterpret_cast<hh::dv::DvElementSpotlight*>(element->element)->binaryData.position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::SPOTLIGHT_MODEL:
					reinterpret_cast<hh::dv::DvElementSpotlightModel*>(element->element)->binaryData.position = trans.translation();
					break;
				case hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT:
					reinterpret_cast<app::dv::DvElementVariablePointLight*>(element->element)->GetData()->position = trans.translation();
					break;
				}
				break;
			}
			}
		}
#endif
    }

	DvNode DvNode::GetParent() {
		return LoopThroughParents(node);
	}

	void DvNode::DeleteNode() {
		Delete(node);
	}
}

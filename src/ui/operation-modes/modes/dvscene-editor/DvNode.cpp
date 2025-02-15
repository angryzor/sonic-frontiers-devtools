#include "DvNode.h"
#include <utilities/math/MathUtils.h>

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
		return node->nodeType == hh::dv::DvNodeBase::NodeType::PATH;
	}

	Eigen::Affine3f DvNode::GetTransform() 
	{
		#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(node->nodeType == hh::dv::DvNodeBase::NodeType::PATH)
		{
			auto* path = reinterpret_cast<hh::dv::DvNodePath*>(node);
			auto transform = TransformToAffine3f(path->transform);
			if(auto* x = node->parent){
				auto parentTrans = TransformToAffine3f(reinterpret_cast<hh::dv::DvNodePath*>(LoopThroughParents(x).node)->transform);
				transform = transform * parentTrans;
			}
			return transform;
		}
		else
#endif
			return Eigen::Affine3f{};
	}
	
    void DvNode::SetTransform(const Eigen::Affine3f &transform)
    {
		#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(node->nodeType == hh::dv::DvNodeBase::NodeType::PATH)
		{
			auto* path = reinterpret_cast<hh::dv::DvNodePath*>(node);
			auto trans = transform;
			if(auto* x = node->parent){
				auto parentTrans = TransformToAffine3f(reinterpret_cast<hh::dv::DvNodePath*>(LoopThroughParents(x).node)->transform);
				trans = TransformToAffine3f(path->transform) * TransformToAffine3f(node->transform).inverse() * transform;
			}
			path->transform = Affine3fToTransform(trans);
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

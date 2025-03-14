#pragma once
#include "Context.h"
#include <dieventlib/dvnode.h>

namespace ui::operation_modes::modes::dvscene_editor {
    struct DvNode {
	private:
		DvNode LoopThroughParents(hh::dv::DvNodeBase* node) {
			if(node->nodeType == hh::dv::DvNodeBase::NodeType::PATH)
				return DvNode(node, *ctx);
			else
				return LoopThroughParents(node->parent);
		}

		void Delete(dv::DvNode& node) {
			auto parent = GetParent();
			int y = 0;
			for(auto& x : parent.fileNode->childNodes){
				if (strcmp(reinterpret_cast<char*>(&x.guid), reinterpret_cast<char*>(&fileNode->guid)) == 0)
				{
					parent.fileNode->childNodes.erase(parent.fileNode->childNodes.begin() + y);
					break;
				}
				y++;
			}
			for (auto& x : node.childNodes)
				Delete(node);
			node.childNodes.clear();
		}

		void Delete(hh::dv::DvNodeBase* node){
			if(auto* y = node->parent){
				y->childrenElements0.remove(y->childrenElements0.find(node));
				y->childrenElements1.remove(y->childrenElements1.find(node));
				y->childrenElements2.remove(y->childrenElements2.find(node));
				y->childrenPath.remove(y->childrenPath.find(node));
				y->childrenCamera.remove(y->childrenCamera.find(node));
				y->childrenMotion.remove(y->childrenMotion.find(node));
#ifdef DEVTOOLS_TARGET_SDK_rangers
				if (node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER || node->nodeType == hh::dv::DvNodeBase::NodeType::MODEL)
					hh::game::GameObjectSystem::GetInstance()->GetGameObjectByHandle(reinterpret_cast<hh::dv::DvNodeBaseAnimationModel*>(node)->nodeCharacterObj->dvStandardChar)->Kill();
#endif
				if(node->childrenElements0.begin())
					for(auto x = 0; x < node->childrenElements0.size(); x++)
						Delete(node->childrenElements0[x]);
				if (node->childrenElements1.begin())
					for (auto x = 0; x < node->childrenElements1.size(); x++)
						Delete(node->childrenElements1[x]);
				if (node->childrenElements2.begin())
					for (auto x = 0; x < node->childrenElements2.size(); x++)
						Delete(node->childrenElements2[x]);
				if (node->childrenPath.begin())
					for (auto x = 0; x < node->childrenPath.size(); x++)
						Delete(node->childrenPath[x]);
				if (node->childrenCamera.begin())
					for (auto x = 0; x < node->childrenCamera.size(); x++)
						Delete(node->childrenCamera[x]);
				if (node->childrenMotion.begin())
					for (auto x = 0; x < node->childrenMotion.size(); x++)
						Delete(node->childrenMotion[x]);
				node->Free();
			}
		}
	public:
		Context* ctx;
        hh::dv::DvNodeBase* node;
		dv::DvNode* fileNode;

		DvNode();
		DvNode(hh::dv::DvNodeBase* node, Context& ctx);

		bool operator==(const DvNode& other) const;
		const char* GetName() const;
		bool CanTransform() const;
		Eigen::Affine3f GetTransform() const;
		void SetTransform(const Eigen::Affine3f& transform) const;
		DvNode GetParent();
		void DeleteNode();
		void UpdateRuntimeNode();
	};
}

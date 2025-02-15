#pragma once

namespace ui::operation_modes::modes::dvscene_editor {
    struct DvNode {
	private:
		DvNode LoopThroughParents(hh::dv::DvNodeBase* node) {
			if(node->nodeType == hh::dv::DvNodeBase::NodeType::PATH)
				return DvNode(node);
			else
				return LoopThroughParents(node->parent);
		}

		void Delete(hh::dv::DvNodeBase* node){
			if(auto* y = node->parent){
				y->childrenElements0.remove(y->childrenElements0.find(node));
				y->childrenElements1.remove(y->childrenElements1.find(node));
				y->childrenElements2.remove(y->childrenElements2.find(node));
				y->childrenPath.remove(y->childrenPath.find(node));
				y->childrenCamera.remove(y->childrenCamera.find(node));
				y->childrenMotion.remove(y->childrenMotion.find(node));
				if (node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER || node->nodeType == hh::dv::DvNodeBase::NodeType::MODEL)
					hh::game::GameObjectSystem::GetInstance()->GetGameObjectByHandle(reinterpret_cast<hh::dv::DvNodeBaseAnimationModel*>(node)->nodeCharacterObj->dvStandardChar)->Kill();
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
        hh::dv::DvNodeBase* node;

		DvNode();
		DvNode(hh::dv::DvNodeBase* node);

		bool operator==(const DvNode& other) const;
		const char* GetName() const;
		bool CanTransform() const;
		Eigen::Affine3f GetTransform();
		void SetTransform(const Eigen::Affine3f& transform);
		DvNode GetParent();
		void DeleteNode();
	};
}

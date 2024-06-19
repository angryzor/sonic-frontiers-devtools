#include "ObjectList.h"
#include "ObjectInspection.h"
#include <ui/common/Icons.h>
#include <utilities/math/MathUtils.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectList::ObjectList(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : CompatibleObject{ allocator }, objectInspection{ objectInspection }
{
}

void ObjectList::RenderObjectTreeNode(GameObject* obj) {
	ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (objectInspection.focusedObjects.find(obj) != -1)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	auto* transform = obj->GetComponent<GOCTransform>();

	if ((transform && transform->GetChildren().size() > 0) || obj->GetChildren().size() > 0) {
		bool isOpen = ImGui::TreeNodeEx(obj, nodeflags, "%s", obj->name.c_str());

		if (ImGui::IsItemClicked()) {
			objectInspection.focusedObjects.clear();
			objectInspection.focusedObjects.push_back(obj);
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("GameObject", &obj, sizeof(obj));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
				GameObject* child = *static_cast<GameObject**>(payload->Data);

				GOCTransform* objTransform = obj->GetComponent<GOCTransform>();
				GOCTransform* childTransform = child->GetComponent<GOCTransform>();

				if (objTransform && childTransform && obj != child) {
					auto parentAbsoluteTransform = TransformToAffine3f(objTransform->GetFrame().fullTransform);
					auto childAbsoluteTransform = TransformToAffine3f(childTransform->GetFrame().fullTransform);

					childTransform->SetLocalTransform(Affine3fToTransform(parentAbsoluteTransform.inverse() * childAbsoluteTransform));
					childTransform->SetParent(objTransform);
				}
			}
			ImGui::EndDragDropTarget();
		}
		
		if (isOpen) {
			for (auto& child : transform->GetChildren()) {
				RenderObjectTreeNode(child.GetOwnerGameObject());
			}
			ImGui::TreePop();
		}
	}
	else {
		ImGui::TreeNodeEx(obj, nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", obj->name.c_str());

		if (ImGui::IsItemClicked()) {
			objectInspection.focusedObjects.clear();
			objectInspection.focusedObjects.push_back(obj);
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("GameObject", &obj, sizeof(obj));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
				GameObject* child = *static_cast<GameObject**>(payload->Data);

				GOCTransform* objTransform = obj->GetComponent<GOCTransform>();
				GOCTransform* childTransform = child->GetComponent<GOCTransform>();

				if (objTransform && childTransform && obj != child) {
					auto parentAbsoluteTransform = TransformToAffine3f(objTransform->GetFrame().fullTransform);
					auto childAbsoluteTransform = TransformToAffine3f(childTransform->GetFrame().fullTransform);

					childTransform->SetLocalTransform(Affine3fToTransform(parentAbsoluteTransform.inverse() * childAbsoluteTransform));
					childTransform->SetParent(objTransform);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}

void ObjectList::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Objects", NULL, windowFlags)) {
		if (ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto* layer : GameManager::GetInstance()->gameObjectLayers) {
						for (auto* obj : layer->objects) {
							auto* transform = obj->GetComponent<GOCTransform>();

							if (!transform || !transform->IsExistParent())
								RenderObjectTreeNode(obj);
						}
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto* layer : GameManager::GetInstance()->gameObjectLayers) {
						if (layer->objects.size() != 0 && ImGui::TreeNode(layer, layer->name)) {
							for (auto* obj : layer->objects) {
								ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

								if (objectInspection.focusedObjects.find(obj) != -1)
									nodeflags |= ImGuiTreeNodeFlags_Selected;

								ImGui::TreeNodeEx(obj, nodeflags, "%s", obj->name.c_str());

								if (ImGui::IsItemClicked()) {
									objectInspection.focusedObjects.clear();
									objectInspection.focusedObjects.push_back(obj);
								}

								if (ImGui::BeginDragDropSource()) {
									ImGui::SetDragDropPayload("GameObject", &obj, sizeof(obj));
									ImGui::EndDragDropSource();
								}

								if (ImGui::BeginDragDropTarget()) {
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
										GameObject* child = *static_cast<GameObject**>(payload->Data);

										GOCTransform* objTransform = obj->GetComponent<GOCTransform>();
										GOCTransform* childTransform = child->GetComponent<GOCTransform>();

										if (obj != child) {
											auto parentAbsoluteTransform = TransformToAffine3f(objTransform->GetFrame().fullTransform);
											auto childAbsoluteTransform = TransformToAffine3f(childTransform->GetFrame().fullTransform);

											childTransform->SetLocalTransform(Affine3fToTransform(parentAbsoluteTransform.inverse() * childAbsoluteTransform));
											childTransform->SetParent(objTransform);
										}
									}
									ImGui::EndDragDropTarget();
								}
							}
							ImGui::TreePop();
						}
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

#include "ObjectLibrary.h"
#include "Context.h"
#include <ui/common/Icons.h>

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	void ObjectLibrary::RenderPanel() {
		auto& context = GetContext();
		auto* registry = GameObjectSystem::GetInstance()->gameObjectRegistry;

		auto* focusedChunk = context.GetFocusedChunk();
		if (focusedChunk) {
			const char* targetLayerPreview = "<none>";

			for (auto* layer : focusedChunk->GetLayers())
				if (layer == context.placementTargetLayer)
					targetLayerPreview = layer->GetName();

			if (ImGui::BeginCombo("Target placement layer", targetLayerPreview)) {
				for (auto* layer : focusedChunk->GetLayers()) {
					if (ImGui::Selectable(layer->GetName(), layer == context.placementTargetLayer))
						context.placementTargetLayer = layer;
					if (context.placementTargetLayer == layer)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (context.placementTargetLayer) {
				if (context.objectClassToPlace) {
					bool clickedStop = ImGui::Button("Stop placing");
					ImGui::SameLine();
					ImGui::Text("Placing %s", context.objectClassToPlace->name);
					if (clickedStop)
						context.objectClassToPlace = nullptr;
				}
				else {
					if (!selectedClass)
						ImGui::BeginDisabled();
					if (ImGui::Button("Place"))
						context.objectClassToPlace = selectedClass;
					if (!selectedClass)
						ImGui::EndDisabled();
				}
			}
		}

		if (ImGui::BeginChild("List of objects"))
			tree.Render("ObjectLibrary");

		ImGui::EndChild();
	}

	PanelTraits ObjectLibrary::GetPanelTraits() const
	{
		return { "Object library", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	TreeView<ObjectLibraryTreeViewNode> ObjectLibrary::BuildTree()
	{
		auto* registry = GameObjectSystem::GetInstance()->gameObjectRegistry;
		TreeViewNode<ObjectLibraryTreeViewNode> rootNode{ GetAllocator(), { *this, GetAllocator() } };
		rootNode.node.groupLabel = "ROOT";

		for (auto* objClass : registry->GetGameObjectClasses()) {
			TreeViewNode<ObjectLibraryTreeViewNode>* currentNode = &rootNode;
			auto* categoryAttr = static_cast<const char*>(objClass->GetAttributeValue("category"));
			const char* category = categoryAttr == nullptr ? nullptr : categoryAttr;

			while (category != nullptr) {
				const char* divider = strstr(category, "/");
				size_t elementLen = divider == nullptr ? strlen(category) : divider - category;

				csl::ut::VariableString categoryElement{ category, static_cast<int>(elementLen), GetAllocator() };

				TreeViewNode<ObjectLibraryTreeViewNode>* categoryNode{};
				for (auto& child : currentNode->children) {
					if (child.node.type == ObjectLibraryTreeViewNode::Type::GROUP && !strcmp(child.node.groupLabel.c_str(), categoryElement.c_str())) {
						categoryNode = &child;
						break;
					}
				}

				if (categoryNode == nullptr) {
					currentNode->AddChild({ GetAllocator(), { *this, GetAllocator(), std::move(categoryElement) } });
					categoryNode = &currentNode->children[currentNode->children.size() - 1];
				}

				category = divider == nullptr ? nullptr : divider + 1;
				currentNode = categoryNode;
			}

			currentNode->AddChild({ GetAllocator(), { *this, GetAllocator(), objClass } });
		}

		return { GetAllocator(), std::move(rootNode) };
	}

	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, const GameObjectClass* objectClass) : library{ library }, type{ Type::OBJECT_CLASS }, objectClass{ objectClass }, groupLabel{ allocator } {}
	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, csl::ut::VariableString&& groupLabel) : library{ library }, type{ Type::GROUP }, groupLabel{ std::move(groupLabel) } {}
	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator) : library{ library }, type{ Type::GROUP }, groupLabel{ allocator } {}
	ObjectLibraryTreeViewNode::ObjectLibraryTreeViewNode(ObjectLibraryTreeViewNode&& other) : library{ other.library }, type{ other.type }, objectClass{ other.objectClass }, groupLabel{ std::move(other.groupLabel) } {}

	const void* ObjectLibraryTreeViewNode::GetID() const
	{
		switch (type) {
		case Type::OBJECT_CLASS:
			return objectClass;
		case Type::GROUP:
			return groupLabel;
		}
	}

	const char* ObjectLibraryTreeViewNode::GetLabel() const
	{
		switch (type) {
		case Type::OBJECT_CLASS:
			return objectClass->name;
		case Type::GROUP:
			return groupLabel.c_str();
		}
	}

	bool ObjectLibraryTreeViewNode::MatchesSearchString(const char* searchString) const
	{
		return strstr(GetLabel(), searchString);
	}

	bool ObjectLibraryTreeViewNode::Render(ImGuiTreeNodeFlags nodeflags) const
	{
		if (type == Type::OBJECT_CLASS && library.selectedClass == objectClass)
			nodeflags |= ImGuiTreeNodeFlags_Selected;

		bool isOpen = ImGui::TreeNodeEx(GetID(), nodeflags, "%s", GetLabel());

		if (type == Type::OBJECT_CLASS) {
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload("GameObjectClass", &objectClass, sizeof(objectClass));
				ImGui::EndDragDropSource();
			}
			else if (ImGui::IsItemClicked()) {
				library.selectedClass = objectClass;

				if (library.GetContext().objectClassToPlace)
					library.GetContext().objectClassToPlace = library.selectedClass;
			}
		}

		return isOpen;
	}
}

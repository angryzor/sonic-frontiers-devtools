#pragma once

template<typename T>
class TreeView;

template<typename T>
class TreeViewNode {
	friend class TreeView<T>;
public:
	csl::ut::MoveArray<TreeViewNode<T>> children;
	TreeViewNode<T>* parent{};
	T node;

	TreeViewNode(TreeViewNode<T>& other) = delete;
	TreeViewNode(TreeViewNode<T>&& other) : node{ std::move(other.node) }, children{ std::move(other.children) }, parent{ other.parent } {
		for (auto& child : children)
			child.parent = this;
	}
	TreeViewNode<T>& operator=(TreeViewNode<T>& other) = delete;

	TreeViewNode(csl::fnd::IAllocator* allocator, T& node) : children{ allocator }, node{ node } {}
	TreeViewNode(csl::fnd::IAllocator* allocator, T&& node) : children{ allocator }, node{ std::move(node) } {}

	void AddChild(TreeViewNode<T>&& child) {
		child.parent = this;
		children.push_back(std::move(child));
	}

protected:

	int GetTotalVisibleObjects(TreeView<T>& tv) const {
		int result{ 0 };
		int childrenResult{ 0 };

		for (auto& child : children)
			childrenResult += child.GetTotalVisibleObjects(tv);

		if (parent == nullptr || ImGui::GetStateStorage()->GetInt(ImGui::GetID(node.GetID())))
			result += childrenResult;

		if (parent != nullptr && (tv.searchStr[0] == 0 || childrenResult > 0 || node.MatchesSearchString(tv.searchStr)))
			result++;

		return result;
	}

	void Render(TreeView<T>& tv, int startIdx, int count) {
		ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (children.size() != 0) {
			if (node.Render(nodeflags)) {
				ImGui::Indent();
				RenderChildren(tv, startIdx, count - 1);
				ImGui::Unindent();
			}
		}
		else
			node.Render(nodeflags | ImGuiTreeNodeFlags_Leaf);
	}

	void RenderChildren(TreeView<T>& tv, int startIdx, int count) {
		for (TreeViewNode<T>& child : children) {
			if (count <= 0)
				break;

			auto totalVisible = child.GetTotalVisibleObjects(tv);

			if (startIdx < totalVisible) {
				child.Render(tv, startIdx, count);

				count -= totalVisible - startIdx;
			}

			startIdx = std::max(0, startIdx - totalVisible);
		}
	}
};

template<typename T>
class TreeView : public hh::fnd::ReferencedObject {
public:
	TreeViewNode<T> rootNode;
	char searchStr[200]{ "" };

	TreeView(csl::fnd::IAllocator* allocator, TreeViewNode<T>&& rootNode) : hh::fnd::ReferencedObject{ allocator, true }, rootNode{ std::move(rootNode) } {}

	void Render(const char* id) {
		ImGui::PushID(id);
		ImGui::InputText("Search", searchStr, 200);
		if (ImGui::BeginChild("Tree")) {
			ImGuiListClipper clipper;
			clipper.Begin(rootNode.GetTotalVisibleObjects(*this), ImGui::GetTextLineHeight());

			while (clipper.Step())
				rootNode.RenderChildren(*this, clipper.DisplayStart, clipper.DisplayEnd - clipper.DisplayStart);

			clipper.End();
		}
		ImGui::EndChild();
		ImGui::PopID();
	}
};

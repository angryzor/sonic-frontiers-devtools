#pragma once
#include <ui/common/TreeView.h>

class ObjectLibrary;
struct ObjectLibraryTreeViewNode {
	enum class Type {
		OBJECT_CLASS,
		GROUP,
	};

	Type type;
	csl::ut::VariableString groupLabel;
	const hh::game::GameObjectClass* objectClass;
	ObjectLibrary& library;

	const void* GetID() const;
	const char* GetLabel() const;
	bool MatchesSearchString(const char* searchString) const;
	bool Render(ImGuiTreeNodeFlags nodeflags) const;

	ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* objectClass);
	ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, csl::ut::VariableString&& groupLabel);
	ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator);
	ObjectLibraryTreeViewNode(ObjectLibraryTreeViewNode&& other);
};

class LevelEditor;
class ObjectLibrary : public hh::fnd::BaseObject {
	friend class ObjectLibraryTreeViewNode;
    LevelEditor& levelEditor;
	TreeView<ObjectLibraryTreeViewNode> tree;
    const hh::game::GameObjectClass* selectedClass{};
public:
    ObjectLibrary(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
	TreeView<ObjectLibraryTreeViewNode> BuildTree();
};

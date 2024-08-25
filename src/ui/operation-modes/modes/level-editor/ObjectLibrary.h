#pragma once
#include <ui/common/TreeView.h>
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::level_editor {
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
		bool IsSelected() const;
		bool MatchesSearchString(const char* searchString) const;
		void PreRender() const;
		void PostRender() const;

		ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* objectClass);
		ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator, csl::ut::VariableString&& groupLabel);
		ObjectLibraryTreeViewNode(ObjectLibrary& library, csl::fnd::IAllocator* allocator);
		ObjectLibraryTreeViewNode(ObjectLibraryTreeViewNode&& other);
	};

	class LevelEditor;
	class ObjectLibrary : public Panel<Context> {
		friend struct ObjectLibraryTreeViewNode;
		TreeView<ObjectLibraryTreeViewNode> tree{ BuildTree() };
		const hh::game::GameObjectClass* selectedClass{};
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		TreeView<ObjectLibraryTreeViewNode> BuildTree();
	};
}

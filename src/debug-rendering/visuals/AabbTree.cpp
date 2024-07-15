#include "AabbTree.h"

void RenderTreeNode(hh::gfnd::DrawContext& ctx, hh::ut::AabbTree& tree, unsigned int depth, unsigned int offset) {
	auto& node = tree.nodes[offset];

	ctx.DrawAABB(node.aabb.min, node.aabb.max, node.IsLeaf() ? hh::fnd::Color8{ 0, 255, 0, 255 } : hh::fnd::Color8{ 0, 0, 255, 255 });

	if (!node.IsLeaf()) {
		RenderTreeNode(ctx, tree, depth + 1, 2 * offset);
		RenderTreeNode(ctx, tree, depth + 1, 2 * offset + 1);
	}
}

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, hh::ut::AabbTree& tree)
{
	tree.Lock();

	if (!tree.nodes.empty()) {
		RenderTreeNode(ctx, tree, 0, 1);

		for (const auto& aabb : tree.aabbs)
			ctx.DrawAABB(aabb.min, aabb.max, { 255, 0, 0, 255 });
	}

	tree.Unlock();
}

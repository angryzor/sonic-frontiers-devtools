#include "KdTree.h"
#include "Geometry.h"

using namespace hh::ut;

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const KdTreeNode& node) {
	switch (node.type) {
	case KdTreeNode::Type::SPLIT_X:
		ctx.DrawPlane(
			Eigen::Affine3f{ Eigen::Translation3f{ node.deadzoneStartCoord, node.aabb.Center().y(), node.aabb.Center().z() } * Eigen::AngleAxisf{ 1.5707964f, Eigen::Vector3f::UnitY() } },
			node.aabb.max.z() - node.aabb.min.z(),
			node.aabb.max.y() - node.aabb.min.y(),
			csl::ut::Color8{ 255, 0, 0, 255 }
		);
		ctx.DrawPlane(
			Eigen::Affine3f{ Eigen::Translation3f{ node.deadzoneEndCoord, node.aabb.Center().y(), node.aabb.Center().z() } *Eigen::AngleAxisf{ 1.5707964f, Eigen::Vector3f::UnitY() } },
			node.aabb.max.z() - node.aabb.min.z(),
			node.aabb.max.y() - node.aabb.min.y(),
			csl::ut::Color8{ 0, 255, 0, 255 }
		);
		RenderDebugVisual(ctx, node.left);
		RenderDebugVisual(ctx, node.right);
		break;
	case KdTreeNode::Type::SPLIT_Y:
		ctx.DrawPlane(
			Eigen::Affine3f{ Eigen::Translation3f{ node.aabb.Center().x(), node.deadzoneStartCoord, node.aabb.Center().z() } *Eigen::AngleAxisf{ 1.5707964f, Eigen::Vector3f::UnitX() } },
			node.aabb.max.x() - node.aabb.min.x(),
			node.aabb.max.z() - node.aabb.min.z(),
			csl::ut::Color8{ 255, 0, 0, 255 }
		);
		ctx.DrawPlane(
			Eigen::Affine3f{ Eigen::Translation3f{ node.aabb.Center().x(), node.deadzoneEndCoord, node.aabb.Center().z() } *Eigen::AngleAxisf{ 1.5707964f, Eigen::Vector3f::UnitX() } },
			node.aabb.max.x() - node.aabb.min.x(),
			node.aabb.max.z() - node.aabb.min.z(),
			csl::ut::Color8{ 0, 255, 0, 255 }
		);
		RenderDebugVisual(ctx, node.left);
		RenderDebugVisual(ctx, node.right);
		break;
	case KdTreeNode::Type::SPLIT_Z:
		ctx.DrawPlane(
			Eigen::Affine3f{ Eigen::Translation3f{ node.aabb.Center().x(), node.aabb.Center().y(), node.deadzoneStartCoord } },
			node.aabb.max.x() - node.aabb.min.x(),
			node.aabb.max.y() - node.aabb.min.y(),
			csl::ut::Color8{ 255, 0, 0, 255 }
		);
		ctx.DrawPlane(
			Eigen::Affine3f{ Eigen::Translation3f{ node.aabb.Center().x(), node.aabb.Center().y(), node.deadzoneEndCoord } },
			node.aabb.max.x() - node.aabb.min.x(),
			node.aabb.max.y() - node.aabb.min.y(),
			csl::ut::Color8{ 0, 255, 0, 255 }
		);
		RenderDebugVisual(ctx, node.left);
		RenderDebugVisual(ctx, node.right);
		break;
	}
}

void RenderDebugVisual(hh::gfnd::DrawContext& ctx, const KdTree& tree) {
	RenderDebugVisual(ctx, tree.rootNode->aabb, csl::ut::Color8{ 0, 0, 255, 255 });
	RenderDebugVisual(ctx, *tree.rootNode);
}

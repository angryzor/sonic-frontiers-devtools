#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/math/MathUtils.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include "TextureElement.h"
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	using namespace SurfRide;

	template<> struct SelectionBehaviorTraits<Context> {
		using ObjectType = TextureElement;
	};

	template<> struct MousePickingBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = TextureElement;
		using LocationType = ImVec2;

		using BehaviorTraitsImpl::BehaviorTraitsImpl;

		static bool IsPositionInCrop(const SRS_TEXTURE& texture, const SRS_CROP& crop, const ImVec2& position) {
			ImVec2 scaledPos{ position.x / texture.width, position.y / texture.height };

			return crop.left <= scaledPos.x && crop.right >= scaledPos.x && crop.top <= scaledPos.y && crop.bottom >= scaledPos.y;
		}

		static bool IsCropInRectangle(const SRS_TEXTURE& texture, const ImVec2& start, const ImVec2 end, const SRS_CROP& crop) {
			ImVec2 scaledStart{ start.x / texture.width, start.y / texture.height };
			ImVec2 scaledEnd{ end.x / texture.width, end.y / texture.height };

			return !(crop.right < scaledStart.x || crop.bottom < scaledStart.y || crop.left > scaledEnd.x || crop.top > scaledEnd.y);
		}

		void GetAllClickResults(const ImVec2& position, csl::ut::MoveArray<ObjectType>& results) {
			if (!context.focusedTexture.has_value())
				return;

			auto& texture = context.focusedTexture->GetTexture();

			for (size_t i = 0; i < texture.cropCount; i++)
				if (IsPositionInCrop(texture, texture.crops[i], position))
					results.push_back(CropRef{ context.focusedTexture->project, context.focusedTexture->textureListIndex, context.focusedTexture->textureIndex, i });
		}

		void GetBestClickResult(const ImVec2& position, csl::ut::MoveArray<ObjectType>& results, typename LocationType& location, bool& pickedLocation) {
			location = position;
			pickedLocation = true;

			if (!context.focusedTexture.has_value())
				return;

			auto& texture = context.focusedTexture->GetTexture();

			for (size_t i = 0; i < texture.cropCount; i++)
				if (IsPositionInCrop(texture, texture.crops[i], position)) {
					results.push_back(CropRef{ context.focusedTexture->project, context.focusedTexture->textureListIndex, context.focusedTexture->textureIndex, i });
					return;
				}
		}

		void GetDragResults(const ImVec2& start, const ImVec2 end, csl::ut::MoveArray<ObjectType>& results) {
			if (!context.focusedTexture.has_value())
				return;

			auto& texture = context.focusedTexture->GetTexture();

			for (size_t i = 0; i < texture.cropCount; i++)
				if (IsCropInRectangle(texture, start, end, texture.crops[i]))
					results.push_back(CropRef{ context.focusedTexture->project, context.focusedTexture->textureListIndex, context.focusedTexture->textureIndex, i });
		}

		const char* GetObjectName(ObjectType object) {
			return object.GetName();
		}
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<TextureElement>& elements, csl::geom::Aabb& aabb)
		{
			aabb.min = { INFINITY, INFINITY, INFINITY };
			aabb.max = { -INFINITY, -INFINITY, -INFINITY };

			bool updated{};

			for (auto& el : elements) {
				if (el.type != TextureElement::Type::CROP)
					continue;

				auto& crop = el.crop.GetCrop();

				aabb.AddPoint(Eigen::Vector3f{ crop.left, crop.top, 0.0f });
				aabb.AddPoint(Eigen::Vector3f{ crop.right, crop.top, 0.0f });
				aabb.AddPoint(Eigen::Vector3f{ crop.right, crop.bottom, 0.0f });
				aabb.AddPoint(Eigen::Vector3f{ crop.left, crop.bottom, 0.0f });

				updated = true;
			}

			return updated;
		}
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(TextureElement element) { return element.type == TextureElement::Type::CROP; }
		bool IsRoot(TextureElement element) { return true; }
		TextureElement GetParent(TextureElement element) { return TextureListRef{ nullptr, 0ull }; }
		Eigen::Affine3f GetSelectionSpaceTransform(TextureElement element) const {
			auto& crop = element.crop.GetCrop();
			
			return Eigen::Affine3f{ Eigen::Translation3f{ crop.left, crop.top, 0.0f } };
		}
		void SetSelectionSpaceTransform(TextureElement element, const Eigen::Affine3f& transform) {
			auto& crop = element.crop.GetCrop();

			auto width = crop.right - crop.left;
			auto height = crop.bottom - crop.top;

			Eigen::Vector3f newPos = transform * Eigen::Vector3f{ 0.0f, 0.0f, 0.0f };

			crop.left = newPos.x();
			crop.top = newPos.y();
			crop.right = newPos.x() + width;
			crop.bottom = newPos.y() + height;

			context.RefreshAfterCropUpdate(element.crop);
		}
	};

	template<> struct ScreenSpaceManipulationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		Eigen::Vector3f ImGuiToSelectionSpace(const ImVec2 delta) { return Eigen::Vector3f{ delta.x / context.focusedTexture->GetTexture().width, delta.y / context.focusedTexture->GetTexture().height, 0.0f }; }
	};

	template<> struct PlacementBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		bool CanPlace() const { return context.focusedTexture.has_value(); }
		TextureElement PlaceObject(ImVec2 location) {
			auto placeLoc = ImVec2(location.x / context.focusedTexture->GetTexture().width, location.y / context.focusedTexture->GetTexture().height);

			return context.AddCrop(*context.focusedTexture, { placeLoc.x, placeLoc.y, placeLoc.x + 0.2f, placeLoc.y + 0.2f });
		}
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/Zoom.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/ScreenSpaceManipulation.h>
#include <ui/operation-modes/behaviors/Placement.h>

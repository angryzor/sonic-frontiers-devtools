#pragma once

#include <ui/operation-modes/OperationMode.h>
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include "ProjectTree.h"
#include "Timeline.h"
#include "ElementInspector.h"

class SurfRideEditor : public OperationMode {
public:
	struct Selection {
		enum class Type {
			SCENE,
			CAMERA_DATA,
			LAYER,
			CAST,
		};

		Type type;
		union {
			SurfRide::Scene* scene;
			SurfRide::SRS_CAMERA* cameraData;
			SurfRide::Layer* layer;
			SurfRide::Cast* cast;
		};

		Selection(Type type, SurfRide::Scene* scene);
		Selection(Type type, SurfRide::SRS_CAMERA* cameraData);
		Selection(Type type, SurfRide::Layer* layer);
		Selection(Type type, SurfRide::Cast* cast);

		bool operator==(const Selection& other) const;
	};
private:
    ProjectTree projectTree;
	Timeline timeline;
    ElementInspector elementInspector;
	csl::geom::Aabb selectionAabb;
	bool haveSelectionAabb{};
	bool manipulating{};
	bool selecting{};
	bool dragSelecting{};
public:
	hh::fnd::Reference<hh::ui::GOCSprite> gocSprite;
	csl::ut::MoveArray<Selection> focusedElements{ GetAllocator() };

	SurfRideEditor(csl::fnd::IAllocator* allocator);
	~SurfRideEditor();
	virtual void Render() override;
	void HandleSelection();
	void HandleManipulation();
	SurfRide::Cast* RayCastCast(SurfRide::Cast* cast, const Ray3f& ray);
	bool Intersects(const SurfRide::Cast* cast, const Ray3f& ray);
	bool Intersects(const SurfRide::Transform& transform, const SurfRide::Vector2& size, const Ray3f& ray);
	void Select(const Selection& selection);
	void Deselect();
	void CalcSelectionAabb();
	void UpdateSelectionAabb(const SurfRide::Cast* cast);
	void UpdateSelectionAabb(const Eigen::Matrix4f& transform, const SurfRide::Vector2& size);
	void DrawSelectionAabb();
	bool IsMouseInSelectionAabb();
};
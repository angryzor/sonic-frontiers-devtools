#pragma once

#include <ui/operation-modes/OperationMode.h>
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
	};
private:
    ProjectTree projectTree;
	Timeline timeline;
    ElementInspector elementInspector;
public:
	hh::fnd::Reference<hh::ui::GOCSprite> gocSprite;
	csl::ut::MoveArray<Selection> focusedElements{ GetAllocator() };

	SurfRideEditor(csl::fnd::IAllocator* allocator);
	~SurfRideEditor();
	virtual void Render() override;
};
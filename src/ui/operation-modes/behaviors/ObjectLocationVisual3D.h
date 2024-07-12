#pragma once
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include "ForwardDeclarations.h"

class ObjectLocationVisual3DBehaviorBase : public OperationModeBehavior, public DebugRenderable {
protected:
	hh::fnd::Reference<hh::gfnd::GraphicsGeometry> targetBox{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateGraphicsGeometry)(nullptr, GetAllocator()) };
	hh::fnd::Reference<hh::gfnd::GraphicsGeometry> selectedTargetBox{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateGraphicsGeometry)(nullptr, GetAllocator()) };

	inline static float debugBoxScale{ 0.03f };
	inline static bool scaleChanged{ false };
public:
	ObjectLocationVisual3DBehaviorBase(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode) : OperationModeBehavior{ allocator, operationMode }, DebugRenderable{} {
		UpdateDebugBoxGeometry();
	}

	static constexpr unsigned int id = 4;
	virtual unsigned int GetId() override { return id; }

	static void SetDebugBoxScale(float scale) {
		debugBoxScale = scale;
		scaleChanged = true;
	}

	void Render() {
		if (scaleChanged) {
			UpdateDebugBoxGeometry();
			scaleChanged = false;
		}
	}

	void UpdateDebugBoxGeometry() {
		hh::fnd::Geometry box{ GetAllocator() };
		box.CreateBox({ 0, 0, 0 }, { debugBoxScale, debugBoxScale, debugBoxScale }, csl::math::Quaternion::Identity());
		targetBox->Initialize(GOCVisualDebugDrawRenderer::instance->drawContext, box);
		targetBox->SetColor({ 255, 0, 255, 255 });
		selectedTargetBox->Initialize(GOCVisualDebugDrawRenderer::instance->drawContext, box);
		selectedTargetBox->SetColor({ 255, 255, 0, 255 });
	}
};

template<typename OpModeContext>
class ObjectLocationVisual3DBehavior : public ObjectLocationVisual3DBehaviorBase {
public:
	using Traits = ObjectLocationVisual3DBehaviorTraits<OpModeContext>;
	using ObjectType = typename SelectionBehaviorTraits<OpModeContext>::ObjectType;

private:
	Traits traits;

public:
	ObjectLocationVisual3DBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : ObjectLocationVisual3DBehaviorBase{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	virtual void RenderDebugVisuals(hh::gfnd::DrawContext& ctx) override {
		auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>();

		csl::ut::MoveArray<ObjectType> objects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		traits.GetInvisibleObjects(objects);

		for (auto& object : objects)
			(selectionBehavior->GetSelection().find(object) != -1 ? selectedTargetBox : targetBox)->Render(&ctx, traits.GetWorldTransform(object));
	}
};

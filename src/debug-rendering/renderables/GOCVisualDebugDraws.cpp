#include "GOCVisualDebugDraws.h"
#include <debug-rendering/DebugRenderer.h>

#ifdef DEVTOOLS_TARGET_SDK_wars
constexpr size_t gocVisualDebugDrawSetupAddr = 0x140682D50;
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
constexpr size_t gocVisualDebugDrawSetupAddr = 0x140D06320;
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
constexpr size_t gocVisualDebugDrawSetupAddr = 0x1409F44A0;
#endif

namespace devtools::debug_rendering::renderables {
	using namespace hh::game;
	using namespace hh::gfx;

	GOCVisualDebugDraws* GOCVisualDebugDraws::instance{};

	class GOCMyVisualDebugDraw : public GOCVisualDebugDraw {
	public:
		//hh::fnd::Reference<hh::gfnd::GraphicsGeometry> geometry;
		hh::fnd::Reference<hh::gfnd::GraphicsGeometry> fillGeometry;
		csl::ut::Color8 color{ 0, 0, 0, 0 };
		bool hasGeometry{};

		GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator)
#ifdef DEVTOOLS_TARGET_SDK_wars
			: GOCVisualDebugDraw{}
#else
			: GOCVisualDebugDraw{ allocator }
#endif
			//, geometry{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystemNeedle::CreateGraphicsGeometry)(nullptr, allocator) }
			, fillGeometry{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystemNeedle::CreateGraphicsGeometry)(nullptr, allocator) }
		{
		}

		virtual void OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data) {
			if (event == GOCEvent::ADDED_TO_OBJECT)
				GOCVisualDebugDraws::instance->AddGOC(this);
			if (event == GOCEvent::REMOVED_FROM_OBJECT)
				GOCVisualDebugDraws::instance->RemoveGOC(this);

			GOCVisualDebugDraw::OnGOCEvent(event, ownerGameObject, data);
		}

		static void InstallHooks();

		GOCOMPONENT_CLASS_DECLARATION(GOCMyVisualDebugDraw)
	};
}

HOOK(bool, __fastcall, VisualDebugDrawSetup, gocVisualDebugDrawSetupAddr, devtools::debug_rendering::renderables::GOCMyVisualDebugDraw* gocVisual, const hh::gfx::GOCVisualDebugDraw::SetupInfo& setupInfo) {
	bool ret = originalVisualDebugDrawSetup(gocVisual, setupInfo);

	if (setupInfo.geometry != nullptr) {
		//gocVisual->geometry->InitializeEdge(DebugRenderer::instance->drawContext, *setupInfo.geometry);
		//gocVisual->geometry->SetColor(setupInfo.color);

		csl::ut::Color8 fillColor{ setupInfo.color };
		fillColor.a = devtools::debug_rendering::renderables::GOCVisualDebugDraws::instance->opacity;

		gocVisual->fillGeometry->Initialize(devtools::debug_rendering::DebugRenderer::instance->drawContext, *setupInfo.geometry);
		gocVisual->fillGeometry->SetColor(fillColor);

		gocVisual->hasGeometry = true;
	}

	gocVisual->color = setupInfo.color;

	return ret;
}

namespace devtools::debug_rendering::renderables {
	void GOCMyVisualDebugDraw::InstallHooks() {
		reinterpret_cast<hh::game::GOComponentClass*>(reinterpret_cast<size_t>(hh::gfx::GOCVisualDebugDraw::GetClass()))->instantiator = &GOCMyVisualDebugDraw::Create;
	}

	hh::game::GOComponent* GOCMyVisualDebugDraw::Create(csl::fnd::IAllocator* allocator) {
		return new (std::align_val_t(alignof(GOCMyVisualDebugDraw)), allocator) GOCMyVisualDebugDraw(allocator);
	}

	GOCVisualDebugDraws::GOCVisualDebugDraws(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator } {
		instance = this;
	}

	GOCVisualDebugDraws::~GOCVisualDebugDraws() {
		instance = nullptr;
	}

	void GOCVisualDebugDraws::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
		for (auto& goc : gocs) {
			if (!goc->hasGeometry || (!enabled && !(goc->visualFlags.test(GOCVisualDebugDraw::Flag::VISIBLE))))
				continue;

			goc->fillGeometry->Render(&ctx, goc->worldMatrix);
		}
	}

	void GOCVisualDebugDraws::AddGOC(GOCMyVisualDebugDraw* goc) {
		gocs.push_back(goc);
	}

	void GOCVisualDebugDraws::RemoveGOC(GOCMyVisualDebugDraw* goc) {
		gocs.remove(gocs.find(goc));
	}

	void GOCVisualDebugDraws::InstallHooks() {
		INSTALL_HOOK(VisualDebugDrawSetup);
		GOCMyVisualDebugDraw::InstallHooks();
	}
}

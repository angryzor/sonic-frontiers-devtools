#pragma once
#include <utilities/CompatibleObject.h>
#include "DebugRenderable.h"

namespace devtools::debug_rendering {
	class DebugRenderer : public CompatibleObject {
		class Renderable : public hh::gfnd::Renderable {
			DebugRenderer* renderer;

		public:
			Renderable(csl::fnd::IAllocator* allocator, DebugRenderer* renderer);
			virtual void Render(const hh::gfnd::RenderableParameter* renderableParameter) override;
		};

		hh::fnd::Reference<Renderable> renderable{};
		csl::ut::MoveArray<DebugRenderable*> debugRenderables{ GetAllocator() };

#ifndef DEVTOOLS_TARGET_SDK_wars
		hh::needle::CScratchMemoryContext memCtx;
		hh::gfnd::DrawContext::Unk2 unk2;
		hh::gfnd::DrawContext::Unk3 unk3;
#endif

	public:
		hh::gfnd::DrawContext* drawContext;

		static hh::fnd::Reference<DebugRenderer> instance;

		bool enabled{ false };
		bool renderWhenUIIsHidden{ false };

		DebugRenderer(csl::fnd::IAllocator* allocator);
		static void Init(csl::fnd::IAllocator* allocator);
		static void Deinit();
		void AddRenderable(DebugRenderable* renderable);
		void RemoveRenderable(DebugRenderable* renderable);
		void RenderImGuiVisuals();
	};
}

#pragma once
class GOCMyVisualDebugDraw;
class GOCVisualDebugDrawRenderer : public hh::fnd::ReferencedObject
{
	class Renderable : public hh::gfnd::Renderable {
		GOCVisualDebugDrawRenderer* renderer;

	public:
		Renderable(csl::fnd::IAllocator* allocator, GOCVisualDebugDrawRenderer* renderer);
		virtual void Render(const hh::gfnd::RenderableParameter* renderableParameter) override;
	};

	csl::ut::MoveArray<GOCMyVisualDebugDraw*> gocs{ GetAllocator() };
	hh::fnd::Reference<Renderable> renderable;
	hh::fnd::Reference<hh::gfnd::SharedDebugDrawResource> sharedDDRes;
public:
	hh::needle::CScratchMemoryContext memCtx;
	hh::gfnd::DrawContext::Unk2 unk2;
	hh::gfnd::DrawContext::Unk3 unk3;
	hh::gfnd::DrawContext* drawContext;
	GOCVisualDebugDrawRenderer(csl::fnd::IAllocator* allocator);
	void AddGOC(GOCMyVisualDebugDraw* goc);
	void RemoveGOC(GOCMyVisualDebugDraw* goc);
	static hh::fnd::Reference<GOCVisualDebugDrawRenderer> instance;
	static void InstallHooks();
};

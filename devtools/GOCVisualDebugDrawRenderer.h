#pragma once
class GOCMyVisualDebugDraw;
class GOCVisualDebugDrawRenderer : public hh::fnd::ReferencedObject
{
	csl::ut::MoveArray<GOCMyVisualDebugDraw*> gocs{ GetAllocator() };
public:
	GOCVisualDebugDrawRenderer(csl::fnd::IAllocator* allocator);
	void AddGOC(GOCMyVisualDebugDraw* goc);
	void RemoveGOC(GOCMyVisualDebugDraw* goc);
	static hh::fnd::Reference<GOCVisualDebugDrawRenderer> instance;
	static void InstallHooks();
	void Render();
};

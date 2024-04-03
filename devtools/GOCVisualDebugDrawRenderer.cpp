#include "Pch.h"
#include "GOCVisualDebugDrawRenderer.h"
#include <d3dcompiler.h>
#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler")
#endif
#include "Desktop.h"

using namespace hh::game;
using namespace hh::gfx;

class GOCMyVisualDebugDraw : public GOCVisualDebugDraw {
public:
	hh::fnd::Reference<hh::gfnd::GraphicsGeometry> geometry;
	//hh::fnd::Reference<hh::gfnd::GraphicsGeometry> fillGeometry;
	csl::ut::Color<uint8_t> color{ 0, 0, 0, 0 };
	bool hasGeometry;

	GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator);
	virtual void OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data);

	static void InstallHooks();

	GOCOMPONENT_CLASS_DECLARATION(GOCMyVisualDebugDraw)
};

GOCMyVisualDebugDraw* GOCMyVisualDebugDraw::Create(csl::fnd::IAllocator* allocator) {
	return new (std::align_val_t(alignof(GOCMyVisualDebugDraw)), allocator) GOCMyVisualDebugDraw(allocator);
}

GOCMyVisualDebugDraw::GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator)
	: GOCVisualDebugDraw{ allocator }
	, geometry{ (*rangerssdk::GetAddress(&hh::gfnd::DrawSystemNeedle::CreateGraphicsGeometry))(nullptr, allocator) }
	//, fillGeometry{ (*rangerssdk::GetAddress(&hh::gfnd::DrawSystemNeedle::CreateGraphicsGeometry))(nullptr, allocator) }
{
}

void GOCMyVisualDebugDraw::OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data) {
	if (event == GOCEvent::ACTIVATE)
		GOCVisualDebugDrawRenderer::instance->AddGOC(this);
	if (event == GOCEvent::DEACTIVATE)
		GOCVisualDebugDrawRenderer::instance->RemoveGOC(this);
	
	GOCVisualDebugDraw::OnGOCEvent(event, ownerGameObject, data);
}

void GOCMyVisualDebugDraw::InstallHooks() {
	WRITE_MEMORY(0x143CECB00 + offsetof(hh::game::GOComponentClass, instantiator), void*, &GOCMyVisualDebugDraw::Create);
}

HOOK(bool, __fastcall, VisualDebugDrawSetup, 0x140D06320, GOCMyVisualDebugDraw* gocVisual, const GOCVisualDebugDraw::SetupInfo& setupInfo) {
	bool ret = originalVisualDebugDrawSetup(gocVisual, setupInfo);

	if (setupInfo.geometry != nullptr) {
		gocVisual->geometry->InitializeEdge(GOCVisualDebugDrawRenderer::instance->drawContext, *setupInfo.geometry);
		gocVisual->geometry->SetColor(setupInfo.color);

		//csl::ut::Color8 fillColor{ setupInfo.color };
		//fillColor.a = 50;
		//gocVisual->fillGeometry->Initialize(GOCVisualDebugDrawRenderer::instance->drawContext, *setupInfo.geometry);
		//gocVisual->fillGeometry->SetColor(setupInfo.color);

		gocVisual->hasGeometry = true;
	}

	gocVisual->color = setupInfo.color;

	return ret;
}

hh::fnd::Reference<GOCVisualDebugDrawRenderer> GOCVisualDebugDrawRenderer::instance = nullptr;

void GOCVisualDebugDrawRenderer::InstallHooks() {
	INSTALL_HOOK(VisualDebugDrawSetup);
	GOCMyVisualDebugDraw::InstallHooks();
}

GOCVisualDebugDrawRenderer::GOCVisualDebugDrawRenderer(csl::fnd::IAllocator* allocator)
	: ReferencedObject{ allocator, true }
	, memCtx{ true }
	, unk2{ &memCtx }
	, unk3{ &unk2 }
	//, sharedDDRes{ (*rangerssdk::GetAddress(&hh::gfnd::DrawSystem::CreateSharedDebugDrawResource))(allocator) }
	, drawContext{ (*rangerssdk::GetAddress(&hh::gfnd::DrawSystem::CreateDrawContext))(allocator, &unk3) }
	, renderable{ new (allocator) Renderable(allocator, this) }
{
	renderable->name = "DevTools Debug Overlay";
	hh::gfnd::GraphicsContext::GetInstance()->AddRenderableToViewport(renderable, 7);
}

void GOCVisualDebugDrawRenderer::AddGOC(GOCMyVisualDebugDraw* goc) {
	gocs.push_back(goc);
}

void GOCVisualDebugDrawRenderer::RemoveGOC(GOCMyVisualDebugDraw* goc) {
	gocs.remove(gocs.find(goc));
}

void GOCVisualDebugDrawRenderer::Renderable::Render(const hh::gfnd::RenderableParameter* renderableParameter) {
	if (!renderer->enabled || renderer->gocs.empty())
		return;

	auto* gameManager = hh::game::GameManager::GetInstance();
	if (!gameManager)
		return;

	auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0);
	if (!camera)
		return;

	renderer->drawContext->BeginScene(renderableParameter);

	hh::gfnd::DrawContext::BeginDrawInfo bdi{};
	memcpy(bdi.viewMatrix, camera->viewportData.viewMatrix.data(), sizeof(camera->viewportData.viewMatrix));
	memcpy(bdi.projMatrix, camera->viewportData.projMatrix.data(), sizeof(camera->viewportData.projMatrix));
	renderer->drawContext->BeginDraw(bdi);

	for (auto& goc : renderer->gocs) {
		if (!goc->hasGeometry)
			continue;

		goc->geometry->Render(renderer->drawContext, goc->transformationMatrix);
	}

	renderer->drawContext->EndDraw();
	renderer->drawContext->EndScene();
}

GOCVisualDebugDrawRenderer::Renderable::Renderable(csl::fnd::IAllocator* allocator, GOCVisualDebugDrawRenderer* renderer) : hh::gfnd::Renderable{ allocator }, renderer{ renderer }
{
}
